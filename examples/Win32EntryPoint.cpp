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
#include <Networking/SteamworksNetworkingService.hpp>
#include <Physics/BulletDynamics/EngineService.hpp>
#include <Renderer/Direct3D11/RendererService.hpp>
#include <Scripting/Angelscript/EngineService.hpp>
#include <Configuration/ConfigurationService.hpp>
#include <Utility/Memory/Win32HeapAllocator.hpp>
#include <FileSystem/Win32/ContentProvider.hpp>
#include <Utility/Win32ApplicationHelpers.hpp>
#include <Utility/Memory/StandardLibrary.hpp>
#include <Scheduler/Win32/TaskScheduler.hpp>
#include <Sound/XAudio2/RendererService.hpp>
#include <System/Win32/SystemInterface.hpp>
#include <Input/Win32InputService.hpp>
#include <Foundation/GameEngine.hpp>
#include <Input/XInputService.hpp>
#include <Utility/Assert.hpp>
#include <Build.hpp>
//------------------------------------------------------------------//
#define BOOST_ERROR_CODE_HEADER_ONLY
#include <boost/system/error_code.hpp>
#include <shellapi.h>
#include <windows.h>
#include <Shlwapi.h>
#include <dbghelp.h>
#include <shlobj.h>
#include <TCHAR.h>
//------------------------------------------------------------------//
#include <memory>
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
using namespace ::Eldritch2::System;
using namespace ::Eldritch2::Sound;
using namespace ::Eldritch2::Input;
using namespace ::Eldritch2;

#define ET_ENABLE_EXCEPTION_HANDLER 0

namespace {

	using GlobalAllocator	= Win32GlobalHeapAllocator;

	class Win32Application : public GlobalAllocator,
							 public ::Eldritch2::System::Win32::SystemInterface,
							 public ::Eldritch2::Scheduler::Win32::TaskScheduler,
							 public ::Eldritch2::FileSystem::Win32::ContentProvider,
							 public GameEngine {
	public:
		//!	Constructs this @ref Win32Application instance.
		Win32Application() : GlobalAllocator( UTF8L("Root Allocator") ),
							 ::Eldritch2::Scheduler::Win32::TaskScheduler( GetSystemInterface(), GetGlobalAllocator() ),
							 ::Eldritch2::FileSystem::Win32::ContentProvider( GetSystemInterface() ),
							 GameEngine( GetSystemInterface(), GetScheduler(), GetContentProvider(), GetGlobalAllocator() ),
							 _configurationService( GetGameEngine(), GetContentProvider() ),
							 _controllerInputService( GetGameEngine() ),
							 _inputService( GetGameEngine() ),
							 _networkingService( GetGameEngine() ),
							 _scriptService( GetGameEngine() ),
							 _physicsService( GetGameEngine() ),
							 _direct3DRendererService( GetGameEngine() ),
							 _audioRendererService( GetGameEngine() ) {}

		//!	Destroys this @ref Win32Application instance.
		~Win32Application() {
			ClearAttachedServices();
		}

	// ---------------------------------------------------

		ETForceInlineHint GlobalAllocator& GetGlobalAllocator() {
			return static_cast<GlobalAllocator&>(*this);
		}

		ETInlineHint ::Eldritch2::System::Win32::SystemInterface& GetSystemInterface() {
			return static_cast<::Eldritch2::System::Win32::SystemInterface&>(*this);
		}

		ETInlineHint ::Eldritch2::Scheduler::Win32::TaskScheduler& GetScheduler() {
			return static_cast<::Eldritch2::Scheduler::Win32::TaskScheduler&>(*this);
		}

		ETInlineHint ::Eldritch2::FileSystem::Win32::ContentProvider& GetContentProvider() {
			return static_cast<::Eldritch2::FileSystem::Win32::ContentProvider&>(*this);
		}

		ETInlineHint GameEngine& GetGameEngine() {
			return static_cast<GameEngine&>(*this);
		}

	// ---------------------------------------------------

		ConfigurationService				_configurationService;
		XInputService						_controllerInputService;
		Win32InputService					_inputService;
		SteamworksNetworkingService			_networkingService;
		AngelScript::EngineService			_scriptService;
		BulletDynamics::EngineService		_physicsService;
		Direct3D11::RendererService			_direct3DRendererService;
		Sound::XAudio2::RendererService		_audioRendererService;
	};

// ---------------------------------------------------

	ET16ByteAligned char	applicationObjectMemory[sizeof(Win32Application)];
	
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
								 (GENERIC_READ | GENERIC_WRITE),
								 (FILE_SHARE_WRITE | FILE_SHARE_READ),
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

}	// anonymous namespace

 int WINAPI wWinMain( __in ::HINSTANCE hInstance, __in_opt ::HINSTANCE /*hPrevInstance*/, __in ::LPWSTR /*lpCmdLine*/, __in int nCmdShow ) {
	StoreMainArguments( hInstance, nCmdShow );

	const int	returnValue( (new(applicationObjectMemory) Win32Application())->GetGameEngine().ApplicationEntryPoint() );

	reinterpret_cast<Win32Application*>(applicationObjectMemory)->~Win32Application();
	
	return returnValue;
}