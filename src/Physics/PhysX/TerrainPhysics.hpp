/*==================================================================*\
  TerrainPhysics.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/PhysX/PhysxPointer.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Physics {
		namespace PhysX {
			namespace AssetViews {
				class	TerrainAsset;
			}
		}
	}
}

namespace physx {
	class	PxRigidStatic;
	class	PxScene;
}

namespace Eldritch2 {
namespace Physics {
namespace PhysX {

	class TerrainPhysics {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//! Constructs this @ref TerrainPhysics instance.
		TerrainPhysics( PhysxPointer<physx::PxRigidStatic> actor, const AssetViews::TerrainAsset& asset );
	//!	Disable copy construction.
		TerrainPhysics( const TerrainPhysics& ) = delete;

		~TerrainPhysics() = default;

	// ---------------------------------------------------

	public:
		void	Enable();

		void	Disable();

	// ---------------------------------------------------

	//!	Disable copy assignment.
		TerrainPhysics&	operator=( const TerrainPhysics& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		const PhysxPointer<physx::PxRigidStatic>	_actor;
		const AssetViews::TerrainAsset*				_asset;
	};

}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2