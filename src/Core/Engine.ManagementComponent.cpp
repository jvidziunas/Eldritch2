/*==================================================================*\
  Engine.ManagementComponent.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  �2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scheduling/JobExecutor.hpp>
#include <Core/PropertyRegistrar.hpp>
#include <Assets/AssetDatabase.hpp>
#include <Core/Engine.hpp>
//------------------------------------------------------------------//
#include <microprofile/microprofile.h>
//------------------------------------------------------------------//
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Core {

	using namespace ::Eldritch2::Scheduling;
	using namespace ::Eldritch2::Scripting;
	using namespace ::Eldritch2::Logging;
	using namespace ::Eldritch2::Assets;

	Engine::ManagementComponent::ManagementComponent(
		Engine& owner
	) : EngineComponent( owner.GetBlackboard() ),
		_owner( &owner ),
		_maxPackagesSweptPerFrame( 5u ) {}

// ---------------------------------------------------

	Result<UniquePointer<WorldComponent>> Engine::ManagementComponent::CreateWorldComponent( Allocator& allocator, const World& world ) {
		UniquePointer<WorldComponent> component( CreateUnique<World::ManagementComponent>( allocator, world ) );
		if (!component) {
			return Error::OutOfMemory;
		}

		return eastl::move( component );
	}

// ---------------------------------------------------

	void Engine::ManagementComponent::AcceptVisitor( JobExecutor& executor, const InitializationVisitor ) {
		MICROPROFILE_SCOPEI( "Engine/Initialization", "Early-phase initialization", 0xBBBBBB );

	/*	Attach component services for dependency injection. This should be done *before* any asynchronous work since
	 *	we don't statically know what services our components will request from one another during initialization. */
		for (EngineComponent* component : _owner->_components) {
			component->AcceptVisitor( _owner->_services );
		}

		executor.AwaitWork(
			[this] ( JobExecutor& executor ) {
				MICROPROFILE_SCOPEI( "Engine/Initialization", "Load configuration", 0xBBBBBB );
			//	Load configuration data off the disk and broadcast values out to engine components.
				_owner->ApplyConfiguration( "Eldritch2.ini" );

				const ArrayList<EngineComponent*>&	components( _owner->_components );
				executor.ForEach<2u>( components.Begin(), components.End(), [] ( JobExecutor& executor, EngineComponent* component ) {
					component->AcceptVisitor( executor, ConfigurationBroadcastVisitor() );
				} );
			},
			[this] ( JobExecutor& /*executor*/ ) {
				MICROPROFILE_SCOPEI( "Engine/Initialization", "Build asset API", 0xBBBBBB );
				_owner->BuildAssetApi();
			},
			[this] ( JobExecutor& /*executor*/ ) {
				MICROPROFILE_SCOPEI( "Engine/Initialization", "Scan packages", 0xBBBBBB );
				_owner->ScanPackages();
			}
		);
	}

// ---------------------------------------------------

	void Engine::ManagementComponent::AcceptVisitor( JobExecutor& /*executor*/, const ServiceTickVisitor ) {
		_owner->SweepPackages( _maxPackagesSweptPerFrame );
	}

// ---------------------------------------------------

	void Engine::ManagementComponent::AcceptVisitor( JobExecutor& executor, const WorldTickVisitor ) {
		MICROPROFILE_SCOPEI( "Engine/WorldTick", "Tick worlds", 0xBBBBBB );

		_owner->TickWorlds( executor );
	}

// ---------------------------------------------------

	void Engine::ManagementComponent::AcceptVisitor( PropertyRegistrar& properties ) {
		MICROPROFILE_SCOPEI( "Engine/Initialization", "Core property registration", 0xBBBBBB );

		properties.BeginSection( "Engine" )
			.WhenPropertyChanged( "LogThreshold", [this] ( Range<const Utf8Char*> value ) {
				using namespace ::boost::iostreams;

				basic_array_source<Utf8Char>	source( value.Begin(), value.End() );
				stream<decltype(source)>		stream( source );

				MessageType	threshold;

				stream >> threshold;

				_owner->GetLog().SetMuteThreshold( threshold );
			} )
			.WhenPropertyChanged( "MaxPackagesSweptPerFrame", _maxPackagesSweptPerFrame );
	}

}	// namespace Core
}	// namespace Eldritch2