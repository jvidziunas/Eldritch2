/*==================================================================*\
  BulletEngine.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Physics {

	namespace Physics		= ::Eldritch2::Physics;
	
// ---------------------------------------------------

	ETInlineHint size_t	BulletEngine::GetWorldPersistentManifoldPoolSizeInElements() const {
		return _persistentManifoldPoolSizeInElements;
	}

// ---------------------------------------------------

	ETInlineHint size_t	BulletEngine::GetWorldCollisionAlgorithmPoolSizeInElements() const {
		return _collisionAlgorithmPoolSizeInElements;
	}

}	// namespace Physics
}	// namespace Eldritch2