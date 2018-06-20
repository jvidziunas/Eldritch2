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
#include <microprofile/microprofile.h>
ET_PUSH_COMPILER_WARNING_STATE()
//	(6340) Valve has a few mismatches in their printf specifiers, it seems! We can't fix these, so disable the warning.
ET_SET_MSVC_WARNING_STATE(disable : 6340)
#	include <steam_api.h>
ET_POP_COMPILER_WARNING_STATE()
//------------------------------------------------------------------//

//==================================================================//
// LIBRARIES
//==================================================================//
#if ET_PLATFORM_WINDOWS
ET_LINK_LIBRARY("steam_api64.lib")
#endif
//------------------------------------------------------------------//

using namespace ::Eldritch2::Scheduling;
using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Logging;
using namespace ::Eldritch2::Core;

namespace Eldritch2 {
	namespace Networking {
		namespace Steamworks {
			namespace {

				ETPureFunctionHint const Utf8Char* GetErrorString(EP2PSessionError error) {
					switch (error) {
					case k_EP2PSessionErrorNotRunningApp:			return "User not running the same application";
					case k_EP2PSessionErrorNoRightsToApp:			return "User does not have rights to use application";
					case k_EP2PSessionErrorDestinationNotLoggedIn:	return "User not connected to Steam";
					case k_EP2PSessionErrorTimeout:					return "Session timeout/blocked connection";
					default:										return "Unknown";
					};
				}

			// ---------------------------------------------------

				ETInlineHint ISteamGameServer* GetSteamGameServer(HSteamUser user, HSteamPipe pipe) {
					return SteamClient()->GetISteamGameServer(user, pipe, STEAMGAMESERVER_INTERFACE_VERSION);
				}

			// ---------------------------------------------------

				ETInlineHint ISteamNetworking* GetSteamNetworking(HSteamUser user, HSteamPipe pipe) {
					return SteamClient()->GetISteamNetworking(user, pipe, STEAMNETWORKING_INTERFACE_VERSION);
				}

			}	// anonymous namespace

			SteamworksWorldComponent::SteamworksWorldComponent(
				const World& owner
			) : WorldComponent(owner.GetServices()),
				_log(owner.GetLog()),
				_connectedToSteam(false),
				_gamePort(0u),
				_queryPort(0u) {
			}

		// ---------------------------------------------------

			SteamworksWorldComponent::~SteamworksWorldComponent() {
				ET_ASSERT(!IsConnectedToSteam(), "Disconnect from Steam before destroying the world!");
			}

		// ---------------------------------------------------

			void SteamworksWorldComponent::AcceptVisitor(JobExecutor& /*executor*/, const LateInitializationVisitor) {
				ConnectToSteam();
			}

		// ---------------------------------------------------

			void SteamworksWorldComponent::AcceptVisitor(JobExecutor& /*executor*/, const VariableTickVisitor&) {
				MICROPROFILE_SCOPEI("World/Steamworks", "Pump callbacks", 0xBBBBBB);

				if (IsConnectedToSteam()) {
					Steam_RunCallbacks(_pipe, true);
					SteamGameServer_ReleaseCurrentThreadMemory();
				}
			}

		// ---------------------------------------------------

			void SteamworksWorldComponent::AcceptVisitor(JobExecutor& /*executor*/, const LateTickVisitor&) {
				MICROPROFILE_SCOPEI("World/Steamworks", "Push local state", 0xBBCCBB);

				if (IsConnectedToSteam()) {
					ISteamNetworking* const networking(GetSteamNetworking(_serverUser, _pipe));

					for (uint32 packetSize; networking->IsP2PPacketAvailable(&packetSize); ) {
						CSteamID remoteId;

						networking->ReadP2PPacket(nullptr, 0, &packetSize, &remoteId);
					}
				}
			}

		// ---------------------------------------------------

			void SteamworksWorldComponent::AcceptVisitor(JobExecutor& /*executor*/, const TearDownVisitor) {
				DisconnectFromSteam();
			}

		// ---------------------------------------------------

			void SteamworksWorldComponent::AcceptVisitor(Wren::ApiBuilder& /*api*/) {}

		// ---------------------------------------------------

			bool SteamworksWorldComponent::IsConnectedToSteam() const {
				return _connectedToSteam;
			}

		// ---------------------------------------------------

			bool SteamworksWorldComponent::ConnectToSteam() {
				enum : uint32 { AnyInboundIp = 0u };

				if (IsConnectedToSteam()) {
					return true;
				}

				if (_gamePort == 0u || _queryPort == 0u) {
					_log.Write(MessageType::Error, "Unable to create world Steamworks presence: No ports assigned!" UTF8_NEWLINE);
					return false;
				}

				HSteamPipe	pipe(SteamClient()->CreateSteamPipe());
				HSteamUser	serverUser(SteamClient()->CreateLocalUser(&pipe, EAccountType::k_EAccountTypeAnonGameServer));

			/*	Game server callbacks *MUST* be registered here because of Valve's awful OpenGL-style hidden global state. Internally, Steam has a
			 *	notion of an active thread-local callback list that's updated when a thread calls CreateSteamPipe(). Naive implementations that rely
			 *	on the automatic registration in the constructor causes undefined behavior at program shutdown, since they will have been added to a list
			 *	that outlives the SteamworksWorldComponent that registered them-- Valve's shutdown code then stomps all over previously-freed memory. */
				_steamServersConnected.Register(this, &SteamworksWorldComponent::OnSteamServersConnected);
				_steamServersConnectFailure.Register(this, &SteamworksWorldComponent::OnSteamServersConnectFailure);
				_steamServersDisconnected.Register(this, &SteamworksWorldComponent::OnSteamServersDisconnected);
				_peerToPeerSessionRequestInitiated.Register(this, &SteamworksWorldComponent::OnP2PSessionRequestInitiated);
				_peerToPeerSessionConnectFail.Register(this, &SteamworksWorldComponent::OnP2PSessionConnectionFail);
				_policyResponse.Register(this, &SteamworksWorldComponent::OnPolicyResponse);
				_gameServerAuthTicketResponse.Register(this, &SteamworksWorldComponent::OnValidateAuthTicketResponse);
				_clientKickRequestRecieved.Register(this, &SteamworksWorldComponent::OnClientKickRequestReceived);

				ISteamGameServer* const server(GetSteamGameServer(serverUser, pipe));
				if (!server->InitGameServer(AnyInboundIp, _gamePort, _queryPort, 0u, SteamUtils()->GetAppID(), "0.0.0.0")) {
					_log.Write(MessageType::Error, "Unable to bind Steam game server!" UTF8_NEWLINE);
					return false;
				}

				server->LogOnAnonymous();

				_pipe = pipe;
				_serverUser = serverUser;
				_connectedToSteam = true;

				return true;
			}

		// ---------------------------------------------------

			void SteamworksWorldComponent::DisconnectFromSteam() {
				if (!IsConnectedToSteam()) {
					return;
				}

				ISteamGameServer* const server(GetSteamGameServer(_serverUser, _pipe));

				if (server->BLoggedOn()) {
					server->LogOff();
				}

				SteamClient()->ReleaseUser(_pipe, _serverUser);
				SteamClient()->BReleaseSteamPipe(_pipe);

				_connectedToSteam = false;
			}

		// ---------------------------------------------------

			void SteamworksWorldComponent::OnSteamServersConnected(SteamServersConnected_t* /*message*/) {
				_log.Write(MessageType::Message, "Steam connection established." UTF8_NEWLINE);
			}

		// ---------------------------------------------------

			void SteamworksWorldComponent::OnSteamServersConnectFailure(SteamServerConnectFailure_t* /*message*/) {
				_log.Write(MessageType::Error, "Error connecting to Steam servers!" UTF8_NEWLINE);
			}

		// ---------------------------------------------------

			void SteamworksWorldComponent::OnSteamServersDisconnected(SteamServersDisconnected_t* /*message*/) {
				_log.Write(MessageType::Message, "Disconnected from Steam servers." UTF8_NEWLINE);

				DisconnectFromSteam();
			}

		// ---------------------------------------------------

			void SteamworksWorldComponent::OnP2PSessionRequestInitiated(P2PSessionRequest_t* message) {
				ISteamNetworking* const networking(GetSteamNetworking(_serverUser, _pipe));

				if (!networking->AcceptP2PSessionWithUser(message->m_steamIDRemote)) {
					return;
				}

				_log.Write(MessageType::Message, "Accepted peer-to-peer connection to SteamID '{}'." UTF8_NEWLINE, message->m_steamIDRemote.ConvertToUint64());
			}

		// ---------------------------------------------------

			void SteamworksWorldComponent::OnP2PSessionConnectionFail(P2PSessionConnectFail_t* message) {
				_log.Write(MessageType::Warning, "Connection to SteamID '{}' dropped: {}." UTF8_NEWLINE, message->m_steamIDRemote.ConvertToUint64(), GetErrorString(static_cast<EP2PSessionError>(message->m_eP2PSessionError)));
			}

		// ---------------------------------------------------

			void SteamworksWorldComponent::OnPolicyResponse(GSPolicyResponse_t*) {}

		// ---------------------------------------------------

			void SteamworksWorldComponent::OnValidateAuthTicketResponse(ValidateAuthTicketResponse_t* /*pParam*/) {}

		// ---------------------------------------------------

			void SteamworksWorldComponent::OnClientKickRequestReceived(GSClientKick_t* kickedClient) {
				ISteamNetworking* const networking(GetSteamNetworking(_serverUser, _pipe));
				ISteamGameServer* const server(GetSteamGameServer(_serverUser, _pipe));

			//	EDenyReason m_eDenyReason;
				_log.Write(MessageType::Message, "Kicking user '{}'." UTF8_NEWLINE, kickedClient->m_SteamID.ConvertToUint64());

				networking->CloseP2PSessionWithUser(kickedClient->m_SteamID);
				server->EndAuthSession(kickedClient->m_SteamID);
			}

		}	// namespace Steamworks
	}	// namespace Networking
}	// namespace Eldritch2