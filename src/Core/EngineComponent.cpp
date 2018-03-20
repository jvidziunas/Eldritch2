/*==================================================================*\
  EngineComponent.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Core/EngineComponent.hpp>
#include <Core/WorldComponent.hpp>
//------------------------------------------------------------------//
#include <microprofile/microprofile.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Core {

	using namespace ::Eldritch2::Scheduling;
	using namespace ::Eldritch2::Scripting;
	using namespace ::Eldritch2::Assets;

	Result<UniquePointer<WorldComponent>> EngineComponent::CreateWorldComponent( Allocator& /*allocator*/, const World& /*world*/ ) {
		return UniquePointer<WorldComponent>( nullptr );
	}

// ---------------------------------------------------

	void EngineComponent::AcceptVisitor( JobExecutor& /*executor*/, const InitializationVisitor ) {
		MICROPROFILE_SCOPEI( "Engine/Initialization", "Default early-phase initialization", 0xBBBBBB );

	//	Default implementation should not do anything.
	}

// ---------------------------------------------------

	void EngineComponent::AcceptVisitor( JobExecutor& /*executor*/, const ConfigurationBroadcastVisitor ) {
		MICROPROFILE_SCOPEI( "Engine/Initialization", "Default configuration broadcast", 0xBBBBBB );

	//	Default implementation should not do anything.
	}

// ---------------------------------------------------

	void EngineComponent::AcceptVisitor( PropertyRegistrar& /*properties*/ ) {
		MICROPROFILE_SCOPEI( "Engine/Initialization", "Default property registration", 0xBBBBBB );

	//	Default implementation should not do anything.
	}

// ---------------------------------------------------

	void EngineComponent::AcceptVisitor( AssetApiBuilder& /*factories*/ ) {
		MICROPROFILE_SCOPEI( "Engine/Initialization", "Default asset factory registration", 0xBBBBBB );

	//	Default implementation should not do anything.
	}

// ---------------------------------------------------

	void EngineComponent::AcceptVisitor( Blackboard& /*services*/ ) {
		MICROPROFILE_SCOPEI( "Engine/Initialization", "Default shared service registration", 0xBBBBBB );

	//	Default implementation should not do anything.
	}

// ---------------------------------------------------

	void EngineComponent::AcceptVisitor( JobExecutor& /*executor*/, const LateInitializationVisitor ) {
		MICROPROFILE_SCOPEI( "Engine/LateInitialization", "Default late-phase initialization", 0xBBBBBB );

	//	Default implementation should not do anything.
	}

// ---------------------------------------------------

	void EngineComponent::AcceptVisitor( JobExecutor& /*executor*/, const ServiceTickVisitor ) {
		MICROPROFILE_SCOPEI( "Engine/ServiceTick", "Default service tick", 0xBBBBBB );

	//	Default implementation should not do anything.
	}

// ---------------------------------------------------

	void EngineComponent::AcceptVisitor( JobExecutor& /*executor*/, const WorldTickVisitor ) {
		MICROPROFILE_SCOPEI( "Engine/WorldTick", "Default world tick", 0xBBBBBB );

	//	Default implementation should not do anything.
	}

}	// namespace Core
}	// namespace Eldritch2