/*==================================================================*\
  WorldService.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Networking/Steamworks/EngineService.hpp>
#include <Networking/Steamworks/WorldService.hpp>
#include <Utility/Memory/InstanceNew.hpp>
#include <Utility/ErrorCode.hpp>
#include <Core/World.hpp>
#include <Build.hpp>
//------------------------------------------------------------------//
#include <microprofile/microprofile.h>
#if( ET_COMPILER_IS_MSVC )
//	Valve has a few mismatches in their printf specifiers, it seems! We can't fix these, so disable the warning.
#	pragma warning( push )
#	pragma warning( disable : 6340 )
#endif
#include <steam_api.h>
#if( ET_COMPILER_IS_MSVC )
#	pragma warning( pop )
#endif
//------------------------------------------------------------------//

//==================================================================//
// LIBRARIES
//==================================================================//
#if( ET_PLATFORM_WINDOWS )
	ET_LINK_LIBRARY( "steam_api64.lib" )
#endif
//------------------------------------------------------------------//

using namespace ::Eldritch2::Scheduling;
using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Logging;
using namespace ::Eldritch2::Core;

namespace Eldritch2 {
namespace Networking {
namespace Steamworks {

	WorldService::WorldService( HSteamPipe pipe, HSteamUser user, const World& world ) : Core::WorldService( world.GetServiceBlackboard() ),
																						 _log( world.GetLog() ),
																						 _pipe( pipe ),
																						 _user( user ),
																						 _gamePort( 0u ),
																						 _queryPort( 0u ),
																						 _gameServer( SteamClient()->GetISteamGameServer( user, pipe, STEAMGAMESERVER_INTERFACE_VERSION ) ),
																						 _networking( SteamClient()->GetISteamNetworking( user, pipe, STEAMNETWORKING_INTERFACE_VERSION ) ),
																						 _steamServersConnected( this, &WorldService::OnSteamServersConnected ),
																						 _steamServersConnectFailure( this, &WorldService::OnSteamServersConnectFailure ),
																						 _steamServersDisconnected( this, &WorldService::OnSteamServersDisconnected ),
																						 _peerToPeerSessionRequestInitiated( this, &WorldService::OnP2PSessionRequestInitiated ),
																						 _peerToPeerSessionConnectFail( this, &WorldService::OnP2PSessionConnectionFail ),
																						 _policyResponse( this, &WorldService::OnPolicyResponse ),
																						 _gameServerAuthTicketResponse( this, &WorldService::OnValidateAuthTicketResponse ),
																						 _clientKickRequestRecieved( this, &WorldService::OnClientKickRequestReceived ) {}

// ---------------------------------------------------

	WorldService::~WorldService() {
		SteamClient()->ReleaseUser( _pipe, _user );
		SteamClient()->BReleaseSteamPipe( _pipe );
	}

// ---------------------------------------------------

	Utf8Literal WorldService::GetName() const {
		return "Steamworks World Service";
	}

// ---------------------------------------------------

	void WorldService::RegisterScriptApi( ApiRegistrar& /*registrar*/ ) {}

// ---------------------------------------------------

	void WorldService::AcceptVisitor( JobFiber& /*fiber*/, const GameStartVisitor ) {
		if( 0u == _gamePort || 0u == _queryPort ) {
			return;
		}

		if( !_gameServer->InitGameServer( 0u, _gamePort, _queryPort, 0u, SteamUtils()->GetAppID(), "0.0.0.0" ) ) {
			_log( MessageSeverity::Error, "Unable to initialize Steam game server interface!" ET_UTF8_NEWLINE_LITERAL );
			return;
		}

		_gameServer->LogOnAnonymous();
	}

// ---------------------------------------------------

	void WorldService::OnScriptPreTick( JobFiber& /*fiber*/ ) {
		MICROPROFILE_SCOPEI( "Steamworks Networking Service", "Pull remote state", 0xBBBBBB );

		Steam_RunCallbacks( _pipe, true );
		SteamAPI_ReleaseCurrentThreadMemory();
	}

// ---------------------------------------------------

	void WorldService::OnPostScriptTick( JobFiber& /*fiber*/ ) {
		MICROPROFILE_SCOPEI( "Steamworks Networking Service", "Push local state", 0xBBCCBB );

		SteamAPI_ReleaseCurrentThreadMemory();
	}

// ---------------------------------------------------

	void WorldService::AcceptVisitor( JobFiber& /*fiber*/, const GameStopVisitor ) {
		if( !_gameServer || !_gameServer->BLoggedOn() ) {
			return;
		}

		_gameServer->LogOff();
	}

// ---------------------------------------------------
	
	void WorldService::OnSteamServersConnected( SteamServersConnected_t* /*connectionParameters*/ ) {
		_log( MessageSeverity::Message, "Steam connection established." ET_UTF8_NEWLINE_LITERAL );
	}

// ---------------------------------------------------

	void WorldService::OnSteamServersConnectFailure( SteamServerConnectFailure_t* /*connectionFailureParameters*/ ) {
		_log( MessageSeverity::Error, "Error connecting to Steam servers!" ET_UTF8_NEWLINE_LITERAL );
	}

// ---------------------------------------------------

	void WorldService::OnSteamServersDisconnected( SteamServersDisconnected_t* /*disconnectionParameters*/ ) {
		_log( MessageSeverity::Message, "Disconnected from Steam servers." ET_UTF8_NEWLINE_LITERAL );
	}

// ---------------------------------------------------

	void WorldService::OnP2PSessionRequestInitiated( P2PSessionRequest_t* requestedConnection ) {
		if( !_networking->AcceptP2PSessionWithUser( requestedConnection->m_steamIDRemote ) ) {
			return;
		}

		_log( MessageSeverity::Message, "Accepted peer-to-peer connection to SteamID '{}'." ET_UTF8_NEWLINE_LITERAL, requestedConnection->m_steamIDRemote.ConvertToUint64() );
	}

// ---------------------------------------------------

	void WorldService::OnP2PSessionConnectionFail( P2PSessionConnectFail_t* failedConnection ) {
		static const Utf8Char* const	disconnectionReasonStrings[EP2PSessionError::k_EP2PSessionErrorMax] = {
			"",
			": Player not running same application" ,
			": Local player does not have permission to run the application",
			": Remote player not logged in to Steam",
			": Connection timeout/blocked"
		};
		
		_log( MessageSeverity::Warning, "Dropped connection to SteamID '{}'{}." ET_UTF8_NEWLINE_LITERAL, failedConnection->m_steamIDRemote.ConvertToUint64(), disconnectionReasonStrings[failedConnection->m_eP2PSessionError] );
	}

// ---------------------------------------------------

	void WorldService::OnPolicyResponse( GSPolicyResponse_t* ) {}

// ---------------------------------------------------

	void WorldService::OnValidateAuthTicketResponse( ValidateAuthTicketResponse_t* /*pParam*/ ) {}

// ---------------------------------------------------

	void WorldService::OnClientKickRequestReceived( GSClientKick_t* kickedClient ) {
	//	EDenyReason m_eDenyReason;
		_log( MessageSeverity::Message, "Received kick request for remote user '{}'." ET_UTF8_NEWLINE_LITERAL, kickedClient->m_SteamID.ConvertToUint64() );

		_gameServer->EndAuthSession( kickedClient->m_SteamID );
		_networking->CloseP2PSessionWithUser( kickedClient->m_SteamID );
	}

}	// namespace Steamworks
}	// namespace Networking
}	// namespace Eldritch2