/*==================================================================*\
  EngineService.inl
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
namespace BulletDynamics {

	ETInlineHint size_t	EngineService::GetWorldPersistentManifoldPoolSizeInElements() const {
		return _persistentManifoldPoolSizeInElements;
	}

// ---------------------------------------------------

	ETInlineHint size_t	EngineService::GetWorldCollisionAlgorithmPoolSizeInElements() const {
		return _collisionAlgorithmPoolSizeInElements;
	}

}	// namespace BulletDynamics
}	// namespace Physics
}	// namespace Eldritch2