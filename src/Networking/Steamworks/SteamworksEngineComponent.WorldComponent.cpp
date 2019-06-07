/*==================================================================*\
  SteamworksEngineComponent.WorldComponent.cpp
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
#include <Core/Profiler.hpp>
#include <Logging/Log.hpp>
#include <Core/World.hpp>
#include <Build.hpp>
//------------------------------------------------------------------//

//==================================================================//
// LIBRARIES
//==================================================================//
ET_LINK_LIBRARY("steam_api64.lib")
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Networking { namespace Steamworks {

	using namespace ::Eldritch2::Scheduling;
	using namespace ::Eldritch2::Scripting;
	using namespace ::Eldritch2::Logging;
	using namespace ::Eldritch2::Core;

	// ---------------------------------------------------

	namespace {

		ETForceInlineHint ISteamGameServer* GetSteamGameServer(HSteamUser user, HSteamPipe pipe) ETNoexceptHint {
			return SteamClient()->GetISteamGameServer(user, pipe, STEAMGAMESERVER_INTERFACE_VERSION);
		}

	} // anonymous namespace

	extern Mutex SteamApiMutex;

	SteamworksEngineComponent::WorldComponent::WorldComponent(const ObjectInjector& services) ETNoexceptHint : Core::WorldComponent(services), _connectedToSteam(false), _port(NetworkPort::Unspecified) {}

	// ---------------------------------------------------

	SteamworksEngineComponent::WorldComponent::~WorldComponent() {
		ETAssert(!IsConnectedToSteam(), "Disconnect from Steam before destroying the world!");
	}

	// ---------------------------------------------------

	void SteamworksEngineComponent::WorldComponent::BindResources(JobExecutor& /*executor*/) ETNoexceptHint {
		ET_TERMINATE_WORLD_IF_FAILED(ConnectToSteam(_port));
	}

	// ---------------------------------------------------

	void SteamworksEngineComponent::WorldComponent::FreeResources(JobExecutor& /*executor*/) ETNoexceptHint {
		DisconnectFromSteam();
	}

	// ---------------------------------------------------

	void SteamworksEngineComponent::WorldComponent::OnVariableRateTick(JobExecutor& /*executor*/, MicrosecondTime /*tickDuration*/, float32 /*residualFraction*/) ETNoexceptHint {
		ET_PROFILE_SCOPE("World/Steamworks", "Pump callbacks", 0xBBBBBB);

		if (IsConnectedToSteam()) {
			Steam_RunCallbacks(_pipe, true);
			SteamGameServer_ReleaseCurrentThreadMemory();
		}
	}

	// ---------------------------------------------------

	void SteamworksEngineComponent::WorldComponent::OnFixedRateTickLate(JobExecutor& /*executor*/, MicrosecondTime /*delta*/) ETNoexceptHint {
		ET_PROFILE_SCOPE("World/Steamworks", "Push local state", 0xBBCCBB);

		if (IsConnectedToSteam()) {
			ET_LOCK_SCOPE(SteamApiMutex);
			const auto networking(SteamClient()->GetISteamNetworking(_serverUser, _pipe, STEAMNETWORKING_INTERFACE_VERSION));
			for (uint32 packetByteSize; networking->IsP2PPacketAvailable(ETAddressOf(packetByteSize));) {
				CSteamID remoteId;
				networking->ReadP2PPacket(nullptr, 0, ETAddressOf(packetByteSize), ETAddressOf(remoteId));
			}
		} // End of lock scope.
	}

	// ---------------------------------------------------

	ETConstexpr bool SteamworksEngineComponent::WorldComponent::IsConnectedToSteam() const ETNoexceptHint {
		return _connectedToSteam;
	}

	// ---------------------------------------------------

	Result SteamworksEngineComponent::WorldComponent::ConnectToSteam(NetworkPort port) {
		enum : uint32 { AnyInboundIp = 0u };

		if (IsConnectedToSteam()) {
			return Result::Success;
		}

		ET_ABORT_IF_MSG(port != NetworkPort::Unspecified ? Result::Success : Error::InvalidParameter, (*Inject<Log>()), "Error connecting to Steam: No ports assigned!" ET_NEWLINE);

		ET_LOCK_SCOPE(SteamApiMutex);
		HSteamPipe pipe(SteamClient()->CreateSteamPipe());
		HSteamUser serverUser(SteamClient()->CreateLocalUser(ETAddressOf(pipe), EAccountType::k_EAccountTypeAnonGameServer));
		/*	Game server callbacks *MUST* be registered here because of Valve's awful OpenGL-style hidden global state. Internally, Steam has a
		 *	notion of an active thread-local callback list that's updated when a thread calls CreateSteamPipe(). Naive implementations that rely
		 *	on the automatic registration in the constructor causes undefined behavior at program shutdown, since they will have been added to a list
		 *	that outlives the SteamworksEngineComponent::WorldComponent that registered them-- Valve's shutdown code then stomps all over previously-freed memory. */
		_serversConnected.Register(this, ETAddressOf(WorldComponent::OnSteamConnection));
		_serversFailedToConnect.Register(this, ETAddressOf(WorldComponent::OnSteamConnectionFailure));
		_serversDisconnected.Register(this, ETAddressOf(WorldComponent::OnSteamDisconnection));
		_peerToPeerSessionInitiated.Register(this, ETAddressOf(WorldComponent::OnP2PSessionRequestInitiated));
		_peerToPeerSessionFailure.Register(this, ETAddressOf(WorldComponent::OnP2PSessionConnectionFail));
		_serverPolicyResponse.Register(this, ETAddressOf(WorldComponent::OnServerPolicyResponse));
		_authResponseReceived.Register(this, ETAddressOf(WorldComponent::OnAuthResponse));
		_clientKicked.Register(this, ETAddressOf(WorldComponent::OnClientKick));

		const auto gameServer(GetSteamGameServer(serverUser, pipe));
		if (!gameServer->InitGameServer(AnyInboundIp, port, queryPort, /*unFlags =*/0u, SteamUtils()->GetAppID(), "0.0.0.0")) {
			Inject<Log>()->Write(Severity::Error, "Unable to bind Steam game server!" ET_NEWLINE);
			return Result::Unspecified;
		}

		gameServer->LogOnAnonymous();

		_pipe             = pipe;
		_serverUser       = serverUser;
		_connectedToSteam = true;

		return Result::Success;
	}

	// ---------------------------------------------------

	void SteamworksEngineComponent::WorldComponent::DisconnectFromSteam() {
		if (!IsConnectedToSteam()) {
			return;
		}

		ET_LOCK_SCOPE(SteamApiMutex);
		const auto gameServer(GetSteamGameServer(_serverUser, _pipe));
		if (gameServer->BLoggedOn()) {
			gameServer->LogOff();
		}

		SteamClient()->ReleaseUser(_pipe, _serverUser);
		SteamClient()->BReleaseSteamPipe(_pipe);

		_connectedToSteam = false;
	}

	// ---------------------------------------------------

	void SteamworksEngineComponent::WorldComponent::OnSteamConnection(SteamServersConnected_t* /*message*/) ETNoexceptHint {
		Inject<Log>()->Write(Severity::Message, "Steam connection established." ET_NEWLINE);
	}

	// ---------------------------------------------------

	void SteamworksEngineComponent::WorldComponent::OnSteamConnectionFailure(SteamServerConnectFailure_t* /*message*/) ETNoexceptHint {
		Inject<Log>()->Write(Severity::Error, "Error connecting to Steam servers!" ET_NEWLINE);
	}

	// ---------------------------------------------------

	void SteamworksEngineComponent::WorldComponent::OnSteamDisconnection(SteamServersDisconnected_t* /*message*/) {
		Inject<Log>()->Write(Severity::Message, "Disconnected from Steam servers." ET_NEWLINE);
		DisconnectFromSteam();
	}

	// ---------------------------------------------------

	void SteamworksEngineComponent::WorldComponent::OnPeerToPeerSessionInitiation(P2PSessionRequest_t* session) {
		const auto networking(SteamClient()->GetISteamNetworking(_serverUser, _pipe, STEAMNETWORKING_INTERFACE_VERSION));
		if (!networking->AcceptP2PSessionWithUser(session->m_steamIDRemote)) {
			return;
		}

		Inject<Log>()->Write(Severity::Message, "Accepted peer-to-peer connection to {}." ET_NEWLINE, session->m_steamIDRemote.ConvertToUint64());
	}

	// ---------------------------------------------------

	void SteamworksEngineComponent::WorldComponent::OnPeerToPeerSessionFailure(P2PSessionConnectFail_t* session) {
		Inject<Log>()->Write(Severity::Warning, "Connection to {} dropped: {}." ET_NEWLINE, session->m_steamIDRemote.ConvertToUint64(), AsCString(EP2PSessionError(session->m_eP2PSessionError)));
	}

	// ---------------------------------------------------

	void SteamworksEngineComponent::WorldComponent::OnServerPolicyResponse(GSPolicyResponse_t* /*message*/) {}

	// ---------------------------------------------------

	void SteamworksEngineComponent::WorldComponent::OnAuthResponse(ValidateAuthTicketResponse_t* /*message*/) {}

	// ---------------------------------------------------

	void SteamworksEngineComponent::WorldComponent::OnClientKick(GSClientKick_t* client) {
		const auto networking(SteamClient()->GetISteamNetworking(_serverUser, _pipe, STEAMNETWORKING_INTERFACE_VERSION));
		const auto server(GetSteamGameServer(_serverUser, _pipe));
		//	EDenyReason m_eDenyReason;
		Inject<Log>()->Write(Severity::Message, "Kicking user '{}'." ET_NEWLINE, client->m_SteamID.ConvertToUint64());

		networking->CloseP2PSessionWithUser(client->m_SteamID);
		server->EndAuthSession(client->m_SteamID);
	}

}}} // namespace Eldritch2::Networking::Steamworks
