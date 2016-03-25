/*==================================================================*\
  WorldView.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Foundation/WorldView.hpp>
//------------------------------------------------------------------//
#if( ET_COMPILER_IS_MSVC )
//	Valve has a few mismatches in their printf specifiers, it seems! We can't fix these, so disable the warning.
#	pragma warning( push )
#	pragma warning( disable : 6340 )
#endif
#include <steam_gameserver.h>
#if( ET_COMPILER_IS_MSVC )
#	pragma warning( pop )
#endif
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Networking {
		namespace Steamworks {
			class	WorldView;
		}
	}
}

namespace Eldritch2 {
namespace Networking {
namespace Steamworks {

	class WorldView : public Foundation::WorldView {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref WorldView instance.
		WorldView( Steamworks::EngineService& networkingService, Foundation::World& owningWorld );

		//!	Destroys this @ref WorldView instance.
		~WorldView();

	// ---------------------------------------------------

		static void	ExposeScriptApi( Scripting::ScriptApiRegistrationInitializationVisitor& visitor );

	// ---------------------------------------------------

	protected:
		void	OnPreScriptTick( Scheduler::WorkerContext& executingContext ) override sealed;

		void	OnPostScriptTick( Scheduler::WorkerContext& executingContext ) override sealed;

	// ---------------------------------------------------

		::Eldritch2::ErrorCode	ConnectToSteam( ::ISteamClient* const client );

	// - DATA MEMBERS ------------------------------------

	private:
		STEAM_GAMESERVER_CALLBACK_MANUAL( WorldView, OnSteamServersConnected,		::SteamServersConnected_t,		_steamServersConnected );
		STEAM_GAMESERVER_CALLBACK_MANUAL( WorldView, OnSteamServersConnectFailure,	::SteamServerConnectFailure_t,	_steamServersConnectFailure );
		STEAM_GAMESERVER_CALLBACK_MANUAL( WorldView, OnSteamServersDisconnected,	::SteamServersDisconnected_t,	_steamServersDisconnected );
		STEAM_GAMESERVER_CALLBACK_MANUAL( WorldView, OnP2PSessionRequest,			::P2PSessionRequest_t,			_peerToPeerSessionRequest );
		STEAM_GAMESERVER_CALLBACK_MANUAL( WorldView, OnP2PSessionConnectionFail,	::P2PSessionConnectFail_t,		_peerToPeerSessionConnectFail );
		STEAM_GAMESERVER_CALLBACK_MANUAL( WorldView, OnPolicyResponse,				::GSPolicyResponse_t,			_policyResponse );
		STEAM_GAMESERVER_CALLBACK_MANUAL( WorldView, OnValidateAuthTicketResponse,	::ValidateAuthTicketResponse_t, _gameServerAuthTicketResponse );
		STEAM_GAMESERVER_CALLBACK_MANUAL( WorldView, OnClientGameServerDeny,		::ClientGameServerDeny_t,		_clientGameServerDeny );
		STEAM_GAMESERVER_CALLBACK_MANUAL( WorldView, OnClientGameServerKick,		::GSClientKick_t,				_clientGameServerKick );

		Steamworks::EngineService&	_networkingService;

		::CSteamID					_hostID;

		bool						_ownsPipeAndHandle;
		::HSteamPipe				_steamPipe;
		::HSteamUser				_userHandle;

		::ISteamGameServer*			_gameServer;
		::ISteamUtils*				_serverUtils;
		::ISteamNetworking*			_serverNetworking;
		::ISteamGameServerStats*	_serverStats;
		::ISteamHTTP*				_serverHTTP;
		::ISteamInventory*			_inventoryService;
		::ISteamUGC*				_userGeneratedContent;
	};

}	// namespace Steamworks
}	// namespace Foundation
}	// namespace Eldritch2