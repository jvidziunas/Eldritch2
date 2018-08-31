/*==================================================================*\
  Eldritch2.Win32.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Networking/Steamworks/SteamworksEngineComponent.hpp>
#include <Navigation/Recast/RecastEngineComponent.hpp>
#include <Graphics/Vulkan/VulkanEngineComponent.hpp>
#include <Input/Win32/Win32InputEngineComponent.hpp>
#include <Audio/XAudio2/XAudio2EngineComponent.hpp>
#include <Physics/PhysX/PhysxEngineComponent.hpp>
#include <Input/XInput/XInputEngineComponent.hpp>
#include <Scripting/Wren/WrenEngineComponent.hpp>
#include <Scheduling/Win32/FiberJobSystem.hpp>
#include <Core/Engine.hpp>
//------------------------------------------------------------------//
#include <shellapi.h>
#include <windows.h>
#include <Shlwapi.h>
#include <dbghelp.h>
#include <shlobj.h>
//------------------------------------------------------------------//

//==================================================================//
// LIBRARIES
//==================================================================//
ET_LINK_LIBRARY("Shlwapi.lib")
ET_LINK_LIBRARY("dbghelp.lib")
//------------------------------------------------------------------//

using namespace ::Eldritch2::Networking::Steamworks;
using namespace ::Eldritch2::Navigation::Recast;
using namespace ::Eldritch2::Scheduling::Win32;
using namespace ::Eldritch2::Graphics::Vulkan;
using namespace ::Eldritch2::Scripting::Wren;
using namespace ::Eldritch2::Physics::PhysX;
using namespace ::Eldritch2::Audio::XAudio2;
using namespace ::Eldritch2::Input::XInput;
using namespace ::Eldritch2::Input::Win32;
using namespace ::Eldritch2::Scheduling;
using namespace ::Eldritch2::Core;
using namespace ::Eldritch2;

namespace {

class Application {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Disable copy construction.
	Application(const Application&) = delete;
	//!	Constructs this @ref Application instance.
	Application() :
		_jobSystem(),
		_engine(),
		_managementComponent(_engine),
		_xinputComponent(_engine.GetServiceLocator()),
		_navigationComponent(_engine.GetServiceLocator()),
		_win32InputComponent(_engine.GetServiceLocator()),
		_steamworksComponent(_engine.GetServiceLocator()),
		_wrenComponent(_engine.GetServiceLocator()),
		_physxComponent(_engine.GetServiceLocator()),
		_vulkanComponent(_engine.GetServiceLocator()),
		_xaudio2Component(_engine.GetServiceLocator()) {
	}

	~Application() = default;

	// ---------------------------------------------------

public:
	ErrorCode BootOnCaller() {
		return _jobSystem.BootOnCaller(Max(GetCoreInfo().physicalCores, 2u) - 1u, [this](JobExecutor& executor) {
			_jobSystem.SetShouldShutDown(_engine.BootOnCaller(
				executor,
				_managementComponent,
				_xinputComponent,
				_navigationComponent,
				_win32InputComponent,
				_steamworksComponent,
				_wrenComponent,
				_physxComponent,
				_vulkanComponent,
				_xaudio2Component));
		});
	}

	// ---------------------------------------------------

	//!	Disable copy assignment.
	Application& operator=(const Application&) = delete;

	// - DATA MEMBERS ------------------------------------

private:
	FiberJobSystem              _jobSystem;
	Engine                      _engine;
	Engine::ManagementComponent _managementComponent;
	XInputEngineComponent       _xinputComponent;
	RecastEngineComponent       _navigationComponent;
	Win32InputEngineComponent   _win32InputComponent;
	SteamworksEngineComponent   _steamworksComponent;
	WrenEngineComponent         _wrenComponent;
	PhysxEngineComponent        _physxComponent;
	VulkanEngineComponent       _vulkanComponent;
	XAudio2EngineComponent      _xaudio2Component;
};

} // anonymous namespace

int WINAPI wWinMain(__in HINSTANCE /*hInstance*/, __in_opt HINSTANCE /*hPrevInstance*/, __in LPWSTR /*lpCmdLine*/, __in int /*nCmdShow*/) {
	ET_PROFILE_APP_BEGIN("Eldritch2");
	ET_AT_SCOPE_EXIT(ET_PROFILE_APP_END());
	/*	While the Application structure is pretty big overall, the fiber scheduler means this thread gets a "fresh" stack during bootstrap.
	 *	Thus, it makes more sense to put this on the stack rather than in the image data segment. */
	return AsPosixInt(Application().BootOnCaller());
}
