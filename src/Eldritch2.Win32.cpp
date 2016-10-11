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
#include <Networking/Steamworks/EngineService.hpp>
#include <Scripting/Angelscript/EngineService.hpp>
#include <Utility/Memory/Win32HeapAllocator.hpp>
#include <Scheduling/Win32/FiberScheduler.hpp>
#include <Utility/Memory/StandardLibrary.hpp>
#include <Renderer/Vulkan/EngineService.hpp>
#include <Physics/PhysX/EngineService.hpp>
#include <Configuration/EngineService.hpp>
#include <Audio/XAudio2/EngineService.hpp>
#include <Platform/PlatformInterface.hpp>
#include <Input/XInput/EngineService.hpp>
#include <Input/Win32/EngineService.hpp>
#include <Platform/ContentProvider.hpp>
#include <Utility/Assert.hpp>
#include <Core/Engine.hpp>
//------------------------------------------------------------------//
#include <microprofile/microprofile.h>
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
		Application() : _allocator( "Root Allocator" ),
						_platformInterface(),
						_scheduler( _allocator ),
						_contentProvider( _allocator ),
						_engine( _platformInterface, _scheduler, _allocator ),
						_managementService( _engine ),
						_configurationService( _engine ),
						_xinputService( _engine ),
						_win32InputService( _engine ),
						_steamworksService( _engine ),
						_angelscriptService( _engine ),
						_physxService( _engine ),
						_vulkanService( _engine ),
						_xaudio2Service( _engine ) {}

		~Application() = default;

	// ---------------------------------------------------

		ETPureFunctionHint ETInlineHint size_t GetThreadCount() const {
			return Max<size_t>( _platformInterface.GetThreadCount() - 1u, 1u );
		}

		int ApplicationEntryPoint() {
			MicroProfileSetForceEnable( true );
			MicroProfileSetEnableAllGroups( true );
			MicroProfileSetForceMetaCounters( true );

		//	Perform the initial flip so we can get profile data for the initialization process.
			MicroProfileFlip( nullptr );

			_scheduler.Bootstrap( GetThreadCount(), [this] ( Scheduling::JobFiber& executor ) {
				_engine.InitializeWithServices( executor, { _managementService, _configurationService, _xinputService, _win32InputService, _steamworksService, _angelscriptService, _physxService, _vulkanService, _xaudio2Service } );
				_engine.EnterMainLoopOnCaller( executor );

				_scheduler.ShutDown();
			} );

			return 0;
		}

	// - DATA MEMBERS ------------------------------------

	private:
		Win32GlobalHeapAllocator				_allocator;
		Platform::PlatformInterface				_platformInterface;
		Scheduling::Win32::FiberScheduler		_scheduler;
		Platform::ContentProvider				_contentProvider;
		Core::Engine							_engine;

		Core::Engine::ManagementService			_managementService;
		Configuration::EngineService			_configurationService;
		Input::XInput::EngineService			_xinputService;
		Input::Win32::EngineService				_win32InputService;
		Networking::Steamworks::EngineService	_steamworksService;
		Scripting::AngelScript::EngineService	_angelscriptService;
		Physics::PhysX::EngineService			_physxService;
		Renderer::Vulkan::EngineService			_vulkanService;
		Audio::XAudio2::EngineService			_xaudio2Service;
	};

// ---------------------------------------------------

	ET16ByteAligned char	applicationObjectMemory[sizeof(Application)];

}	// anonymous namespace

 int WINAPI wWinMain( __in HINSTANCE /*hInstance*/, __in_opt HINSTANCE /*hPrevInstance*/, __in LPWSTR /*lpCmdLine*/, __in int /*nCmdShow*/ ) {
	const int	returnValue( (new(applicationObjectMemory) Application())->ApplicationEntryPoint() );

	reinterpret_cast<Application*>(applicationObjectMemory)->~Application();
	
	return returnValue;
}