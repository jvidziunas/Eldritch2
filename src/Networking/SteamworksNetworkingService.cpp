/*==================================================================*\
  SteamworksNetworkingService.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  �2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Configuration/ConfigurationPublishingInitializationVisitor.hpp>
#include <Foundation/WorldViewFactoryPublishingInitializationVisitor.hpp>
#include <Networking/Steamworks/SteamworksWorldView.hpp>
#include <Networking/SteamworksNetworkingService.hpp>
#include <Scheduler/CRTPTransientTask.hpp>
#include <Utility/DisposingResultPair.hpp>
#include <Utility/Memory/InstanceNew.hpp>
#include <Utility/Concurrency/Lock.hpp>
#include <Scheduler/TaskScheduler.hpp>
#include <Foundation/GameEngine.hpp>
#include <Utility/ResultPair.hpp>
#include <Utility/ErrorCode.hpp>
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

	SteamworksNetworkingService::SteamworksNetworkingService( GameEngine& owningEngine ) : GameEngineService( owningEngine ),
																						   _steamServersConnected( this, &SteamworksNetworkingService::OnSteamServersConnected ),
																						   _steamServersConnectFailure( this, &SteamworksNetworkingService::OnSteamServersConnectFailure ),
																						   _steamServersDisconnected( this, &SteamworksNetworkingService::OnSteamServersDisconnected ),
																						   _peerToPeerSessionRequest( this, &SteamworksNetworkingService::OnP2PSessionRequest ),
																						   _peerToPeerSessionConnectFail( this, &SteamworksNetworkingService::OnP2PSessionConnectionFail ),
																						   _policyResponse( this, &SteamworksNetworkingService::OnPolicyResponse ),
																						   _gameServerAuthTicketResponse( this, &SteamworksNetworkingService::OnValidateAuthTicketResponse ),
																						   _clientGameServerDeny( this, &SteamworksNetworkingService::OnClientGameServerDeny ),
																						   _clientGameServerKick( this, &SteamworksNetworkingService::OnClientGameServerKick ),
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
																						   _banList( 0u, ::rde::less<::CSteamID>(), _allocator, UTF8L("Steamworks Ban List Allocator") ),
																						   _playerDirectory( 0u, ::rde::less<NetworkID>(), _allocator, UTF8L("Steamworks Player Directory Allocator") ) {}

// ---------------------------------------------------

	SteamworksNetworkingService::~SteamworksNetworkingService() {
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

	const UTF8Char* const SteamworksNetworkingService::GetName() const { 
		return UTF8L("Steamworks Networking Service");
	}

// ---------------------------------------------------

	bool SteamworksNetworkingService::TryRecievePacket( const int channelID, ::CSteamID& senderID, char (&packetBuffer)[NETWORK_MTU_SIZE] ) {
		ScopedLock	_( _networkMutex );

		return ::SteamGameServerNetworking()->ReadP2PPacket( packetBuffer, static_cast<uint32>(sizeof(packetBuffer)), nullptr, &senderID, channelID );
	}

// ---------------------------------------------------

	void SteamworksNetworkingService::TrySendPacket( const int channelID, const ::CSteamID destinationID, void* const packetBuffer, const size_t packetSizeInBytes ) {
		::SteamNetworking()->SendP2PPacket( destinationID, packetBuffer, static_cast<uint32>(packetSizeInBytes), k_EP2PSendUnreliableNoDelay, channelID );
	}

// ---------------------------------------------------

	void SteamworksNetworkingService::CloseReplicationOnChannel( const int channelID ) {
		ScopedLock	_( _networkMutex );

		// ::SteamGameServerNetworking()->CloseP2PChannelWithUser( channelID );
	}

// ---------------------------------------------------

	void SteamworksNetworkingService::AcceptInitializationVisitor( ConfigurationPublishingInitializationVisitor& visitor ) {
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

	void SteamworksNetworkingService::AcceptInitializationVisitor( ScriptAPIRegistrationInitializationVisitor& /*visitor*/ ) {}

// ---------------------------------------------------

	void SteamworksNetworkingService::AcceptInitializationVisitor( WorldViewFactoryPublishingInitializationVisitor& visitor ) {
		visitor.PublishFactory( this, sizeof( SteamworksWorldView ), [] ( Allocator& allocator, World& world, void* parameter ) -> ErrorCode {
			auto&	networkingService( *static_cast<SteamworksNetworkingService*>(parameter) );
			auto	networkID( networkingService._worldIDCounter.fetch_add( 1u, memory_order_acquire ) );

			return new(allocator, Allocator::AllocationOption::PERMANENT_ALLOCATION) SteamworksWorldView( networkingService, networkID, world ) ? Errors::NONE : Errors::OUT_OF_MEMORY;
		} );
	}

// ---------------------------------------------------

	void SteamworksNetworkingService::AcceptInitializationVisitor( const PostInitializationVisitor ) {
		const NetworkID	localPlayerID( ::SteamUser()->GetSteamID(), 0u );

		if( auto createLobbyWorldResult = (_lobbyWorldName ? CreateWorld( _lobbyWorldName.GetCharacterArray() ) : CreateEditorWorld()) ) {
			if( const auto createPlayerResult = AcknowledgePlayerConnection( localPlayerID ) ) {
				_lobbyWorld = ::std::move( createLobbyWorldResult.object );
			}
		}
	}

// ---------------------------------------------------

	void SteamworksNetworkingService::AcceptTaskVisitor( Allocator& subtaskAllocator, Task& postConfigurationLoadInitializationTask, WorkerContext& executingContext, const PostConfigurationLoadedTaskVisitor ) {
		class InitializeSteamworksTask : public CRTPTransientTask<InitializeSteamworksTask> {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------
		public:
			// Constructs this InitializeSteamworksTask instance.
			ETInlineHint InitializeSteamworksTask( SteamworksNetworkingService& host, Task& postConfigurationLoadInitializationTask, WorkerContext& executingContext ) : CRTPTransientTask<InitializeSteamworksTask>( postConfigurationLoadInitializationTask, Scheduler::CodependentTaskSemantics ),
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
			SteamworksNetworkingService&	_host;
		};

	// ---

		new(subtaskAllocator, Allocator::AllocationOption::TEMPORARY_ALLOCATION) InitializeSteamworksTask( *this, postConfigurationLoadInitializationTask, executingContext );
	}

// ---------------------------------------------------

	void SteamworksNetworkingService::AcceptTaskVisitor( Allocator& subtaskAllocator, Task& serviceTickTask, WorkerContext& executingContext, const ServiceTickTaskVisitor ) {
		class ProcessCallbacksTask : public CRTPTransientTask<ProcessCallbacksTask> {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			// Constructs this ProcessCallbacksTask instance.
			ETInlineHint ProcessCallbacksTask( SteamworksNetworkingService& host, Task& serviceTickTask, WorkerContext& executingContext ) : CRTPTransientTask<ProcessCallbacksTask>( serviceTickTask, Scheduler::CodependentTaskSemantics ),
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
			SteamworksNetworkingService&	_host;
		};

	// ---

		new(subtaskAllocator, Allocator::AllocationOption::TEMPORARY_ALLOCATION) ProcessCallbacksTask( *this, serviceTickTask, executingContext );
	}

// ---------------------------------------------------

	void SteamworksNetworkingService::InitiateSteamConnection() {
		FormatAndLogString( UTF8L("Connecting to Steam instance.") ET_UTF8_NEWLINE_LITERAL );

		const ::EServerMode	serverMode( _useVACAuthentication ? ::eServerModeAuthenticationAndSecure : ::eServerModeAuthentication );
		const uint32		serverAddress( 0u ); // INADDR_ANY

		// Attempt to establish a connection with the Steam master servers.
		if( ::SteamAPI_Init() ) {
			if( _useSteamworks && ::SteamGameServer_Init( serverAddress, _steamPort, _gamePort, _queryPort, serverMode, _versionString.GetCharacterArray() ) ) {
				::SteamGameServer()->SetProduct( UTF8_PROJECT_NAME );
				::SteamGameServer()->SetGameDescription( UTF8_PROJECT_NAME );
			}

			FormatAndLogString( UTF8L("Initial Steam connection established.") ET_UTF8_NEWLINE_LITERAL );
		} else {
			FormatAndLogError( UTF8L("Unable to initialize Steam API!") ET_UTF8_NEWLINE_LITERAL );
		}
	}

// ---------------------------------------------------

	ResultPair<SteamworksNetworkingService::Player> SteamworksNetworkingService::AcknowledgePlayerConnection( const NetworkID& networkID ) {
		const auto	candidate( _playerDirectory.Find( networkID ) );

		if( candidate == _playerDirectory.End() ) {
			FormatAndLogString( UTF8L("Player %ull connected (local ID: %u)") ET_UTF8_NEWLINE_LITERAL, networkID.first.ConvertToUint64(), networkID.second );

			if( auto* const player = new(_allocator, Allocator::AllocationOption::PERMANENT_ALLOCATION) Player( networkID, *this, _allocator ) ) {
				_playerDirectory.Insert( ::rde::make_pair( networkID, ObjectHandle<Player>( *player, ::Eldritch2::PassthroughReferenceCountingSemantics ) ) );
				
				return { player, Errors::NONE };
			}

			return { nullptr, Errors::OUT_OF_MEMORY };
		} else {
			FormatAndLogWarning( UTF8L("Received duplicate player join notification for player %ull (local ID: %u)") ET_UTF8_NEWLINE_LITERAL, networkID.first.ConvertToUint64(), networkID.second );
			return { candidate->second, Errors::NONE };
		}
	}

// ---------------------------------------------------

	void SteamworksNetworkingService::AcknowledgePlayerDisconnection( const NetworkID& networkID ) {
		_playerDirectory.Erase( networkID );
	}

// ---------------------------------------------------

	void SteamworksNetworkingService::AcknowledgeClientDisconnection( const ::CSteamID clientID ) {
		using PlayerDirectory = decltype(_playerDirectory);

	// ---

		_playerDirectory.Erase( _playerDirectory.RemoveIf( [clientID] ( const PlayerDirectory::ValueType& client ) { return client.first.first == clientID; } ), _playerDirectory.End() );
	}

// ---------------------------------------------------

	ErrorCode SteamworksNetworkingService::ProcessAndDispatchCallbacks() {
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

		return Errors::NONE;
	}

// ---------------------------------------------------

	void SteamworksNetworkingService::OnSteamServersConnected( ::SteamServersConnected_t* /*connectionParameters*/ ) {
		FormatAndLogString( UTF8L("Steam connection re-established.") ET_UTF8_NEWLINE_LITERAL );
	}

// ---------------------------------------------------

	void SteamworksNetworkingService::OnSteamServersConnectFailure( ::SteamServerConnectFailure_t* /*connectionFailureParameters*/ ) {}

// ---------------------------------------------------

	void SteamworksNetworkingService::OnSteamServersDisconnected( ::SteamServersDisconnected_t* /*disconnectionParameters*/ ) {
		FormatAndLogError( UTF8L("Disconnected from Steam servers!") ET_UTF8_NEWLINE_LITERAL );
	}

// ---------------------------------------------------

	void SteamworksNetworkingService::OnP2PSessionRequest( ::P2PSessionRequest_t* requestedConnection ) {
		const ::CSteamID	accountID( requestedConnection->m_steamIDRemote );

		if( !_banList.Contains( accountID ) ) {
			::SteamGameServerNetworking()->AcceptP2PSessionWithUser( accountID );
		}
	}

// ---------------------------------------------------

	void SteamworksNetworkingService::OnP2PSessionConnectionFail( ::P2PSessionConnectFail_t* failedConnection ) {
		static const UTF8Char* const	disconnectionReasonStrings[::EP2PSessionError::k_EP2PSessionErrorMax] = {
			UTF8L( "" ),
			UTF8L( ": Player not running same application" ),
			UTF8L( ": Local player does not have permission to run the application" ),
			UTF8L( ": Remote player not logged in to Steam" ),
			UTF8L( ": Connection timeout/blocked" )
		};
		
		FormatAndLogWarning( UTF8L("Dropped connection to client %ull%s.") ET_UTF8_NEWLINE_LITERAL, failedConnection->m_steamIDRemote.ConvertToUint64(), disconnectionReasonStrings[failedConnection->m_eP2PSessionError] );

		AcknowledgeClientDisconnection( failedConnection->m_steamIDRemote );
	}

// ---------------------------------------------------

	void SteamworksNetworkingService::OnPolicyResponse( ::GSPolicyResponse_t* ) {}

// ---------------------------------------------------

	void SteamworksNetworkingService::OnValidateAuthTicketResponse( ::ValidateAuthTicketResponse_t* /*pParam*/ ) {}

// ---------------------------------------------------

	void SteamworksNetworkingService::OnClientGameServerDeny( ::ClientGameServerDeny_t* ) {}

// ---------------------------------------------------

	void SteamworksNetworkingService::OnClientGameServerKick( ::GSClientKick_t* kickedClient ) {
		// EDenyReason m_eDenyReason;
		AcknowledgeClientDisconnection( kickedClient->m_SteamID );
	}

}	// namespace Networking
}	// namespace Eldritch2