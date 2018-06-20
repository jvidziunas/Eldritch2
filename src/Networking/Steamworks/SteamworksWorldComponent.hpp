/*==================================================================*\
  SteamworksWorldComponent.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Core/WorldComponent.hpp>
#include <Logging/ChildLog.hpp>
//------------------------------------------------------------------//
ET_PUSH_COMPILER_WARNING_STATE()
//	(6340) Valve has a few mismatches in their printf specifiers, it seems! We can't fix these, so disable the warning.
ET_SET_MSVC_WARNING_STATE(disable : 6340)
#include <steam_gameserver.h>
ET_POP_COMPILER_WARNING_STATE()
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Networking { namespace Steamworks {

	class SteamworksWorldComponent : public Core::WorldComponent {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		SteamworksWorldComponent(const SteamworksWorldComponent&) = delete;
		//!	Constructs this @ref SteamworksWorldComponent instance.
		SteamworksWorldComponent(const Core::World& owner);

		~SteamworksWorldComponent();

		// ---------------------------------------------------

	protected:
		void AcceptVisitor(Scheduling::JobExecutor& executor, const LateInitializationVisitor) override;
		void AcceptVisitor(Scheduling::JobExecutor& executor, const VariableTickVisitor&) override;
		void AcceptVisitor(Scheduling::JobExecutor& executor, const LateTickVisitor&) override;
		void AcceptVisitor(Scheduling::JobExecutor& executor, const TearDownVisitor) override;
		void AcceptVisitor(Scripting::Wren::ApiBuilder& api) override;

		// ---------------------------------------------------

	private:
		bool IsConnectedToSteam() const;

		bool ConnectToSteam();

		void DisconnectFromSteam();

		// ---------------------------------------------------

		//!	Disable copy assignment.
		SteamworksWorldComponent& operator=(const SteamworksWorldComponent&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		//!	Mutable so logs can be written even if we only have a const reference to the @ref SteamworksWorldComponent.
		mutable Logging::ChildLog _log;
		bool                      _connectedToSteam;
		HSteamPipe                _pipe;
		HSteamUser                _serverUser;
		uint16                    _gamePort;
		uint16                    _queryPort;

		STEAM_GAMESERVER_CALLBACK_MANUAL(SteamworksWorldComponent, OnSteamServersConnected, SteamServersConnected_t, _steamServersConnected);
		STEAM_GAMESERVER_CALLBACK_MANUAL(SteamworksWorldComponent, OnSteamServersConnectFailure, SteamServerConnectFailure_t, _steamServersConnectFailure);
		STEAM_GAMESERVER_CALLBACK_MANUAL(SteamworksWorldComponent, OnSteamServersDisconnected, SteamServersDisconnected_t, _steamServersDisconnected);
		STEAM_GAMESERVER_CALLBACK_MANUAL(SteamworksWorldComponent, OnP2PSessionRequestInitiated, P2PSessionRequest_t, _peerToPeerSessionRequestInitiated);
		STEAM_GAMESERVER_CALLBACK_MANUAL(SteamworksWorldComponent, OnP2PSessionConnectionFail, P2PSessionConnectFail_t, _peerToPeerSessionConnectFail);
		STEAM_GAMESERVER_CALLBACK_MANUAL(SteamworksWorldComponent, OnPolicyResponse, GSPolicyResponse_t, _policyResponse);
		STEAM_GAMESERVER_CALLBACK_MANUAL(SteamworksWorldComponent, OnValidateAuthTicketResponse, ValidateAuthTicketResponse_t, _gameServerAuthTicketResponse);
		STEAM_GAMESERVER_CALLBACK_MANUAL(SteamworksWorldComponent, OnClientKickRequestReceived, GSClientKick_t, _clientKickRequestRecieved);
	};

}}} // namespace Eldritch2::Networking::Steamworks
