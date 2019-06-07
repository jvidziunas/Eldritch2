/*==================================================================*\
  SteamworksEngineComponent.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
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
#include <Core/PropertyApiBuilder.hpp>
#include <Core/Profiler.hpp>
#include <Core/Engine.hpp>
//------------------------------------------------------------------//

//==================================================================//
// LIBRARIES
//==================================================================//
ET_LINK_LIBRARY("steam_api64.lib")
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Networking { namespace Steamworks {

	using namespace ::Eldritch2::Scheduling;
	using namespace ::Eldritch2::Logging;
	using namespace ::Eldritch2::Core;

	// ---------------------------------------------------

	ETCacheLineAligned Mutex SteamApiMutex;

	// ---------------------------------------------------

	SteamworksEngineComponent::SteamworksEngineComponent(const ObjectInjector& services) ETNoexceptHint : EngineComponent(services),
																										  _steamPort(NetworkPort::DefaultSteamPort),
																										  _ports(MallocAllocator("Steamworks Port Pool Allocator")),
																										  _bannedIds(MallocAllocator("Steamworks Banned ID List Allocator")) {}

	// ---------------------------------------------------

	SteamworksEngineComponent::~SteamworksEngineComponent() {
		Lock _(SteamApiMutex);
		SteamAPI_Shutdown();
	}

	// ---------------------------------------------------

	void SteamworksEngineComponent::BindConfigurableResources(JobExecutor& /*executor*/) ETNoexceptHint {
		ET_PROFILE_SCOPE("Engine/Initialization", "Steamworks initialization", 0xBBBBBB);

		if (_ports.IsEmpty()) {
			enum : uint16 { DefaultWorldPortBegin = 6670u,
							DefaultWorldPortEnd   = 6689u };

			Inject<Log>()->Write(Severity::VerboseWarning, "No world ports allocated to Steam, using range [{} - {}]." ET_NEWLINE, DefaultWorldPortBegin, DefaultWorldPortEnd);
			_ports.Assign(DefaultWorldPortBegin, DefaultWorldPortEnd);
		}

		{
			ET_LOCK_SCOPE(SteamApiMutex);
			ET_TERMINATE_ENGINE_UNLESS(SteamAPI_Init());
			SteamClient()->SetLocalIPBinding(/*unIp =*/0u, uint16(_steamPort));
			SteamClient()->SetWarningMessageHook([](int severity, const char* message) ETNoexceptHint {
				ETAssert(severity == 0, "Steamworks error: {}", message);
			});
			// End of lock scope.
		} 

		Inject<Log>()->Write(Severity::Message, "Steam initialized successfully." ET_NEWLINE);
	}

	// ---------------------------------------------------

	void SteamworksEngineComponent::PublishApi(PropertyApiBuilder& api) {
		api.DefineGroup("Steamworks")
			.DefineProperty("SteamPort", PropertyApiBuilder::MakeSetter(_steamPort));
		api.DefineGroup("Steamworks.BannedSteamIds")
			.DefineProperty("Id", [this](StringSpan id) ETNoexceptHint {
				uint32 steamId;
				if (id.Consume(steamId)) {
					_bannedIds.Insert(CSteamID(steamId, k_EUniversePublic, k_EAccountTypeIndividual));
				}
			});
	}

	// ---------------------------------------------------

	void SteamworksEngineComponent::TickEarly(JobExecutor& /*executor*/) ETNoexceptHint {
		ET_PROFILE_SCOPE("Engine/ServiceTick", "Steamworks callback dispatch", 0xAAAAAA);

		/*	Run callbacks only for the engine-tier Steam pipe. World components will individually maintain separate
		 *	communication channels with the Steam client and are responsible for dispatching their own callbacks. */
		Steam_RunCallbacks(SteamAPI_GetHSteamPipe(), /*bGameServerCallbacks =*/false);
		SteamAPI_ReleaseCurrentThreadMemory();
	}

	// ---------------------------------------------------

	void SteamworksEngineComponent::AddLocalUser() {}

}}} // namespace Eldritch2::Networking::Steamworks
