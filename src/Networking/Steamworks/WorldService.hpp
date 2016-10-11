/*==================================================================*\
  WorldService.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Core/WorldService.hpp>
#include <Logging/ChildLog.hpp>
//------------------------------------------------------------------//
#if( ET_COMPILER_IS_MSVC )
//	Valve has a few mismatches in their printf specifiers, it seems! We can't fix these, so disable the warning.
#	pragma warning( push )
#	pragma warning( disable : 6340 )
#endif
#include <isteamclient.h>
#include <steam_gameserver.h>
#if( ET_COMPILER_IS_MSVC )
#	pragma warning( pop )
#endif
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Networking {
namespace Steamworks {

	class WorldService : public Core::WorldService {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref WorldService instance.
		WorldService( HSteamPipe pipe, HSteamUser user, const Core::World& world );
	//!	Disable copying.
		WorldService( const WorldService& ) = delete;

	//!	Destroys this @ref WorldService instance.
		~WorldService();

	// ---------------------------------------------------

	public:
		Eldritch2::Utf8Literal	GetName() const override;

	// ---------------------------------------------------

	public:
		static void	RegisterScriptApi( Scripting::ApiRegistrar& registrar );

	// ---------------------------------------------------

	public:
		void	OnScriptPreTick( Scheduling::JobFiber& executor ) override;

		void	OnPostScriptTick( Scheduling::JobFiber& executor ) override;

	// ---------------------------------------------------

	protected:
		void	AcceptVisitor( Scheduling::JobFiber& executor, const GameStartVisitor ) override;
		void	AcceptVisitor( Scheduling::JobFiber& executor, const GameStopVisitor ) override;

	// ---------------------------------------------------

	//!	Disable assignment.
		WorldService&	operator=( const WorldService& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
	//!	Mutable so logs can be written even if we only have a const reference to the @ref WorldService.
		mutable Logging::ChildLog	_log;

		HSteamPipe					_pipe;
		HSteamUser					_user;

		Eldritch2::uint16			_gamePort;
		Eldritch2::uint16			_queryPort;

		ISteamNetworking*			_networking;
		ISteamGameServer*			_gameServer;

	//	Steam callbacks. These will be invoked during fixed ticks as a part of OnPreScriptTick() due to the details of how Steam dispatches callbacks.
		STEAM_GAMESERVER_CALLBACK( WorldService, OnSteamServersConnected,		SteamServersConnected_t,		_steamServersConnected );
		STEAM_GAMESERVER_CALLBACK( WorldService, OnSteamServersConnectFailure,	SteamServerConnectFailure_t,	_steamServersConnectFailure );
		STEAM_GAMESERVER_CALLBACK( WorldService, OnSteamServersDisconnected,	SteamServersDisconnected_t,		_steamServersDisconnected );
		STEAM_GAMESERVER_CALLBACK( WorldService, OnP2PSessionRequestInitiated,	P2PSessionRequest_t,			_peerToPeerSessionRequestInitiated );
		STEAM_GAMESERVER_CALLBACK( WorldService, OnP2PSessionConnectionFail,	P2PSessionConnectFail_t,		_peerToPeerSessionConnectFail );
		STEAM_GAMESERVER_CALLBACK( WorldService, OnPolicyResponse,				GSPolicyResponse_t,				_policyResponse );
		STEAM_GAMESERVER_CALLBACK( WorldService, OnValidateAuthTicketResponse,	ValidateAuthTicketResponse_t,	_gameServerAuthTicketResponse );
		STEAM_GAMESERVER_CALLBACK( WorldService, OnClientKickRequestReceived,	GSClientKick_t,					_clientKickRequestRecieved );
	};

}	// namespace Steamworks
}	// namespace Core
}	// namespace Eldritch2