/*==================================================================*\
  WorldView.ArticulatedBodyComponent.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/BulletDynamics/ArticulatedBodyResourceView.hpp>
#include <Physics/BulletDynamics/WorldView.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Physics {
namespace BulletDynamics {

	WorldView::ArticulatedBodyComponent::ArticulatedBodyComponent( const ArticulatedBodyResourceView& asset, WorldView& owningView ) {
		_bodies.SetCapacity( asset.GetBodies().Size() );

		for( const auto& bodyDefinition : asset.GetBodies() ) {
			::btRigidBody::btRigidBodyConstructionInfo	constructionInfo( bodyDefinition.mass, nullptr, bodyDefinition.shape );

			constructionInfo.m_linearDamping	= bodyDefinition.linearDamping;
			constructionInfo.m_angularDamping	= bodyDefinition.angularDamping;
			constructionInfo.m_friction			= bodyDefinition.friction;
			constructionInfo.m_rollingFriction	= bodyDefinition.rollingFriction;
			constructionInfo.m_restitution		= bodyDefinition.restitution;

			_bodies.EmplaceBack( constructionInfo );
		}
	}

// ---------------------------------------------------

	WorldView::ArticulatedBodyComponent::~ArticulatedBodyComponent() {
		// Ensure there are no dangling pointers left in the dynamics world
		SetEnabled( false );
	}

// ---------------------------------------------------

	void WorldView::ArticulatedBodyComponent::SetEnabled( bool value /*= true*/ ) {
		auto&	dynamicsWorld( GetActiveWorldView()._dynamicsWorld );

		if( _enabled == value ) {
			return;
		}

		_enabled = value;

		if( value ) {
			for( auto& body : _bodies ) {
				dynamicsWorld.addRigidBody( &body );
			}
		} else {
			for( auto& body : _bodies ) {
				dynamicsWorld.removeRigidBody( &body );
			}
		}
	}

// ---------------------------------------------------

	ETNoAliasHint void WorldView::ArticulatedBodyComponent::ExposeScriptAPI( Scripting::ScriptAPIRegistrationInitializationVisitor& /*typeRegistrar*/ ) {}

// ---------------------------------------------------

	void WorldView::ArticulatedBodyComponent::Dispose() {
		GetActiveWorldView()._allocator.Delete( *this );
	}

}	// namespace BulletDynamics
}	// namespace Physics
}	// namespace Eldritch2