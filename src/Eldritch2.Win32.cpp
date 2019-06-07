/*==================================================================*\
  Eldritch2.Win32.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Networking/Steamworks/SteamworksEngineComponent.hpp>
#include <Navigation/Recast/RecastEngineComponent.hpp>
#include <Graphics/Vulkan/VulkanEngineComponent.hpp>
#include <Flatbuffers/FlatBufferContentDatabase.hpp>
#include <Input/Win32/Win32InputEngineComponent.hpp>
#include <Audio/XAudio2/XAudio2EngineComponent.hpp>
#include <Physics/PhysX/PhysxEngineComponent.hpp>
#include <Input/XInput/XInputEngineComponent.hpp>
#include <Scripting/Wren/WrenEngineComponent.hpp>
#include <Scheduling/Win32/FiberJobSystem.hpp>
#include <Core/Profiler.hpp>
#include <Core/Engine.hpp>
//------------------------------------------------------------------//

//==================================================================//
// LIBRARIES
//==================================================================//
ET_LINK_LIBRARY("Shlwapi.lib")
ET_LINK_LIBRARY("dbghelp.lib")
//------------------------------------------------------------------//

int WINAPI wWinMain(__in HINSTANCE /*hInstance*/, __in_opt HINSTANCE /*hPrevInstance*/, __in LPWSTR /*lpCmdLine*/, __in int /*nCmdShow*/) {
	using namespace ::Eldritch2;
	using EngineType = Core::Engine<
		Core::AbstractEngine::ManagementComponent<FlatBuffers::FlatBufferContentDatabase>,
		Input::XInput::XInputEngineComponent,
		Navigation::Recast::RecastEngineComponent,
		Input::Win32::Win32InputEngineComponent,
		Networking::Steamworks::SteamworksEngineComponent,
		Scripting::Wren::WrenEngineComponent,
		Physics::PhysX::PhysxEngineComponent,
		Graphics::Vulkan::VulkanEngineComponent,
		Audio::XAudio2::XAudio2EngineComponent>;

	ET_PROFILE_APP_BEGIN("Eldritch2");
	ET_AT_SCOPE_EXIT(ET_PROFILE_APP_END());
	ETVerify(Succeeded(InitializeUiThread()), "Error initializing UI thread!");
	ET_AT_SCOPE_EXIT(TerminateUiThread());

	Scheduling::Win32::FiberJobSystem jobSystem;
	EngineType                        engine;
	return AsPosixInt(jobSystem.BootOnCaller(Maximum(GetCoreInfo().physicalCores, 2u) - 1u, [&](Scheduling::JobExecutor& executor) ETNoexceptHint {
		engine.BootOnCaller(executor);
		jobSystem.SetShouldShutDown(Result::Success);
	}));
}
