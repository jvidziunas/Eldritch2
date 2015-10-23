/*==================================================================*\
  EngineService.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Configuration/ConfigurationPublishingInitializationVisitor.hpp>
#include <Foundation/WorldViewFactoryPublishingInitializationVisitor.hpp>
#include <Networking/Steamworks/EngineService.hpp>
#include <Networking/Steamworks/WorldView.hpp>
#include <Scheduler/CRTPTransientTask.hpp>
#include <Utility/Memory/InstanceNew.hpp>
#include <Utility/Concurrency/Lock.hpp>
#include <Scheduler/TaskScheduler.hpp>
#include <Utility/DisposingResult.hpp>
#include <Foundation/GameEngine.hpp>
#include <Utility/Result.hpp>
#include <Build.hpp>
//------------------------------------------------------------------//

//==================================================================//
// LIBRARIES
//==================================================================//
#if( ET_PLATFORM_WINDOWS )
	ET_LINK_LIBRARY( "steam_api64.lib" )
#endif
//------------------------------------------------------------------//

using namespace ::Eldritch2::Configuration;
using namespace ::Eldritch2::Networking;
using namespace ::Eldritch2::Foundation;
using namespace ::Eldritch2::Scheduler;
using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Utility;
using namespace ::Eldritch2;
using namespace ::std;

namespace Eldritch2 {
namespace Networking {
namespace Steamworks {

	EngineService::EngineService( GameEngine& owningEngine ) : GameEngineService( owningEngine ),
															   _steamServersConnected( this, &EngineService::OnSteamServersConnected ),
															   _steamServersConnectFailure( this, &EngineService::OnSteamServersConnectFailure ),
															   _steamServersDisconnected( this, &EngineService::OnSteamServersDisconnected ),
															   _peerToPeerSessionRequest( this, &EngineService::OnP2PSessionRequest ),
															   _peerToPeerSessionConnectFail( this, &EngineService::OnP2PSessionConnectionFail ),
															   _policyResponse( this, &EngineService::OnPolicyResponse ),
															   _gameServerAuthTicketResponse( this, &EngineService::OnValidateAuthTicketResponse ),
															   _clientGameServerDeny( this, &EngineService::OnClientGameServerDeny ),
															   _clientGameServerKick( this, &EngineService::OnClientGameServerKick ),
															   _allocator( GetEngineAllocator(), UTF8L("Steamworks Networking Service Root Allocator") ),
															   _networkMutex( owningEngine.GetTaskScheduler().AllocateReaderWriterUserMutex( _allocator ).object ),
															   _worldIDCounter( 1u ),
															   _steamPort( 6690u ),
															   _gamePort( 6691u ),
															   _queryPort( 6692u ),
															   _versionString( UTF8_VERSION_STRING, _allocator ),
															   _lobbyWorldName( UTF8_PROJECT_NAME UTF8L("Lobby"), _allocator ),
															   _useVACAuthentication( true ),
															   _useSteamworks( false ),
															   _replicationUploadBandwidthLimitInKilobytesPerSecond( 1024u ),
															   _replicationDownloadBandwidthLimitInKilobytesPerSecond( 3u * 1024u ),
															   _contentDownloadBandwidthLimitInKilobytesPerSecond( 3u * 1024u ),
															   _contentUploadBandwidthLimitInKilobytesPerSecond( 1024u ),
															   _banList( { _allocator, UTF8L("Steamworks Ban List Allocator") } ),
															   _playerDirectory( { _allocator, UTF8L("Steamworks Player Directory Allocator") } ) {}

// ---------------------------------------------------

	EngineService::~EngineService() {
		_playerDirectory.Clear();

		if( ::SteamGameServer() ) {
			if( ::SteamGameServer()->BLoggedOn() ) {
				::SteamGameServer()->LogOff();
			}

			::SteamGameServer()->EnableHeartbeats( false );
			::SteamGameServer_Shutdown();
		}
		
		::SteamAPI_Shutdown();

		if( _networkMutex ) {
			_allocator.Delete( *_networkMutex, ::Eldritch2::AlignedDeallocationSemantics );
		}
	}

// ---------------------------------------------------

	const UTF8Char* const EngineService::GetName() const { 
		return UTF8L("Steamworks Networking Service");
	}

// ---------------------------------------------------

	bool EngineService::TryRecievePacket( const int channelID, ::CSteamID& senderID, char (&packetBuffer)[NETWORK_MTU_SIZE] ) {
		ScopedLock	_( *_networkMutex );

		return ::SteamGameServerNetworking()->ReadP2PPacket( packetBuffer, static_cast<uint32>(sizeof(packetBuffer)), nullptr, &senderID, channelID );
	}

// ---------------------------------------------------

	void EngineService::TrySendPacket( const int channelID, const ::CSteamID destinationID, void* const packetBuffer, const size_t packetSizeInBytes ) {
		::SteamNetworking()->SendP2PPacket( destinationID, packetBuffer, static_cast<uint32>(packetSizeInBytes), k_EP2PSendUnreliableNoDelay, channelID );
	}

// ---------------------------------------------------

	void EngineService::CloseReplicationOnChannel( const int channelID ) {
		ScopedLock	_( *_networkMutex );

		// ::SteamGameServerNetworking()->CloseP2PChannelWithUser( channelID );
	}

// ---------------------------------------------------

	void EngineService::AcceptInitializationVisitor( ConfigurationPublishingInitializationVisitor& visitor ) {
		visitor.PushSection( UTF8L("Steamworks") );

		visitor.Register( UTF8L("SteamPort"), _steamPort ).Register( UTF8L("GamePort"), _gamePort ).Register( UTF8L("QueryPort"), _queryPort ).Register( UTF8L("VersionString"), _versionString );
		visitor.Register( UTF8L("UseValveAntiCheatAuthentication"), _useVACAuthentication ).Register( UTF8L("UseSteamworks"), _useSteamworks );
		visitor.Register( UTF8L("ReplicationUploadBandwidthLimitInKilobytesPerSecond"), _replicationUploadBandwidthLimitInKilobytesPerSecond );
		visitor.Register( UTF8L("ReplicationDownloadBandwidthLimitInKilobytesPerSecond"), _replicationDownloadBandwidthLimitInKilobytesPerSecond );
		visitor.Register( UTF8L("ContentDownloadBandwidthLimitInKilobytesPerSecond"), _contentDownloadBandwidthLimitInKilobytesPerSecond );
		visitor.Register( UTF8L("ContentUploadBandwidthLimitInKilobytesPerSecond"), _contentUploadBandwidthLimitInKilobytesPerSecond );

		visitor.PushSection( UTF8_PROJECT_NAME );
		visitor.Register( UTF8L("LobbyWorldName"), _lobbyWorldName );
	}

// ---------------------------------------------------

	void EngineService::AcceptInitializationVisitor( ScriptAPIRegistrationInitializationVisitor& /*visitor*/ ) {}

// ---------------------------------------------------

	void EngineService::AcceptInitializationVisitor( WorldViewFactoryPublishingInitializationVisitor& visitor ) {
		visitor.PublishFactory( this, sizeof(WorldView), [] ( Allocator& allocator, World& world, void* parameter ) -> ErrorCode {
			auto&	networkingService( *static_cast<EngineService*>(parameter) );
			auto	networkID( networkingService._worldIDCounter.fetch_add( 1u, memory_order_acquire ) );

			return new(allocator, Allocator::AllocationOption::PERMANENT_ALLOCATION) WorldView( networkingService, networkID, world ) ? Error::NONE : Error::OUT_OF_MEMORY;
		} );
	}

// ---------------------------------------------------

	void EngineService::AcceptInitializationVisitor( const PostInitializationVisitor ) {
		auto	createPlayerResult( AcknowledgePlayerConnection( NetworkID( ::SteamUser()->GetSteamID(), 0u ) ) );
		auto	createLobbyWorldResult( _lobbyWorldName ? CreateWorld( _lobbyWorldName.GetCharacterArray() ) : CreateEditorWorld() );

		if( createPlayerResult && createLobbyWorldResult ) {
			_lobbyWorld = move( createLobbyWorldResult.object );
		}
	}

// ---------------------------------------------------

	void EngineService::AcceptTaskVisitor( Allocator& subtaskAllocator, Task& postConfigurationLoadInitializationTask, WorkerContext& executingContext, const PostConfigurationLoadedTaskVisitor ) {
		class InitializeSteamworksTask : public CRTPTransientTask<InitializeSteamworksTask> {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------
		public:
			//!	Constructs this @ref InitializeSteamworksTask instance.
			ETInlineHint InitializeSteamworksTask( EngineService& host, Task& postConfigurationLoadInitializationTask, WorkerContext& executingContext ) : CRTPTransientTask<InitializeSteamworksTask>( postConfigurationLoadInitializationTask, Scheduler::CodependentTaskSemantics ),
																																										 _host( host ) {
				TrySchedulingOnContext( executingContext );
			}

		// ---------------------------------------------------

			const UTF8Char* const GetHumanReadableName() const override sealed {
				return UTF8L("Initialize Steamworks Task");
			}

		// ---------------------------------------------------

			Task* Execute( WorkerContext& /*executingContext*/ ) override sealed {
				_host.InitiateSteamConnection();
				return nullptr;
			}

		// - DATA MEMBERS ------------------------------------

		private:
			EngineService&	_host;
		};

	// ---

		new(subtaskAllocator, Allocator::AllocationOption::TEMPORARY_ALLOCATION) InitializeSteamworksTask( *this, postConfigurationLoadInitializationTask, executingContext );
	}

// ---------------------------------------------------

	void EngineService::AcceptTaskVisitor( Allocator& subtaskAllocator, Task& serviceTickTask, WorkerContext& executingContext, const ServiceTickTaskVisitor ) {
		class ProcessCallbacksTask : public CRTPTransientTask<ProcessCallbacksTask> {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref ProcessCallbacksTask instance.
			ETInlineHint ProcessCallbacksTask( EngineService& host, Task& serviceTickTask, WorkerContext& executingContext ) : CRTPTransientTask<ProcessCallbacksTask>( serviceTickTask, Scheduler::CodependentTaskSemantics ),
																																			 _host( host ) {
				TrySchedulingOnContext( executingContext );
			}

		// ---------------------------------------------------

			const UTF8Char* const GetHumanReadableName() const override sealed {
				return UTF8L("Steamworks Callback Processing Task");
			}

			Task* Execute( WorkerContext& /*executingContext*/ ) override sealed {
				_host.ProcessAndDispatchCallbacks();
				return nullptr;
			}

		// - DATA MEMBERS ------------------------------------

		private:
			EngineService&	_host;
		};

	// ---

		new(subtaskAllocator, Allocator::AllocationOption::TEMPORARY_ALLOCATION) ProcessCallbacksTask( *this, serviceTickTask, executingContext );
	}

// ---------------------------------------------------

	void EngineService::InitiateSteamConnection() {
		GetLogger()( UTF8L("Connecting to Steam instance.") ET_UTF8_NEWLINE_LITERAL );

		const ::EServerMode	serverMode( _useVACAuthentication ? ::eServerModeAuthenticationAndSecure : ::eServerModeAuthentication );
		const uint32		serverAddress( 0u ); // INADDR_ANY

		// Attempt to establish a connection with the Steam master servers.
		if( ::SteamAPI_Init() ) {
			if( _useSteamworks && ::SteamGameServer_Init( serverAddress, _steamPort, _gamePort, _queryPort, serverMode, _versionString.GetCharacterArray() ) ) {
				::SteamGameServer()->SetProduct( UTF8_PROJECT_NAME );
				::SteamGameServer()->SetGameDescription( UTF8_PROJECT_NAME );
			}

			GetLogger()( UTF8L("Initial Steam connection established.") ET_UTF8_NEWLINE_LITERAL );
		} else {
			GetLogger( LogMessageType::ERROR )( UTF8L("Unable to initialize Steam API!") ET_UTF8_NEWLINE_LITERAL );
		}
	}

// ---------------------------------------------------

	Result<EngineService::Player> EngineService::AcknowledgePlayerConnection( const NetworkID& networkID ) {
		const auto	candidate( _playerDirectory.Find( networkID ) );

		if( candidate == _playerDirectory.End() ) {
			GetLogger()( UTF8L("Player {%llu:%i} connected.") ET_UTF8_NEWLINE_LITERAL, networkID.first.ConvertToUint64(), networkID.second );

			if( auto* const player = new(_allocator, Allocator::AllocationOption::PERMANENT_ALLOCATION) Player( networkID, *this, _allocator ) ) {
				_playerDirectory.Insert( { networkID, ObjectHandle<Player>( *player, ::Eldritch2::PassthroughReferenceCountingSemantics ) } );
				
				return { *player };
			}

			return { Error::OUT_OF_MEMORY };
		} else {
			GetLogger( LogMessageType::WARNING )( UTF8L("Received duplicate player join notification for player {%llu:%i}!)") ET_UTF8_NEWLINE_LITERAL, networkID.first.ConvertToUint64(), networkID.second );
			
			return { *candidate->second };
		}
	}

// ---------------------------------------------------

	void EngineService::AcknowledgePlayerDisconnection( const NetworkID& networkID ) {
		_playerDirectory.Erase( networkID );
	}

// ---------------------------------------------------

	void EngineService::AcknowledgeClientDisconnection( const ::CSteamID clientID ) {
		using PlayerDirectory = decltype(_playerDirectory);

	// ---

		_playerDirectory.Erase( _playerDirectory.RemoveIf( [clientID] ( const PlayerDirectory::ValueType& client ) { return client.first.first == clientID; } ), _playerDirectory.End() );
	}

// ---------------------------------------------------

	ErrorCode EngineService::ProcessAndDispatchCallbacks() {
		enum : int {
			REPLICATION_CHANNEL	= 0
		};

	// ---

		::SteamAPI_RunCallbacks();

		if( nullptr != ::SteamGameServerNetworking() ) {
			::CSteamID	senderID;
			char		packetBuffer[NETWORK_MTU_SIZE];

			::SteamGameServer_RunCallbacks();

			// Read all player update data
			while( TryRecievePacket( REPLICATION_CHANNEL, senderID, packetBuffer ) ) {

			}
		}

		return Error::NONE;
	}

// ---------------------------------------------------

	void EngineService::OnSteamServersConnected( ::SteamServersConnected_t* /*connectionParameters*/ ) {
		GetLogger()( UTF8L("Steam connection re-established.") ET_UTF8_NEWLINE_LITERAL );
	}

// ---------------------------------------------------

	void EngineService::OnSteamServersConnectFailure( ::SteamServerConnectFailure_t* /*connectionFailureParameters*/ ) {}

// ---------------------------------------------------

	void EngineService::OnSteamServersDisconnected( ::SteamServersDisconnected_t* /*disconnectionParameters*/ ) {
		GetLogger( LogMessageType::ERROR )( UTF8L("Disconnected from Steam servers!") ET_UTF8_NEWLINE_LITERAL );
	}

// ---------------------------------------------------

	void EngineService::OnP2PSessionRequest( ::P2PSessionRequest_t* requestedConnection ) {
		const ::CSteamID	accountID( requestedConnection->m_steamIDRemote );

		if( !_banList.Contains( accountID ) ) {
			::SteamGameServerNetworking()->AcceptP2PSessionWithUser( accountID );
		}
	}

// ---------------------------------------------------

	void EngineService::OnP2PSessionConnectionFail( ::P2PSessionConnectFail_t* failedConnection ) {
		static const UTF8Char* const	disconnectionReasonStrings[::EP2PSessionError::k_EP2PSessionErrorMax] = {
			UTF8L( "" ),
			UTF8L( ": Player not running same application" ),
			UTF8L( ": Local player does not have permission to run the application" ),
			UTF8L( ": Remote player not logged in to Steam" ),
			UTF8L( ": Connection timeout/blocked" )
		};
		
		GetLogger( LogMessageType::ERROR )( UTF8L("Dropped connection to client %llu%s.") ET_UTF8_NEWLINE_LITERAL, failedConnection->m_steamIDRemote.ConvertToUint64(), disconnectionReasonStrings[failedConnection->m_eP2PSessionError] );

		AcknowledgeClientDisconnection( failedConnection->m_steamIDRemote );
	}

// ---------------------------------------------------

	void EngineService::OnPolicyResponse( ::GSPolicyResponse_t* ) {}

// ---------------------------------------------------

	void EngineService::OnValidateAuthTicketResponse( ::ValidateAuthTicketResponse_t* /*pParam*/ ) {}

// ---------------------------------------------------

	void EngineService::OnClientGameServerDeny( ::ClientGameServerDeny_t* ) {}

// ---------------------------------------------------

	void EngineService::OnClientGameServerKick( ::GSClientKick_t* kickedClient ) {
		// EDenyReason m_eDenyReason;
		AcknowledgeClientDisconnection( kickedClient->m_SteamID );
	}

}	// namespace Steamworks
}	// namespace Networking
}	// namespace Eldritch2