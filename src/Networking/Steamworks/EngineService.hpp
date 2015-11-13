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
#include <Utility/Result.hpp>
//------------------------------------------------------------------//
#if( ET_COMPILER_IS_MSVC )
//	Valve has a few mismatches in their printf specifiers, it seems! We can't fix these, so disable the warning.
#	pragma warning( push )
#	pragma warning( disable : 6340 )
#endif
#include <isteamclient.h>
#include <isteamnetworking.h>
#include <steam_gameserver.h>
#if( ET_COMPILER_IS_MSVC )
#	pragma warning( pop )
#endif
//------------------------------------------------------------------//
#include <atomic>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Networking {
		class	WorldView;
	}
}

namespace Eldritch2 {
namespace Networking {
namespace Steamworks {

	class EngineService : public Foundation::GameEngineService {
	// - TYPE PUBLISHING ---------------------------------

	public:
		enum : size_t {
			NETWORK_MTU_SIZE = 1200u
		};

	// ---

		using NetworkID	= ::std::pair<::CSteamID, int>;

	// ---

		class Player : public Foundation::Player {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref Player instance.
			Player( const NetworkID& networkID, EngineService& networkingService, ::Eldritch2::Allocator& allocator );

			//!	Destroys this @ref Player instance.
			~Player() = default;
		
		// ---------------------------------------------------

			void	Dispose() override sealed;

		// ---------------------------------------------------

			const ::Eldritch2::UTF8Char*	GetName() const override sealed;

			void							NotifyPersonaNameChange( const ::Eldritch2::UTF8Char* const newName );

		// - DATA MEMBERS ------------------------------------

		private:
			::Eldritch2::UTF8String<>	_name;
			EngineService&				_networkingService;
			const NetworkID				_networkID;
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		//! Constructs this @ref EngineService instance.
		EngineService( Foundation::GameEngine& owningEngine );

		//! Destroys this @ref EngineService instance.
		~EngineService();

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

		::Eldritch2::Result<Player>	AcknowledgePlayerConnection( const NetworkID& networkID );

		::Eldritch2::ErrorCode		ProcessAndDispatchCallbacks();

		void						InitiateSteamConnection();

		void						AcknowledgePlayerDisconnection( const NetworkID& networkID );

		void						AcknowledgeClientDisconnection( const ::CSteamID clientID );

	// - DATA MEMBERS ------------------------------------

	private:
		STEAM_GAMESERVER_CALLBACK( EngineService, OnSteamServersConnected,		::SteamServersConnected_t,		_steamServersConnected );
		STEAM_GAMESERVER_CALLBACK( EngineService, OnSteamServersConnectFailure,	::SteamServerConnectFailure_t,	_steamServersConnectFailure );
		STEAM_GAMESERVER_CALLBACK( EngineService, OnSteamServersDisconnected,	::SteamServersDisconnected_t,	_steamServersDisconnected );
		STEAM_GAMESERVER_CALLBACK( EngineService, OnP2PSessionRequest,			::P2PSessionRequest_t,			_peerToPeerSessionRequest );
		STEAM_GAMESERVER_CALLBACK( EngineService, OnP2PSessionConnectionFail,	::P2PSessionConnectFail_t,		_peerToPeerSessionConnectFail );
		STEAM_GAMESERVER_CALLBACK( EngineService, OnPolicyResponse,				::GSPolicyResponse_t,			_policyResponse );
		STEAM_GAMESERVER_CALLBACK( EngineService, OnValidateAuthTicketResponse,	::ValidateAuthTicketResponse_t, _gameServerAuthTicketResponse );
		STEAM_GAMESERVER_CALLBACK( EngineService, OnClientGameServerDeny,		::ClientGameServerDeny_t,		_clientGameServerDeny );
		STEAM_GAMESERVER_CALLBACK( EngineService, OnClientGameServerKick,		::GSClientKick_t,				_clientGameServerKick );

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

}	// namespace Steamworks
}	// namespace Networking
}	// namespace Eldritch2