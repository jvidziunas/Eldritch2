/*==================================================================*\
  WorldView.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Networking/Steamworks/EngineService.hpp>
#include <Networking/Steamworks/WorldView.hpp>
#include <Scheduler/CRTPTransientTask.hpp>
#include <Utility/Memory/InstanceNew.hpp>
#include <Utility/Concurrency/Lock.hpp>
#include <Utility/ErrorCode.hpp>
#include <Build.hpp>
//------------------------------------------------------------------//
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

using namespace ::Eldritch2::Networking;
using namespace ::Eldritch2::Foundation;
using namespace ::Eldritch2::Scheduler;
using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Utility;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Networking {
namespace Steamworks {

	WorldView::WorldView( EngineService& networkingService, World& owningWorld ) : Foundation::WorldView( owningWorld ),
																				   _networkingService( networkingService ),
																				   _serverUtils( nullptr ),
																				   _gameServer( nullptr ),
																				   _serverNetworking( nullptr ),
																				   _serverStats( nullptr ),
																				   _serverHTTP( nullptr ),
																				   _inventoryService( nullptr ),
																				   _userGeneratedContent( nullptr ),
																				   _ownsPipeAndHandle( false ) {}

// ---------------------------------------------------

	WorldView::~WorldView() {
		if( _gameServer ) {
			_gameServer->EnableHeartbeats( false );

			_steamServersConnected.Unregister();
			_steamServersConnectFailure.Unregister();
			_steamServersDisconnected.Unregister();
			_peerToPeerSessionRequest.Unregister();
			_peerToPeerSessionConnectFail.Unregister();
			_policyResponse.Unregister();
			_gameServerAuthTicketResponse.Unregister();
			_clientGameServerDeny.Unregister();
			_clientGameServerKick.Unregister();

			if( _gameServer->BLoggedOn() ) {
				_gameServer->LogOff();
			}
		}

		if( _ownsPipeAndHandle ) {
			::SteamClient()->ReleaseUser( _steamPipe, _userHandle );
			::SteamClient()->BReleaseSteamPipe( _steamPipe );
		}
	}

// ---------------------------------------------------

	void WorldView::AcceptTaskVisitor( Allocator& subtaskAllocator, WorkerContext& executingContext, Task& visitingTask, const PreScriptTickTaskVisitor ) {
		class ReceivePacketsTask : public CRTPTransientTask<ReceivePacketsTask> {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref ReceivePacketsTask instance.
			ETInlineHint ReceivePacketsTask( WorldView& host, WorkerContext& executingContext, Task& preScriptTickTask ) : CRTPTransientTask<ReceivePacketsTask>( preScriptTickTask, Scheduler::CodependentTaskSemantics ), _host( host ) {
				TrySchedulingOnContext( executingContext );
			}

			~ReceivePacketsTask() = default;

		// ---------------------------------------------------

			const UTF8Char* const GetHumanReadableName() const override sealed {
				return UTF8L("Steamworks Receive Packets Task");
			}

			Task* Execute( WorkerContext& /*executingContext*/ ) override sealed {
				::CSteamID	senderID;
				char		receivedPacket[1200u];
				uint32		receivedSize( 0u );

				while( _host._serverNetworking->ReadP2PPacket( receivedPacket, sizeof(receivedPacket), &receivedSize, &senderID ) ) {

				}

				return nullptr;
			}

		// - DATA MEMBERS ------------------------------------

		private:
			WorldView&	_host;
		};

	// ---

		if( _serverNetworking ) {
			new(subtaskAllocator, Allocator::AllocationOption::TEMPORARY_ALLOCATION) ReceivePacketsTask( *this, executingContext, visitingTask );
		}
	}

// ---------------------------------------------------

	void WorldView::AcceptTaskVisitor( Allocator& subtaskAllocator, WorkerContext& executingContext, Task& visitingTask, const PostScriptTickTaskVisitor ) {
		class BroadcastPacketsTask : public CRTPTransientTask<BroadcastPacketsTask> {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref BroadcastPacketsTask instance.
			ETInlineHint BroadcastPacketsTask( WorldView& host, WorkerContext& executingContext, Task& postScriptTickTask ) : CRTPTransientTask<BroadcastPacketsTask>( postScriptTickTask, Scheduler::CodependentTaskSemantics ), _host( host ) {
				TrySchedulingOnContext( executingContext );
			}

			~BroadcastPacketsTask() = default;

		// ---------------------------------------------------

			const UTF8Char* const GetHumanReadableName() const override sealed {
				return UTF8L("Steamworks Broadcast Packets Task");
			}

			Task* Execute( WorkerContext& /*executingContext*/ ) override sealed {
				return nullptr;
			}

		// - DATA MEMBERS ------------------------------------

		private:
			WorldView&	_host;
		};

	// ---

		if( _serverNetworking ) {
			new(subtaskAllocator, Allocator::AllocationOption::TEMPORARY_ALLOCATION) BroadcastPacketsTask( *this, executingContext, visitingTask );
		}
	}

// ---------------------------------------------------

	ErrorCode WorldView::ConnectToSteam( ::ISteamClient* const steamClient ) {
		::HSteamPipe	pipe;
		::HSteamUser	serverUser( steamClient->CreateLocalUser( &pipe, EAccountType::k_EAccountTypeGameServer ) );

		_serverUtils = steamClient->GetISteamUtils( pipe, STEAMUTILS_INTERFACE_VERSION );

		if( const auto gameServer = steamClient->GetISteamGameServer( serverUser, pipe, STEAMGAMESERVER_INTERFACE_VERSION ) ) {
			gameServer->InitGameServer( 0u, 0u, 0u, 0u, _serverUtils->GetAppID(), "" );

			gameServer->SetProduct( UTF8_PROJECT_NAME );
			gameServer->SetGameDescription( UTF8_PROJECT_NAME );
			gameServer->SetModDir( "" );

			_steamServersConnected.Register( this, &WorldView::OnSteamServersConnected );
			_steamServersConnectFailure.Register( this, &WorldView::OnSteamServersConnectFailure );
			_steamServersDisconnected.Register( this, &WorldView::OnSteamServersDisconnected );
			_peerToPeerSessionRequest.Register( this, &WorldView::OnP2PSessionRequest );
			_peerToPeerSessionConnectFail.Register( this, &WorldView::OnP2PSessionConnectionFail );
			_policyResponse.Register( this, &WorldView::OnPolicyResponse );
			_gameServerAuthTicketResponse.Register( this, &WorldView::OnValidateAuthTicketResponse );
			_clientGameServerDeny.Register( this, &WorldView::OnClientGameServerDeny );
			_clientGameServerKick.Register( this, &WorldView::OnClientGameServerKick );

			_gameServer = gameServer;
		}
		
		_serverNetworking		= steamClient->GetISteamNetworking( serverUser, pipe, STEAMNETWORKING_INTERFACE_VERSION );
		_serverStats			= steamClient->GetISteamGameServerStats( serverUser, pipe, STEAMGAMESERVERSTATS_INTERFACE_VERSION );
		_serverHTTP				= steamClient->GetISteamHTTP( serverUser, pipe, STEAMHTTP_INTERFACE_VERSION );
		_inventoryService		= steamClient->GetISteamInventory( serverUser, pipe, STEAMINVENTORY_INTERFACE_VERSION );
		_userGeneratedContent	= steamClient->GetISteamUGC( serverUser, pipe, STEAMUGC_INTERFACE_VERSION );

		_steamPipe				= pipe;
		_userHandle				= serverUser;
		_ownsPipeAndHandle		= true;

		return Error::NONE;
	}

// ---------------------------------------------------

	void WorldView::OnSteamServersConnected( ::SteamServersConnected_t* /*connectionParameters*/ ) {
		// _owningWorld.GetLogger()( UTF8L("Steam connection re-established.") ET_UTF8_NEWLINE_LITERAL );
	}

// ---------------------------------------------------

	void WorldView::OnSteamServersConnectFailure( ::SteamServerConnectFailure_t* /*connectionFailureParameters*/ ) {}

// ---------------------------------------------------

	void WorldView::OnSteamServersDisconnected( ::SteamServersDisconnected_t* /*disconnectionParameters*/ ) {
		// _owningWorld.GetLogger( LogMessageType::ERROR )(UTF8L( "Disconnected from Steam servers!" ) ET_UTF8_NEWLINE_LITERAL);
	}

// ---------------------------------------------------

	void WorldView::OnP2PSessionRequest( ::P2PSessionRequest_t* requestedConnection ) {
		const ::CSteamID	accountID( requestedConnection->m_steamIDRemote );
		
		if( /*!_banList.Contains( accountID )*/ true ) {
			_serverNetworking->AcceptP2PSessionWithUser( accountID );
		}
	}

// ---------------------------------------------------

	void WorldView::OnP2PSessionConnectionFail( ::P2PSessionConnectFail_t* failedConnection ) {
		static const UTF8Char* const	disconnectionReasonStrings[::EP2PSessionError::k_EP2PSessionErrorMax] = {
			UTF8L( "" ),
			UTF8L( ": Player not running same application" ),
			UTF8L( ": Local player does not have permission to run the application" ),
			UTF8L( ": Remote player not logged in to Steam" ),
			UTF8L( ": Connection timeout/blocked" )
		};
		
		// GetLogger( LogMessageType::ERROR )( UTF8L("Dropped connection to client %llu%s.") ET_UTF8_NEWLINE_LITERAL, failedConnection->m_steamIDRemote.ConvertToUint64(), disconnectionReasonStrings[failedConnection->m_eP2PSessionError] );
	}

// ---------------------------------------------------

	void WorldView::OnPolicyResponse( ::GSPolicyResponse_t* ) {}

// ---------------------------------------------------

	void WorldView::OnValidateAuthTicketResponse( ::ValidateAuthTicketResponse_t* /*pParam*/ ) {}

// ---------------------------------------------------

	void WorldView::OnClientGameServerDeny( ::ClientGameServerDeny_t* ) {}

// ---------------------------------------------------

	void WorldView::OnClientGameServerKick( ::GSClientKick_t* /*kickedClient*/ ) {
		// EDenyReason m_eDenyReason;
	}

}	// namespace Steamworks
}	// namespace Networking
}	// namespace Eldritch2