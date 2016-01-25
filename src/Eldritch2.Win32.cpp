/*==================================================================*\
  Eldritch2.Win32.cpp
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
#include <Scheduler/Win32/FiberScheduler.hpp>
#include <Utility/Memory/StandardLibrary.hpp>
#include <System/Win32/SystemInterface.hpp>
#include <Configuration/EngineService.hpp>
#include <Sound/XAudio2/EngineService.hpp>
#include <Input/XInput/EngineService.hpp>
#include <Input/Win32/EngineService.hpp>
#include <Foundation/GameEngine.hpp>
#include <Utility/Assert.hpp>
//------------------------------------------------------------------//
#define BOOST_ERROR_CODE_HEADER_ONLY
#include <boost/system/error_code.hpp>
#include <shellapi.h>
#include <windows.h>
#include <Shlwapi.h>
#include <dbghelp.h>
#include <shlobj.h>
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
						public Scheduler::Win32::FiberScheduler,
						public FileSystem::Win32::ContentProvider,
						public Foundation::GameEngine {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref Application instance.
		Application() : Win32GlobalHeapAllocator( UTF8L("Root Allocator") ),
						Scheduler::Win32::FiberScheduler( GetSystemInterface(), GetGlobalAllocator() ),
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

		ETInlineHint Scheduler::Win32::FiberScheduler& GetScheduler() {
			return static_cast<Scheduler::Win32::FiberScheduler&>(*this);
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

}	// anonymous namespace

 int WINAPI wWinMain( __in ::HINSTANCE hInstance, __in_opt ::HINSTANCE /*hPrevInstance*/, __in ::LPWSTR /*lpCmdLine*/, __in int nCmdShow ) {
	::Eldritch2::Utility::StoreMainArguments( hInstance, nCmdShow );

	const int	returnValue( (new(applicationObjectMemory) Application())->GetGameEngine().ApplicationEntryPoint() );

	reinterpret_cast<Application*>(applicationObjectMemory)->~Application();
	
	return returnValue;
}