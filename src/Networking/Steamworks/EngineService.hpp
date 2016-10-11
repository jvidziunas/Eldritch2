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
#include <Networking/Steamworks/SteamTypeHelpers.hpp>
#include <Utility/Containers/Utf8String.hpp>
#include <Platform/UserReadWriteMutex.hpp>
#include <Utility/Containers/HashSet.hpp>
#include <Utility/CountedPointer.hpp>
#include <Utility/IdentifierPool.hpp>
#include <Core/EngineService.hpp>
#include <Logging/ChildLog.hpp>
#include <Core/Player.hpp>
//------------------------------------------------------------------//
#if( ET_COMPILER_IS_MSVC )
//	Valve has a few mismatches in their printf specifiers, it seems! We can't fix these, so disable the warning.
#	pragma warning( push )
#	pragma warning( disable : 6340 )
#endif
#include <isteamclient.h>
#if( ET_COMPILER_IS_MSVC )
#	pragma warning( pop )
#endif
//------------------------------------------------------------------//
#include <atomic>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Core {
		class	Engine;
	}
}

namespace Eldritch2 {
namespace Networking {
namespace Steamworks {

	class EngineService : public Core::EngineService {
	// - TYPE PUBLISHING ---------------------------------

	public:
		class Player : public Core::Player {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
		//!	Constructs this @ref Player instance.
			Player( EngineService& service, Eldritch2::Allocator& allocator );
		//!	Disable copying.
			Player( const Player& ) = delete;

			~Player() = default;

		// ---------------------------------------------------

		public:
			const Eldritch2::Utf8Char*	GetName() const override sealed;

			void						NotifyPersonaNameChange( const Eldritch2::Utf8Char* const newName );

		// ---------------------------------------------------

		//!	Disable assignment.
			Player&	operator=( const Player& ) = delete;

		// - DATA MEMBERS ------------------------------------

		private:
			CSteamID				_steamId;
			Eldritch2::Utf8String<>	_name;
			EngineService&			_service;
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//! Constructs this @ref EngineService instance.
		EngineService( const Core::Engine& engine );
	//!	Disable copying.
		EngineService( const EngineService& ) = delete;

	//! Destroys this @ref EngineService instance.
		~EngineService();

	// ---------------------------------------------------

	public:
		Eldritch2::Result<Eldritch2::UniquePointer<Core::WorldService>>	CreateWorldService( Eldritch2::Allocator& allocator, const Core::World& world ) override;

	// ---------------------------------------------------

	public:
		Eldritch2::Utf8Literal	GetName() const override sealed;

	// ---------------------------------------------------

	protected:
		void	AcceptVisitor( Scheduling::JobFiber& executor, const ConfigurationBroadcastVisitor ) override;
		void	AcceptVisitor( Configuration::ConfigurationRegistrar& registrar ) override;
		void	AcceptVisitor( Core::ServiceBlackboard& serviceBlackboard ) override;
		void	AcceptVisitor( Scripting::ApiRegistrar& registrar ) override;
		void	AcceptVisitor( Scheduling::JobFiber& executor, const InitializationCompleteVisitor ) override;
		void	AcceptVisitor( Scheduling::JobFiber& executor, const ServiceTickVisitor ) override;

	// ---------------------------------------------------

	protected:
		void	OnPlayerConnected();

	// ---------------------------------------------------

	//!	Disable assignment.
		EngineService&	operator=( const EngineService& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		mutable Eldritch2::ChildAllocator				_allocator;
		mutable Logging::ChildLog						_log;

		Eldritch2::uint16								_steamPort;

		Eldritch2::IdentifierPool<Eldritch2::uint16>	_networkPortPool;

		Eldritch2::Utf8String<>							_versionString;
		Eldritch2::Utf8String<>							_lobbyWorldName;

		bool											_useVacAuthentication;

		Eldritch2::uint32								_replicationUploadBandwidthLimitInKilobytesPerSecond;
		Eldritch2::uint32								_replicationDownloadBandwidthLimitInKilobytesPerSecond;

		Eldritch2::uint32								_contentDownloadBandwidthLimitInKilobytesPerSecond;
		Eldritch2::uint32								_contentUploadBandwidthLimitInKilobytesPerSecond;

		Eldritch2::HashSet<CSteamID>					_banList;
	};

}	// namespace Steamworks
}	// namespace Networking
}	// namespace Eldritch2