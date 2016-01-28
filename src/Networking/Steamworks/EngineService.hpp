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
#include <steamclientpublic.h>
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
		class Player : public Foundation::Player {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref Player instance.
			Player( const Scripting::ObjectHandle<Foundation::World>& world, EngineService& service, ::Eldritch2::Allocator& allocator );

			//!	Destroys this @ref Player instance.
			~Player() = default;
		
		// ---------------------------------------------------

			void	Dispose() override sealed;

		// ---------------------------------------------------

			const ::Eldritch2::UTF8Char*	GetName() const override sealed;

			void							NotifyPersonaNameChange( const ::Eldritch2::UTF8Char* const newName );

		// - DATA MEMBERS ------------------------------------

		private:
			::Eldritch2::UTF8String<>					_name;
			EngineService&								_service;
			Scripting::ObjectHandle<Foundation::World>	_world;
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		//! Constructs this @ref EngineService instance.
		EngineService( Foundation::GameEngine& owningEngine );

		//! Destroys this @ref EngineService instance.
		~EngineService();

	// ---------------------------------------------------

		const ::Eldritch2::UTF8Char* const	GetName() const override sealed;

	// ---------------------------------------------------

		::Eldritch2::ErrorCode	AllocateWorldView( ::Eldritch2::Allocator& allocator, Foundation::World& world ) override;

	// ---------------------------------------------------

	protected:
		void	OnEngineConfigurationBroadcast( Scheduler::WorkerContext& executingContext ) override sealed;

		void	AcceptInitializationVisitor( Configuration::ConfigurationPublishingInitializationVisitor& visitor ) override sealed;
		void	AcceptInitializationVisitor( Scripting::ScriptAPIRegistrationInitializationVisitor& visitor ) override sealed;

		void	OnEngineInitializationCompleted( Scheduler::WorkerContext& executingContext ) override sealed;

	// ---------------------------------------------------

		void	OnServiceTickStarted( Scheduler::WorkerContext& executingContext ) override sealed;

	// ---------------------------------------------------

		void	OnPlayerConnected();

	// - DATA MEMBERS ------------------------------------

	private:
		::Eldritch2::ChildAllocator											_allocator;

		Configuration::ConfigurablePODVariable<::Eldritch2::uint16>			_steamPort;
		Configuration::ConfigurablePODVariable<::Eldritch2::uint16>			_gamePort;
		Configuration::ConfigurablePODVariable<::Eldritch2::uint16>			_queryPort;

		Configuration::ConfigurableUTF8String								_versionString;
		Configuration::ConfigurableUTF8String								_lobbyWorldName;

		Configuration::ConfigurablePODVariable<bool>						_useVACAuthentication;
		Configuration::ConfigurablePODVariable<bool>						_useSteamworks;

		Configuration::ConfigurablePODVariable<::Eldritch2::uint32>			_replicationUploadBandwidthLimitInKilobytesPerSecond;
		Configuration::ConfigurablePODVariable<::Eldritch2::uint32>			_replicationDownloadBandwidthLimitInKilobytesPerSecond;

		Configuration::ConfigurablePODVariable<::Eldritch2::uint32>			_contentDownloadBandwidthLimitInKilobytesPerSecond;
		Configuration::ConfigurablePODVariable<::Eldritch2::uint32>			_contentUploadBandwidthLimitInKilobytesPerSecond;

		::Eldritch2::FlatOrderedSet<::CSteamID>								_banList;

		Scripting::ObjectHandle<Foundation::World>							_lobbyWorld;
	};

}	// namespace Steamworks
}	// namespace Networking
}	// namespace Eldritch2