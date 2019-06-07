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
#include <Networking/Steamworks/SteamTools.hpp>
#include <Core/EngineComponent.hpp>
#include <Core/WorldComponent.hpp>
//------------------------------------------------------------------//
//	(6340) Valve has a few mismatches in their printf specifiers; we can't fix these, so disable the warning.
ET_PUSH_MSVC_WARNING_STATE(disable : 6340)
#include <steam_gameserver.h>
ET_POP_MSVC_WARNING_STATE()
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Networking { namespace Steamworks {

	enum class NetworkPort : uint16 {
		Unspecified      = 0u,
		DefaultSteamPort = 6690u
	};

	// ---

	class SteamworksEngineComponent : public Core::EngineComponent {
		// - TYPE PUBLISHING ---------------------------------

	public:
		class WorldComponent : public Core::WorldComponent {
			// - TYPE PUBLISHING ---------------------------------

		public:
			template <typename CallbackData>
			using SteamCallback = CCallbackManual<WorldComponent, CallbackData>;

			// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref WorldComponent instance.
			WorldComponent(const ObjectInjector& services) ETNoexceptHint;
			//!	Disable copy construction.
			WorldComponent(const WorldComponent&) = delete;

			~WorldComponent();

			// ---------------------------------------------------

		public:
			ETConstexpr bool IsConnectedToSteam() const ETNoexceptHint;

			// ---------------------------------------------------

		public:
			void OnVariableRateTick(Scheduling::JobExecutor& executor, MicrosecondTime tickDuration, float32 residualFraction) ETNoexceptHint override;

			void OnFixedRateTickLate(Scheduling::JobExecutor& executor, MicrosecondTime delta) ETNoexceptHint override;

			// ---------------------------------------------------

		public:
			void BindResources(Scheduling::JobExecutor& executor) ETNoexceptHint override;

			void FreeResources(Scheduling::JobExecutor& executor) ETNoexceptHint override;

			// ---------------------------------------------------

		public:
			void PublishApi(Scripting::Wren::ApiBuilder& api) override;

			using Core::WorldComponent::PublishApi;

			// ---------------------------------------------------

		private:
			Result ConnectToSteam(NetworkPort port);

			void DisconnectFromSteam();

			// ---------------------------------------------------

		private:
			void OnSteamConnection(SteamServersConnected_t*) ETNoexceptHint;

			void OnSteamConnectionFailure(SteamServerConnectFailure_t*) ETNoexceptHint;

			void OnSteamDisconnection(SteamServersDisconnected_t*) ETNoexceptHint;

			void OnPeerToPeerSessionInitiation(P2PSessionRequest_t*) ETNoexceptHint;

			void OnPeerToPeerSessionFailure(P2PSessionConnectFail_t*) ETNoexceptHint;

			void OnServerPolicyResponse(GSPolicyResponse_t*) ETNoexceptHint;

			void OnAuthResponse(ValidateAuthTicketResponse_t*) ETNoexceptHint;

			void OnClientKick(GSClientKick_t*) ETNoexceptHint;

			// ---------------------------------------------------

			//!	Disable copy assignment.
			WorldComponent& operator=(const WorldComponent&) = delete;

			// - DATA MEMBERS ------------------------------------

		private:
			bool                                        _connectedToSteam;
			HSteamPipe                                  _pipe;
			HSteamUser                                  _serverUser;
			NetworkPort                                 _port;
			SteamCallback<SteamServersConnected_t>      _serversConnected;
			SteamCallback<SteamServerConnectFailure_t>  _serversFailedToConnect;
			SteamCallback<SteamServersDisconnected_t>   _serversDisconnected;
			SteamCallback<P2PSessionRequest_t>          _peerToPeerSessionInitiated;
			SteamCallback<P2PSessionConnectFail_t>      _peerToPeerSessionFailure;
			SteamCallback<GSPolicyResponse_t>           _serverPolicyResponse;
			SteamCallback<ValidateAuthTicketResponse_t> _authResponseReceived;
			SteamCallback<GSClientKick_t>               _clientKicked;
		};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		SteamworksEngineComponent(const SteamworksEngineComponent&) = delete;
		//! Constructs this @ref SteamworksEngineComponent instance.
		SteamworksEngineComponent(const ObjectInjector& services) ETNoexceptHint;

		~SteamworksEngineComponent();

		// - ENGINE SERVICE SANDBOX METHODS ------------------

	public:
		void TickEarly(Scheduling::JobExecutor& executor) ETNoexceptHint override;

		// ---------------------------------------------------

	public:
		void AddLocalUser();

		// - ENGINE SERVICE SANDBOX METHODS ------------------

	public:
		void BindConfigurableResources(Scheduling::JobExecutor& executor) ETNoexceptHint override;

		void PublishApi(Core::PropertyApiBuilder& api) override;

		using Core::EngineComponent::PublishApi;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		SteamworksEngineComponent& operator=(const SteamworksEngineComponent&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		NetworkPort            _steamPort;
		IdentifierPool<uint16> _ports;
		HashSet<CSteamID>      _bannedIds;
	};

}}} // namespace Eldritch2::Networking::Steamworks
