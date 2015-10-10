/*==================================================================*\
  Win32EntryPoint.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#define BOOST_ERROR_CODE_HEADER_ONLY
#include <boost/system/error_code.hpp>
#include <Foundation/GameEngine.hpp>
#include <Configuration/ConfigurationService.hpp>
#include <Scripting/AngelscriptEngine.hpp>
#include <Packages/Win32ContentProvider.hpp>
#include <System/Win32SystemInterface.hpp>
#include <Scheduler/Win32Scheduler.hpp>
#include <Renderer/Direct3D11Renderer.hpp>
#include <Physics/BulletEngine.hpp>
#include <Sound/XAudio2AudioRenderer.hpp>
#include <Foundation/Application.hpp>
#include <Input/Win32InputService.hpp>
#include <Input/XInputService.hpp>
#include <Networking/SteamworksNetworkingService.hpp>
#include <Utility/Memory/Win32HeapAllocator.hpp>
#include <Utility/Memory/MemStdLib.hpp>
#include <Utility/Win32ApplicationHelpers.hpp>
#include <Utility/Assert.hpp>
#include <Build.hpp>
#include <shellapi.h>
#include <windows.h>
#include <Shlwapi.h>
#include <dbghelp.h>
#include <shlobj.h>
#include <TCHAR.h>
//------------------------------------------------------------------//

//==================================================================//
// LIBRARIES
//==================================================================//
ET_LINK_LIBRARY( "Shlwapi.lib" )
ET_LINK_LIBRARY( "dbghelp.lib" )
//------------------------------------------------------------------//

using namespace ::Eldritch2::Configuration;
using namespace ::Eldritch2::Networking;
using namespace ::Eldritch2::Foundation;
using namespace ::Eldritch2::FileSystem;
using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Scheduler;
using namespace ::Eldritch2::Renderer;
using namespace ::Eldritch2::Physics;
using namespace ::Eldritch2::Utility;
using namespace ::Eldritch2::Sound;
using namespace ::Eldritch2::Input;
using namespace ::Eldritch2;

#define ET_ENABLE_EXCEPTION_HANDLER 0

namespace {

	class Win32Application : public Win32SystemInterface, public Win32Scheduler, public Win32ContentProvider, public GameEngine {
	public:
		// Constructs this Win32Application instance.
		Win32Application( Allocator& allocator ) : Win32SystemInterface(),
												   Win32Scheduler( GetSystemInterface(), allocator ),
												   Win32ContentProvider( GetSystemInterface() ),
												   GameEngine( GetSystemInterface(), GetScheduler(), GetContentProvider(), allocator ),
												   _configurationService( GetGameEngine(), GetContentProvider() ),
												   _controllerInputService( GetGameEngine() ),
												   _inputService( GetGameEngine() ),
												   _networkingService( GetGameEngine() ),
												   _scriptEngine( GetGameEngine() ),
												   _physicsEngine( GetGameEngine() ),
												   _renderer( GetGameEngine() ),
												   _audioRenderer( GetGameEngine() ) {}

		// Destroys this Win32Application instance.
		~Win32Application() {
			ClearAttachedServices();
		}

	// ---------------------------------------------------

		ETInlineHint Win32SystemInterface& GetSystemInterface() {
			return static_cast<Win32SystemInterface&>(*this);
		}

		ETInlineHint Win32Scheduler& GetScheduler() {
			return static_cast<Win32Scheduler&>(*this);
		}

		ETInlineHint Win32ContentProvider& GetContentProvider() {
			return static_cast<Win32ContentProvider&>(*this);
		}

		ETInlineHint GameEngine& GetGameEngine() {
			return static_cast<GameEngine&>(*this);
		}

	// ---------------------------------------------------

		ConfigurationService		_configurationService;
		XInputService				_controllerInputService;
		Win32InputService			_inputService;
		SteamworksNetworkingService	_networkingService;
		AngelscriptEngine			_scriptEngine;
		BulletEngine				_physicsEngine;
		Direct3D11Renderer			_renderer;
		XAudio2AudioRenderer		_audioRenderer;
	};

// ---------------------------------------------------

	static struct {
		ET16ByteAligned char	globalAllocatorMemory[sizeof(Win32GlobalHeapAllocator)];
		ET16ByteAligned char	applicationObjectMemory[sizeof(Win32Application)];
	}	globalObjects;
	
// ---------------------------------------------------

	static int GenerateDump( EXCEPTION_POINTERS* const exceptionPointers ) {
		::MINIDUMP_EXCEPTION_INFORMATION	exceptionInfo;
		::SYSTEMTIME						localTime;
		::HANDLE							dumpFile;
		::TCHAR								fileName[MAX_PATH];
		::TCHAR								path[MAX_PATH]; 
		bool								dumpSuccessful;

		::GetLocalTime( &localTime );
		::GetTempPath( static_cast<::DWORD>(_countof(path)), path );

		::PathCombine( fileName, path, PROJECT_NAME_LIT );
		::CreateDirectory( fileName, nullptr );

		dumpFile = ::CreateFile( PrintFormatted( fileName, SL("%s%s\\%s-%04d%02d%02d-%02d%02d%02d-%ld-%ld.dmp"), 
												 path, PROJECT_NAME_LIT, VERSION_STRING_LIT, 
												 localTime.wYear, localTime.wMonth, localTime.wDay, 
												 localTime.wHour, localTime.wMinute, localTime.wSecond, 
												 ::GetCurrentProcessId(), ::GetCurrentThreadId() ),
								 ( GENERIC_READ | GENERIC_WRITE ),
								 ( FILE_SHARE_WRITE | FILE_SHARE_READ ),
								 0,
								 CREATE_ALWAYS,
								 0,
								 0 );

		exceptionInfo.ExceptionPointers	= exceptionPointers;
		exceptionInfo.ClientPointers	= TRUE;
		exceptionInfo.ThreadId			= ::GetCurrentThreadId();

		dumpSuccessful = !!::MiniDumpWriteDump( ::GetCurrentProcess(), ::GetCurrentProcessId(), dumpFile, MiniDumpWithDataSegs, &exceptionInfo, nullptr, nullptr );

		return EXCEPTION_EXECUTE_HANDLER;
	}

// ---------------------------------------------------

	static ETForceInlineHint Win32Application&	GetApplicationObject() {
		return *reinterpret_cast<Win32Application*>(globalObjects.applicationObjectMemory);
	}

// ---------------------------------------------------

	static ETForceInlineHint Win32GlobalHeapAllocator& GetGlobalAllocator() {
		return *reinterpret_cast<Win32GlobalHeapAllocator*>(globalObjects.globalAllocatorMemory);
	}

}	// anonymous namespace

 int WINAPI wWinMain( __in ::HINSTANCE hInstance, __in_opt ::HINSTANCE /*hPrevInstance*/, __in ::LPWSTR /*lpCmdLine*/, __in int nCmdShow ) {
	int	returnValue;

#if( ET_ENABLE_EXCEPTION_HANDLER && ET_DEBUG_MODE_ENABLED )
	__try {
#endif
		StoreMainArguments( hInstance, nCmdShow );

		auto* const	globalAllocator( new(globalObjects.globalAllocatorMemory) Win32GlobalHeapAllocator( UTF8L("Root Allocator") ) );

		new(globalObjects.applicationObjectMemory) Win32Application( *globalAllocator );

		returnValue = GetApplicationObject().GetGameEngine().ApplicationEntryPoint();

		GetApplicationObject().~Win32Application();
		globalAllocator->~Win32GlobalHeapAllocator();

#if( ET_ENABLE_EXCEPTION_HANDLER && ET_DEBUG_MODE_ENABLED )
	}  __except( GenerateDump( GetExceptionInformation() ) ) {
		_resetstkoflw();
		returnValue = -1;
	}
#endif
	
	return returnValue;
}