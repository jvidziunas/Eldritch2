/*==================================================================*\
  SteamworksNetworkingService.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Configuration/ConfigurablePODVariable.hpp>
#include <Configuration/ConfigurableUTF8String.hpp>
#include <Utility/Containers/FlatOrderedMap.hpp>
#include <Utility/Containers/FlatOrderedSet.hpp>
#include <Utility/Containers/UTF8String.hpp>
#include <Foundation/GameEngineService.hpp>
#include <Utility/Concurrency/Lock.hpp>
#include <Utility/IdentifierPool.hpp>
#include <Scripting/ObjectHandle.hpp>
#include <Foundation/Player.hpp>
//------------------------------------------------------------------//
#include <isteamclient.h>
#include <isteamnetworking.h>
#include <steam_gameserver.h>
//------------------------------------------------------------------//
#include <atomic>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Networking {
		class	SteamworksWorldView;
	}

	namespace Utility {
		template <typename ResultObject>
		struct	DisposingResultPair;
		template <typename ResultObject>
		struct	ResultPair;
	}
}

namespace Eldritch2 {
namespace Networking {

	namespace Configuration	= ::Eldritch2::Configuration;
	namespace Foundation	= ::Eldritch2::Foundation;
	namespace Networking	= ::Eldritch2::Networking;
	namespace Scripting		= ::Eldritch2::Scripting;
	namespace Utility		= ::Eldritch2::Utility;

// ---------------------------------------------------

	class SteamworksNetworkingService : public Foundation::GameEngineService {
	// - TYPE PUBLISHING ---------------------------------

	public:
		typedef ::std::pair<::CSteamID, int>	NetworkID;

		enum : size_t {
			NETWORK_MTU_SIZE	= 1200u
		};

	// ---

		class Player : public Foundation::Player {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			// Constructs this Player instance.
			Player( const NetworkID& networkID, SteamworksNetworkingService& networkingService, ::Eldritch2::Allocator& allocator );

			// Destroys this Player instance.
			~Player();
		
		// ---------------------------------------------------

			void	Dispose() override sealed;

		// ---------------------------------------------------

			const ::Eldritch2::UTF8Char*	GetName() const override sealed;

			void							NotifyPersonaNameChange( const ::Eldritch2::UTF8Char* const newName );

		// - DATA MEMBERS ------------------------------------

		private:
			::Eldritch2::UTF8String<>		_name;
			SteamworksNetworkingService&	_networkingService;
			const NetworkID					_networkID;
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		// Initializes this SteamworksNetworkingService instance.
		SteamworksNetworkingService( Foundation::GameEngine& owningEngine );

		// Destroys this SteamworksNetworkingService instance.
		~SteamworksNetworkingService();

	// ---------------------------------------------------

		const ::Eldritch2::UTF8Char* const	GetName() const override sealed;

	// ---------------------------------------------------

		bool	TryRecievePacket( const int channelID, ::CSteamID& senderID, char( &packetBuffer )[NETWORK_MTU_SIZE] );

		void	TrySendPacket( const int channelID, const ::CSteamID destinationID, void* const packetBuffer, const size_t packetSizeInBytes );

		void	CloseReplicationOnChannel( const int channelID );

	// ---------------------------------------------------

	protected:
		void	AcceptInitializationVisitor( Configuration::ConfigurationPublishingInitializationVisitor& visitor ) override sealed;
		void	AcceptInitializationVisitor( Foundation::WorldViewFactoryPublishingInitializationVisitor& visitor ) override sealed;
		void	AcceptInitializationVisitor( Scripting::ScriptAPIRegistrationInitializationVisitor& visitor ) override sealed;
		void	AcceptInitializationVisitor( const PostInitializationVisitor ) override sealed;

		void	AcceptTaskVisitor( ::Eldritch2::Allocator& subtaskAllocator, Scheduler::Task& visitingTask, Scheduler::WorkerContext& executingContext, const PostConfigurationLoadedTaskVisitor ) override sealed;
		void	AcceptTaskVisitor( ::Eldritch2::Allocator& subtaskAllocator, Scheduler::Task& visitingTask, Scheduler::WorkerContext& executingContext, const ServiceTickTaskVisitor ) override sealed;

	// ---------------------------------------------------

		Utility::ResultPair<Player>	AcknowledgePlayerConnection( const NetworkID& networkID );

		::Eldritch2::ErrorCode		ProcessAndDispatchCallbacks();

		void						InitiateSteamConnection();

		void						AcknowledgePlayerDisconnection( const NetworkID& networkID );

		void						AcknowledgeClientDisconnection( const ::CSteamID clientID );

	// - DATA MEMBERS ------------------------------------

	private:
		// Tells us when we have successfully connected to Steam
		STEAM_GAMESERVER_CALLBACK( SteamworksNetworkingService, OnSteamServersConnected,		::SteamServersConnected_t,		_steamServersConnected );
		// Tells us when there was a failure to connect to Steam
		STEAM_GAMESERVER_CALLBACK( SteamworksNetworkingService, OnSteamServersConnectFailure,	::SteamServerConnectFailure_t,	_steamServersConnectFailure );
		// Tells us when we have been logged out of Steam
		STEAM_GAMESERVER_CALLBACK( SteamworksNetworkingService, OnSteamServersDisconnected,		::SteamServersDisconnected_t,	_steamServersDisconnected );
		STEAM_GAMESERVER_CALLBACK( SteamworksNetworkingService, OnP2PSessionRequest,			::P2PSessionRequest_t,			_peerToPeerSessionRequest );
		STEAM_GAMESERVER_CALLBACK( SteamworksNetworkingService,	OnP2PSessionConnectionFail,		::P2PSessionConnectFail_t,		_peerToPeerSessionConnectFail );
		STEAM_GAMESERVER_CALLBACK( SteamworksNetworkingService,	OnPolicyResponse,				::GSPolicyResponse_t,			_policyResponse );
		STEAM_GAMESERVER_CALLBACK( SteamworksNetworkingService, OnValidateAuthTicketResponse,	::ValidateAuthTicketResponse_t, _gameServerAuthTicketResponse );
		STEAM_GAMESERVER_CALLBACK( SteamworksNetworkingService,	OnClientGameServerDeny,			::ClientGameServerDeny_t,		_clientGameServerDeny );
		STEAM_GAMESERVER_CALLBACK( SteamworksNetworkingService, OnClientGameServerKick,			::GSClientKick_t,				_clientGameServerKick );

		::Eldritch2::ChildAllocator												_allocator;

		Utility::ReaderWriterUserMutex*											_networkMutex;

		::std::atomic<int>														_worldIDCounter;

		Configuration::ConfigurablePODVariable<::Eldritch2::uint16>				_steamPort;
		Configuration::ConfigurablePODVariable<::Eldritch2::uint16>				_gamePort;
		Configuration::ConfigurablePODVariable<::Eldritch2::uint16>				_queryPort;

		Configuration::ConfigurableUTF8String									_versionString;
		Configuration::ConfigurableUTF8String									_lobbyWorldName;

		Configuration::ConfigurablePODVariable<bool>							_useVACAuthentication;
		Configuration::ConfigurablePODVariable<bool>							_useSteamworks;

		Configuration::ConfigurablePODVariable<::Eldritch2::uint32>				_replicationUploadBandwidthLimitInKilobytesPerSecond;
		Configuration::ConfigurablePODVariable<::Eldritch2::uint32>				_replicationDownloadBandwidthLimitInKilobytesPerSecond;

		Configuration::ConfigurablePODVariable<::Eldritch2::uint32>				_contentDownloadBandwidthLimitInKilobytesPerSecond;
		Configuration::ConfigurablePODVariable<::Eldritch2::uint32>				_contentUploadBandwidthLimitInKilobytesPerSecond;

		::Eldritch2::FlatOrderedSet<::CSteamID>									_banList;
		::Eldritch2::FlatOrderedMap<NetworkID, Scripting::ObjectHandle<Player>>	_playerDirectory;
		Scripting::ObjectHandle<Foundation::World>								_lobbyWorld;
	};

}	// namespace Networking
}	// namespace Eldritch2