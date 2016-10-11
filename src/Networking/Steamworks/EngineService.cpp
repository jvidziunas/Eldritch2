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
#include <Configuration/ConfigurationRegistrar.hpp>
#include <Networking/Steamworks/EngineService.hpp>
#include <Networking/Steamworks/WorldService.hpp>
#include <Scripting/ApiRegistrar.hpp>
#include <Utility/Result.hpp>
#include <Core/Engine.hpp>
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

using namespace ::Eldritch2::Configuration;
using namespace ::Eldritch2::Scheduling;
using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Platform;
using namespace ::Eldritch2::Logging;
using namespace ::Eldritch2::Core;

namespace Eldritch2 {
namespace Networking {
namespace Steamworks {

	EngineService::EngineService( const Engine& engine ) : Core::EngineService( engine.GetServiceBlackboard() ),
														   _log( engine.GetLog() ),
														   _allocator( engine.GetAllocator(), "Steamworks Networking Service Root Allocator" ),
														   _steamPort( 6690u ),
														   _networkPortPool( { _allocator, "Steamworks Networking Service Port Pool Allocator" } ),
														   _versionString( VERSION_STRING, { _allocator, "Steamworks Version String Allocator" } ),
														   _lobbyWorldName( PROJECT_NAME "Lobby", { _allocator, "Steamworks Lobby World Name Allocator" } ),
														   _useVacAuthentication( true ),
														   _replicationUploadBandwidthLimitInKilobytesPerSecond( 1024u ),
														   _replicationDownloadBandwidthLimitInKilobytesPerSecond( 3u * 1024u ),
														   _contentDownloadBandwidthLimitInKilobytesPerSecond( 3u * 1024u ),
														   _contentUploadBandwidthLimitInKilobytesPerSecond( 1024u ),
														   _banList( { _allocator, "Steamworks Ban List Allocator" } ) {}

// ---------------------------------------------------

	EngineService::~EngineService() {
		SteamAPI_Shutdown();
	}

// ---------------------------------------------------

	Utf8Literal EngineService::GetName() const { 
		return "Steamworks Networking Service";
	}

// ---------------------------------------------------

	Result<UniquePointer<Core::WorldService>> EngineService::CreateWorldService( Allocator& allocator, const World& world ) {
		if( !SteamAPI_IsSteamRunning() ) {
			_log( MessageSeverity::Error, "Unable to create world Steamworks network presence: Steam not running!" ET_UTF8_NEWLINE_LITERAL );
			return Error::None;
		}

		if( SteamClient() == nullptr ) {
			_log( MessageSeverity::Error, "Unable to create world Steamworks network presence: No connection to local Steam client!" ET_UTF8_NEWLINE_LITERAL );
			return Error::None;
		}

		HSteamPipe	pipe( SteamClient()->CreateSteamPipe() );
		HSteamUser	user( SteamClient()->CreateLocalUser( &pipe, EAccountType::k_EAccountTypeAnonGameServer ) );

		auto	result( MakeUnique<WorldService>( allocator, pipe, user, world ) );
		if( !result ) {
			return Error::OutOfMemory;
		}

		return eastl::move( result );
	}

// ---------------------------------------------------

	void EngineService::AcceptVisitor( JobFiber& /*fiber*/, const ConfigurationBroadcastVisitor ) {
		MICROPROFILE_SCOPEI( GetName(), "Initiate Steam connection", 0xBBBBBB );

		if( _networkPortPool.IsEmpty() ) {
			_log( MessageSeverity::VerboseWarning, "No network ports assigned to Steam, using default." ET_UTF8_NEWLINE_LITERAL );
		//	Add a small set of default ports.
			_networkPortPool.ReleaseRange( { 6691u, 6671u } );
		}

		_log( MessageSeverity::Message, "Connecting to Steam instance." ET_UTF8_NEWLINE_LITERAL );

	//	Attempt to establish a connection with the Steam master servers.
		if( !SteamAPI_Init() ) {
			_log( MessageSeverity::Error, "Unable to initialize Steam API!" ET_UTF8_NEWLINE_LITERAL );
			return;
		}

		SteamClient()->SetLocalIPBinding( 0u, _steamPort );

		_log( MessageSeverity::Message, "Initial Steam connection established." ET_UTF8_NEWLINE_LITERAL );
	}

// ---------------------------------------------------

	void EngineService::AcceptVisitor( ConfigurationRegistrar& registrar ) {
		registrar.BeginSection( "Steamworks" )
			.Register( "SteamPort", _steamPort )
			.Register( "VersionString", [&] ( Range<const Utf8Char*> value ) { _versionString.Assign( value.Begin(), value.End() ); } )
			.Register( "UseValveAntiCheatAuthentication", _useVacAuthentication )
			.Register( "ReplicationUploadBandwidthLimitInKilobytesPerSecond", _replicationUploadBandwidthLimitInKilobytesPerSecond )
			.Register( "ReplicationDownloadBandwidthLimitInKilobytesPerSecond", _replicationDownloadBandwidthLimitInKilobytesPerSecond )
			.Register( "ContentDownloadBandwidthLimitInKilobytesPerSecond", _contentDownloadBandwidthLimitInKilobytesPerSecond )
			.Register( "ContentUploadBandwidthLimitInKilobytesPerSecond", _contentUploadBandwidthLimitInKilobytesPerSecond );

		registrar.BeginSection( "Steamworks.BannedSteamIdList" )
			.RegisterDynamicKeySetter( [&] ( Utf8Literal valueName, Range<const Utf8Char*> /*value*/ ) {
				_banList.Insert( CSteamID() );
			} );

		registrar.BeginSection( "Engine" )
			.Register( "LobbyWorldName", [&] ( Range<const Utf8Char*> value ) {
				_lobbyWorldName.Assign( value.Begin(), value.End() );
			} );
	}

// ---------------------------------------------------

	void EngineService::AcceptVisitor( ApiRegistrar& visitor ) {
		WorldService::RegisterScriptApi( visitor );
	}

// ---------------------------------------------------

	void EngineService::AcceptVisitor( ServiceBlackboard& /*serviceLocator*/ ) {
	//	Nothing here for now.
	}

// ---------------------------------------------------

	void EngineService::AcceptVisitor( JobFiber& /*fiber*/, const InitializationCompleteVisitor ) {
		Engine&			engine( LocateService<Engine>() );
		const ErrorCode	createWorldResult( engine.CreateWorld( { { World::GetRequiredPackagesKey(), _lobbyWorldName }, { World::GetRulesKey(), "LobbyRules" } } ) );

		if( !createWorldResult ) {
			_log( MessageSeverity::Error, "Error creating lobby world: {}!" ET_UTF8_NEWLINE_LITERAL, GetErrorString( createWorldResult ) );
			engine.ShutDown();
			return;
		}
	}

// ---------------------------------------------------

	void EngineService::AcceptVisitor( JobFiber& /*fiber*/, const ServiceTickVisitor ) {
		MICROPROFILE_SCOPEI( GetName(), "Process and dispatch callbacks", 0xAAAAAA );

		SteamAPI_RunCallbacks();
	}

}	// namespace Steamworks
}	// namespace Networking
}	// namespace Eldritch2