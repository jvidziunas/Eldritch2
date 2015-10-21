/*==================================================================*\
  BulletEngineService.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
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

	ETInlineHint size_t	BulletEngineService::GetWorldPersistentManifoldPoolSizeInElements() const {
		return _persistentManifoldPoolSizeInElements;
	}

// ---------------------------------------------------

	ETInlineHint size_t	BulletEngineService::GetWorldCollisionAlgorithmPoolSizeInElements() const {
		return _collisionAlgorithmPoolSizeInElements;
	}

}	// namespace Physics
}	// namespace Eldritch2