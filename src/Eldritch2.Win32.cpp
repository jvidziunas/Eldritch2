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
#include <Renderer/Vulkan/EngineService.hpp>
#include <System/Win32/SystemInterface.hpp>
#include <Physics/PhysX/EngineService.hpp>
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

	class Application {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref Application instance.
		Application() : _allocator( UTF8L("Root Allocator") ),
						_scheduler( GetSystemInterface(), GetGlobalAllocator() ),
						_contentProvider( GetSystemInterface() ),
						_engine( GetSystemInterface(), GetScheduler(), GetContentProvider(), GetGlobalAllocator() ),
						_configurationService( GetGameEngine(), GetContentProvider() ),
						_XInputService( GetGameEngine() ),
						_inputService( GetGameEngine() ),
						_networkingService( GetGameEngine() ),
						_scriptService( GetGameEngine() ),
						_physicsService( GetGameEngine() ),
						_rendererService( GetGameEngine() ),
						_audioRendererService( GetGameEngine() ) {}

		~Application() = default;

	// ---------------------------------------------------

		ETForceInlineHint Win32GlobalHeapAllocator& GetGlobalAllocator() {
			return _allocator;
		}

		ETInlineHint System::Win32::SystemInterface& GetSystemInterface() {
			return _systemInterface;
		}

		ETInlineHint Scheduler::Win32::FiberScheduler& GetScheduler() {
			return _scheduler;
		}

		ETInlineHint FileSystem::Win32::ContentProvider& GetContentProvider() {
			return _contentProvider;
		}

		ETInlineHint Foundation::GameEngine& GetGameEngine() {
			return _engine;
		}

	// - DATA MEMBERS ------------------------------------

	private:
		Win32GlobalHeapAllocator				_allocator;
		System::Win32::SystemInterface			_systemInterface;
		Scheduler::Win32::FiberScheduler		_scheduler;
		FileSystem::Win32::ContentProvider		_contentProvider;
		Foundation::GameEngine					_engine;

		Configuration::EngineService			_configurationService;
		Input::XInput::EngineService			_XInputService;
		Input::Win32::EngineService				_inputService;
		Networking::Steamworks::EngineService	_networkingService;
		Scripting::AngelScript::EngineService	_scriptService;
#if 0
		Physics::BulletDynamics::EngineService	_physicsService;
		Renderer::Direct3D11::EngineService		_rendererService;
#else
		Physics::PhysX::EngineService			_physicsService;
		Renderer::Vulkan::EngineService			_rendererService;
#endif
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