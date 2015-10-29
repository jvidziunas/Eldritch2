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
#include <Physics/BulletDynamics/EngineService.hpp>
#include <Networking/Steamworks/EngineService.hpp>
#include <Scripting/Angelscript/EngineService.hpp>
#include <Renderer/Direct3D11/EngineService.hpp>
#include <Utility/Memory/Win32HeapAllocator.hpp>
#include <FileSystem/Win32/ContentProvider.hpp>
#include <Utility/Win32ApplicationHelpers.hpp>
#include <Utility/Memory/StandardLibrary.hpp>
#include <Scheduler/Win32/TaskScheduler.hpp>
#include <System/Win32/SystemInterface.hpp>
#include <Configuration/EngineService.hpp>
#include <Sound/XAudio2/EngineService.hpp>
#include <Input/XInput/EngineService.hpp>
#include <Input/Win32/EngineService.hpp>
#include <Foundation/GameEngine.hpp>
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

namespace {

	using namespace ::Eldritch2;

	class Application : public Win32GlobalHeapAllocator,
						public System::Win32::SystemInterface,
						public Scheduler::Win32::TaskScheduler,
						public FileSystem::Win32::ContentProvider,
						public Foundation::GameEngine {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref Application instance.
		Application() : Win32GlobalHeapAllocator( UTF8L("Root Allocator") ),
						Scheduler::Win32::TaskScheduler( GetSystemInterface(), GetGlobalAllocator() ),
						FileSystem::Win32::ContentProvider( GetSystemInterface() ),
						Foundation::GameEngine( GetSystemInterface(), GetScheduler(), GetContentProvider(), GetGlobalAllocator() ),
						_configurationService( GetGameEngine(), GetContentProvider() ),
						_XInputService( GetGameEngine() ),
						_inputService( GetGameEngine() ),
						_networkingService( GetGameEngine() ),
						_scriptService( GetGameEngine() ),
						_physicsService( GetGameEngine() ),
						_direct3DRendererService( GetGameEngine() ),
						_audioRendererService( GetGameEngine() ) {}

		//!	Destroys this @ref Application instance.
		~Application() {
			ClearAttachedServices();
		}

	// ---------------------------------------------------

		ETForceInlineHint Win32GlobalHeapAllocator& GetGlobalAllocator() {
			return static_cast<Win32GlobalHeapAllocator&>(*this);
		}

		ETInlineHint System::Win32::SystemInterface& GetSystemInterface() {
			return static_cast<System::Win32::SystemInterface&>(*this);
		}

		ETInlineHint Scheduler::Win32::TaskScheduler& GetScheduler() {
			return static_cast<Scheduler::Win32::TaskScheduler&>(*this);
		}

		ETInlineHint FileSystem::Win32::ContentProvider& GetContentProvider() {
			return static_cast<FileSystem::Win32::ContentProvider&>(*this);
		}

		ETInlineHint Foundation::GameEngine& GetGameEngine() {
			return static_cast<Foundation::GameEngine&>(*this);
		}

	// - DATA MEMBERS ------------------------------------

	private:
		Configuration::EngineService			_configurationService;
		Input::XInput::EngineService			_XInputService;
		Input::Win32::EngineService				_inputService;
		Networking::Steamworks::EngineService	_networkingService;
		Scripting::AngelScript::EngineService	_scriptService;
		Physics::BulletDynamics::EngineService	_physicsService;
		Renderer::Direct3D11::EngineService		_direct3DRendererService;
		Sound::XAudio2::EngineService			_audioRendererService;
	};

// ---------------------------------------------------

	ET16ByteAligned char	applicationObjectMemory[sizeof(Application)];
	
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

		dumpFile = ::CreateFile( ::Eldritch2::PrintFormatted( fileName, SL("%s%s\\%s-%04d%02d%02d-%02d%02d%02d-%ld-%ld.dmp"), 
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
	::Eldritch2::Utility::StoreMainArguments( hInstance, nCmdShow );

	const int	returnValue( (new(applicationObjectMemory) Application())->GetGameEngine().ApplicationEntryPoint() );

	reinterpret_cast<Application*>(applicationObjectMemory)->~Application();
	
	return returnValue;
}