/*==================================================================*\
  SteamworksEngineComponent.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Networking/Steamworks/SteamworksEngineComponent.hpp>
#include <Networking/Steamworks/SteamworksWorldComponent.hpp>
#include <Core/PropertyRegistrar.hpp>
#include <Core/Engine.hpp>
#include <Build.hpp>
//------------------------------------------------------------------//
//	(6340) Valve has a few mismatches in their printf specifiers! We can't fix these, so disable the warning.
ET_PUSH_MSVC_WARNING_STATE(disable : 6340)
#include <steam_api.h>
ET_POP_MSVC_WARNING_STATE()
//------------------------------------------------------------------//

//==================================================================//
// LIBRARIES
//==================================================================//
#if ET_PLATFORM_WINDOWS
ET_LINK_LIBRARY("steam_api64.lib")
#endif
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Networking { namespace Steamworks {

	using namespace ::Eldritch2::Scheduling;
	using namespace ::Eldritch2::Logging;
	using namespace ::Eldritch2::Core;

	namespace {

		enum : NetworkPort {
			DefaultWorldPortBegin = 6670u,
			DefaultWorldPortEnd   = 6689u,
			DefaultSteamPort      = 6690u
		};

	} // anonymous namespace

	SteamworksEngineComponent::SteamworksEngineComponent(const ObjectLocator& services) :
		EngineComponent(services),
		_log(FindService<Engine>()->GetLog()),
		_steamPort(DefaultSteamPort),
		_worldPorts(MallocAllocator("Steamworks Port Pool Allocator")),
		_bannedIds(MallocAllocator("Steamworks Banned ID List Allocator")) {
	}

	// ---------------------------------------------------

	SteamworksEngineComponent::~SteamworksEngineComponent() {
		SteamAPI_Shutdown();
	}

	// ---------------------------------------------------

	UniquePointer<WorldComponent> SteamworksEngineComponent::CreateWorldComponent(Allocator& allocator, const ObjectLocator& services) {
		return MakeUnique<SteamworksWorldComponent>(allocator, services);
	}

	// ---------------------------------------------------

	void SteamworksEngineComponent::BindConfigurableResources(JobExecutor& /*executor*/) {
		ET_PROFILE_SCOPE("Engine/Initialization", "Steamworks initialization", 0xBBBBBB);
		if (_worldPorts.IsEmpty()) {
			_log.Write(Severity::VerboseWarning, "No world ports allocated to Steam, using range [{} - {}]." ET_NEWLINE, DefaultWorldPortBegin, DefaultWorldPortEnd);
			_worldPorts.Assign(DefaultWorldPortBegin, DefaultWorldPortEnd);
		}

		//	Attempt to establish a connection with the Steam master servers.
		if (!SteamAPI_Init()) {
			_log.Write(Severity::Error, "Unable to initialize Steam API!" ET_NEWLINE);
			FindService<Engine>()->SetShouldShutDown();
			return;
		}

		SteamClient()->SetLocalIPBinding(0u, _steamPort);
		SteamClient()->SetWarningMessageHook([](int severity, const char* message) {
			enum : int { SteamworksWarning = 0 };
#if ET_PLATFORM_WINDOWS
			OutputDebugStringA(message);
#endif
			if (severity != SteamworksWarning) {
				ET_TRIGGER_DEBUGBREAK();
			}
		});

		_log.Write(Severity::Message, "Connected to local Steam client." ET_NEWLINE);
	}

	// ---------------------------------------------------

	void SteamworksEngineComponent::PublishConfiguration(PropertyRegistrar& properties) {
		ET_PROFILE_SCOPE("Engine/Initialization", "Steamworks property registration", 0xBBBBBB);
		properties.BeginSection("Steamworks")
			.DefineProperty("SteamPort", _steamPort);

		properties.BeginSection("Steamworks.BannedSteamIds")
			.DefineDynamicProperty([this](StringView /*name*/, StringView /*value*/) {
				_bannedIds.Insert(CSteamID());
			});
	}

	// ---------------------------------------------------

	void SteamworksEngineComponent::TickEarly(JobExecutor& /*executor*/) {
		ET_PROFILE_SCOPE("Engine/ServiceTick", "Steamworks callback dispatch", 0xAAAAAA);
		/*	Run callbacks only for the engine-tier Steam pipe. World components will individually maintain separate
		 *	communication channels with the Steam client and are responsible for dispatching their own callbacks. */
		Steam_RunCallbacks(SteamAPI_GetHSteamPipe(), false);
		SteamAPI_ReleaseCurrentThreadMemory();
	}

	// ---------------------------------------------------

	void SteamworksEngineComponent::AddLocalUser() {}

}}} // namespace Eldritch2::Networking::Steamworks
