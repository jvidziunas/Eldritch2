/*==================================================================*\
  SteamworksEngineComponent.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Networking/Steamworks/SteamworksEngineComponent.hpp>
#include <Networking/Steamworks/SteamworksWorldComponent.hpp>
#include <Core/PropertyRegistrar.hpp>
#include <Core/Engine.hpp>
#include <Build.hpp>
//------------------------------------------------------------------//
#include <microprofile/microprofile.h>
ET_PUSH_COMPILER_WARNING_STATE()
//	(6340) Valve has a few mismatches in their printf specifiers! We can't fix these, so disable the warning.
	ET_SET_MSVC_WARNING_STATE( disable : 6340 )
#	include <steam_api.h>
ET_POP_COMPILER_WARNING_STATE()
//------------------------------------------------------------------//

//==================================================================//
// LIBRARIES
//==================================================================//
#if ET_PLATFORM_WINDOWS
	ET_LINK_LIBRARY( "steam_api64.lib" )
#endif
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Networking {
namespace Steamworks {

	using namespace ::Eldritch2::Scheduling;
	using namespace ::Eldritch2::Logging;
	using namespace ::Eldritch2::Core;

	SteamworksEngineComponent::SteamworksEngineComponent(
		const Blackboard& services,
		Log& log
	) : EngineComponent( services ),
		_log( log ),
		_steamPort( DefaultSteamPort ),
		_worldPorts( MallocAllocator( "Steamworks Port Pool Allocator" ) ),
		_bannedIds( MallocAllocator( "Steamworks Banned ID List Allocator" ) ) {
	}

// ---------------------------------------------------

	SteamworksEngineComponent::~SteamworksEngineComponent() {
		SteamAPI_Shutdown();
	}

// ---------------------------------------------------

	Result<UniquePointer<WorldComponent>> SteamworksEngineComponent::CreateWorldComponent( Allocator& allocator, const World& world ) {
		UniquePointer<WorldComponent> steamworksComponent( CreateUnique<SteamworksWorldComponent>( allocator, world ) );
		if (!steamworksComponent) {
			return Error::OutOfMemory;
		}

		return eastl::move( steamworksComponent );
	}

// ---------------------------------------------------

	void SteamworksEngineComponent::AcceptVisitor( JobExecutor& /*executor*/, const ConfigurationBroadcastVisitor ) {
		MICROPROFILE_SCOPEI( "Engine/Initialization", "Steamworks initialization", 0xBBBBBB );

		if (_worldPorts.IsEmpty()) {
			_log.Write( MessageType::VerboseWarning, "No world ports allocated to Steam, using range [{} - {}]." UTF8_NEWLINE, DefaultWorldPortBegin, DefaultWorldPortEnd );
		//	Add a small set of default ports.
			_worldPorts.ReleaseRange( { DefaultWorldPortBegin, DefaultWorldPortEnd } );
		}

	//	Attempt to establish a connection with the Steam master servers.
		if (!SteamAPI_Init()) {
			_log.Write( MessageType::Error, "Unable to initialize Steam API!" UTF8_NEWLINE );
			FindService<Engine>().SetShouldShutDown();
			return;
		}

		SteamClient()->SetLocalIPBinding( 0u, _steamPort );
		SteamClient()->SetWarningMessageHook( [] ( int severity, const char* message ) {
			OutputDebugStringA( message );

			if (severity >= 1) {
				ET_TRIGGER_DEBUGBREAK();
			}
		} );

		_log.Write( MessageType::Message, "Connected to local Steam client." UTF8_NEWLINE );
	}

// ---------------------------------------------------

	void SteamworksEngineComponent::AcceptVisitor( PropertyRegistrar& properties ) {
		MICROPROFILE_SCOPEI( "Engine/Initialization", "Steamworks property registration", 0xBBBBBB );

		properties.BeginSection( "Steamworks" )
			.WhenPropertyChanged( "SteamPort", _steamPort );

		properties.BeginSection( "Steamworks.BannedSteamIds" )
			.WhenDynamicPropertyChanged( [this] ( const Utf8Char* /*name*/, Range<const Utf8Char*> /*value*/ ) {
				_bannedIds.Insert( CSteamID() );
			} );
	}

// ---------------------------------------------------

	void SteamworksEngineComponent::AcceptVisitor( JobExecutor& /*executor*/, const ServiceTickVisitor ) {
		MICROPROFILE_SCOPEI( "Engine/ServiceTick", "Steamworks callback dispatch", 0xAAAAAA );

	/*	Run callbacks only for the engine-tier Steam pipe. World components will individually maintain separate
	 *	communication channels with the Steam client and are responsible for dispatching their own callbacks. */
		Steam_RunCallbacks( SteamAPI_GetHSteamPipe(), false );
		SteamAPI_ReleaseCurrentThreadMemory();
	}

// ---------------------------------------------------

	void SteamworksEngineComponent::AddLocalUser() {}

}	// namespace Steamworks
}	// namespace Networking
}	// namespace Eldritch2