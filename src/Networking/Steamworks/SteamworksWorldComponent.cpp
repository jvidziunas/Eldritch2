/*==================================================================*\
  SteamworksWorldComponent.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Networking/Steamworks/SteamworksEngineComponent.hpp>
#include <Networking/Steamworks/SteamworksWorldComponent.hpp>
#include <Core/World.hpp>
#include <Build.hpp>
//------------------------------------------------------------------//
//	(6340) Valve has a few mismatches in their printf specifiers, it seems! We can't fix these, so disable the warning.
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

	using namespace ::Eldritch2::Scripting::Wren;
	using namespace ::Eldritch2::Scheduling;
	using namespace ::Eldritch2::Scripting;
	using namespace ::Eldritch2::Logging;
	using namespace ::Eldritch2::Core;

	namespace {

		ETPureFunctionHint ETForceInlineHint const Utf8Char* AsCString(EP2PSessionError error) ETNoexceptHint {
			switch (error) {
			case k_EP2PSessionErrorNotRunningApp: return "User not running the same application";
			case k_EP2PSessionErrorNoRightsToApp: return "User does not have rights to use application";
			case k_EP2PSessionErrorDestinationNotLoggedIn: return "User not connected to Steam";
			case k_EP2PSessionErrorTimeout: return "Session timeout/blocked connection";
			default: return "Unknown";
			};
		}

		// ---------------------------------------------------

		ETInlineHint ETForceInlineHint ISteamGameServer* GetSteamGameServer(HSteamUser user, HSteamPipe pipe) ETNoexceptHint {
			return SteamClient()->GetISteamGameServer(user, pipe, STEAMGAMESERVER_INTERFACE_VERSION);
		}

		// ---------------------------------------------------

		ETInlineHint ETForceInlineHint ISteamNetworking* GetSteamNetworking(HSteamUser user, HSteamPipe pipe) ETNoexceptHint {
			return SteamClient()->GetISteamNetworking(user, pipe, STEAMNETWORKING_INTERFACE_VERSION);
		}

	} // anonymous namespace

	SteamworksWorldComponent::SteamworksWorldComponent(const ObjectLocator& services) :
		WorldComponent(services),
		_connectedToSteam(false),
		_gamePort(0u),
		_queryPort(0u) {
	}

	// ---------------------------------------------------

	SteamworksWorldComponent::~SteamworksWorldComponent() {
		ET_ASSERT(!IsConnectedToSteam(), "Disconnect from Steam before destroying the world!");
	}

	// ---------------------------------------------------

	void SteamworksWorldComponent::BindResources(JobExecutor& /*executor*/) {
		_log.BindResources(FindService<World>()->GetLog());

		ConnectToSteam(_gamePort, _queryPort);
	}

	// ---------------------------------------------------

	void SteamworksWorldComponent::FreeResources(JobExecutor& /*executor*/) {
		DisconnectFromSteam();
	}

	// ---------------------------------------------------

	void SteamworksWorldComponent::OnVariableRateTick(JobExecutor& /*executor*/, MicrosecondTime /*tickDuration*/, float32 /*residualFraction*/) {
		ET_PROFILE_SCOPE("World/Steamworks", "Pump callbacks", 0xBBBBBB);
		if (!IsConnectedToSteam()) {
			return;
		}

		Steam_RunCallbacks(_pipe, true);
		SteamGameServer_ReleaseCurrentThreadMemory();
	}

	// ---------------------------------------------------

	void SteamworksWorldComponent::OnFixedRateTickLate(JobExecutor& /*executor*/, MicrosecondTime /*delta*/) {
		ET_PROFILE_SCOPE("World/Steamworks", "Push local state", 0xBBCCBB);
		if (!IsConnectedToSteam()) {
			return;
		}

		const auto networking(GetSteamNetworking(_serverUser, _pipe));
		for (uint32 packetSize; networking->IsP2PPacketAvailable(ETAddressOf(packetSize));) {
			CSteamID remoteId;
			networking->ReadP2PPacket(nullptr, 0, ETAddressOf(packetSize), ETAddressOf(remoteId));
		}
	}

	// ---------------------------------------------------

	void SteamworksWorldComponent::DefineScriptApi(ApiBuilder& /*api*/) {}

	// ---------------------------------------------------

	bool SteamworksWorldComponent::IsConnectedToSteam() const ETNoexceptHint {
		return _connectedToSteam;
	}

	// ---------------------------------------------------

	ErrorCode SteamworksWorldComponent::ConnectToSteam(uint16 gamePort, uint16 queryPort) {
		enum : uint32 { AnyInboundIp = 0u };

		if (IsConnectedToSteam()) {
			return Error::None;
		}

		if (gamePort == 0u || queryPort == 0u) {
			_log.Write(Severity::Error, "Unable to create world Steamworks presence: No ports assigned!" ET_NEWLINE);
			return Error::InvalidObjectState;
		}

		HSteamPipe pipe(SteamClient()->CreateSteamPipe());
		HSteamUser serverUser(SteamClient()->CreateLocalUser(ETAddressOf(pipe), EAccountType::k_EAccountTypeAnonGameServer));
		/*	Game server callbacks *MUST* be registered here because of Valve's awful OpenGL-style hidden global state. Internally, Steam has a
		 *	notion of an active thread-local callback list that's updated when a thread calls CreateSteamPipe(). Naive implementations that rely
		 *	on the automatic registration in the constructor causes undefined behavior at program shutdown, since they will have been added to a list
		 *	that outlives the SteamworksWorldComponent that registered them-- Valve's shutdown code then stomps all over previously-freed memory. */
		_steamServersConnected.Register(this, ETAddressOf(SteamworksWorldComponent::OnSteamServersConnected));
		_steamServersConnectFailure.Register(this, ETAddressOf(SteamworksWorldComponent::OnSteamServersConnectFailure));
		_steamServersDisconnected.Register(this, ETAddressOf(SteamworksWorldComponent::OnSteamServersDisconnected));
		_peerToPeerSessionRequestInitiated.Register(this, ETAddressOf(SteamworksWorldComponent::OnP2PSessionRequestInitiated));
		_peerToPeerSessionConnectFail.Register(this, ETAddressOf(SteamworksWorldComponent::OnP2PSessionConnectionFail));
		_policyResponse.Register(this, ETAddressOf(SteamworksWorldComponent::OnPolicyResponse));
		_gameServerAuthTicketResponse.Register(this, ETAddressOf(SteamworksWorldComponent::OnValidateAuthTicketResponse));
		_clientKickRequestRecieved.Register(this, ETAddressOf(SteamworksWorldComponent::OnClientKickRequestReceived));

		const auto gameServer(GetSteamGameServer(serverUser, pipe));
		if (!gameServer->InitGameServer(AnyInboundIp, gamePort, queryPort, /*unFlags =*/0u, SteamUtils()->GetAppID(), "0.0.0.0")) {
			_log.Write(Severity::Error, "Unable to bind Steam game server!" ET_NEWLINE);
			return Error::Unspecified;
		}

		gameServer->LogOnAnonymous();

		_pipe             = pipe;
		_serverUser       = serverUser;
		_connectedToSteam = true;

		return Error::None;
	}

	// ---------------------------------------------------

	void SteamworksWorldComponent::DisconnectFromSteam() {
		if (!IsConnectedToSteam()) {
			return;
		}

		const auto gameServer(GetSteamGameServer(_serverUser, _pipe));
		if (gameServer->BLoggedOn()) {
			gameServer->LogOff();
		}

		SteamClient()->ReleaseUser(_pipe, _serverUser);
		SteamClient()->BReleaseSteamPipe(_pipe);

		_connectedToSteam = false;
	}

	// ---------------------------------------------------

	void SteamworksWorldComponent::OnSteamServersConnected(SteamServersConnected_t* /*message*/) {
		_log.Write(Severity::Message, "Steam connection established." ET_NEWLINE);
	}

	// ---------------------------------------------------

	void SteamworksWorldComponent::OnSteamServersConnectFailure(SteamServerConnectFailure_t* /*message*/) {
		_log.Write(Severity::Error, "Error connecting to Steam servers!" ET_NEWLINE);
	}

	// ---------------------------------------------------

	void SteamworksWorldComponent::OnSteamServersDisconnected(SteamServersDisconnected_t* /*message*/) {
		_log.Write(Severity::Message, "Disconnected from Steam servers." ET_NEWLINE);
		DisconnectFromSteam();
	}

	// ---------------------------------------------------

	void SteamworksWorldComponent::OnP2PSessionRequestInitiated(P2PSessionRequest_t* message) {
		const auto steamNetworking(GetSteamNetworking(_serverUser, _pipe));
		if (!steamNetworking->AcceptP2PSessionWithUser(message->m_steamIDRemote)) {
			return;
		}

		_log.Write(Severity::Message, "Accepted peer-to-peer connection to SteamID '{}'." ET_NEWLINE, message->m_steamIDRemote.ConvertToUint64());
	}

	// ---------------------------------------------------

	void SteamworksWorldComponent::OnP2PSessionConnectionFail(P2PSessionConnectFail_t* message) {
		_log.Write(Severity::Warning, "Connection to SteamID '{}' dropped: {}." ET_NEWLINE, message->m_steamIDRemote.ConvertToUint64(), AsCString(EP2PSessionError(message->m_eP2PSessionError)));
	}

	// ---------------------------------------------------

	void SteamworksWorldComponent::OnPolicyResponse(GSPolicyResponse_t*) {}

	// ---------------------------------------------------

	void SteamworksWorldComponent::OnValidateAuthTicketResponse(ValidateAuthTicketResponse_t* /*pParam*/) {}

	// ---------------------------------------------------

	void SteamworksWorldComponent::OnClientKickRequestReceived(GSClientKick_t* kickedClient) {
		const auto steamNetworking(GetSteamNetworking(_serverUser, _pipe));
		const auto gameServer(GetSteamGameServer(_serverUser, _pipe));
		//	EDenyReason m_eDenyReason;
		_log.Write(Severity::Message, "Kicking user '{}'." ET_NEWLINE, kickedClient->m_SteamID.ConvertToUint64());

		steamNetworking->CloseP2PSessionWithUser(kickedClient->m_SteamID);
		gameServer->EndAuthSession(kickedClient->m_SteamID);
	}

}}} // namespace Eldritch2::Networking::Steamworks
