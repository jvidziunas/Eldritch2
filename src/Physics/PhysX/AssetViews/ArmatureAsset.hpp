/*==================================================================*\
  ArmatureResourceView.hpp
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
#include <Assets/Asset.hpp>
//------------------------------------------------------------------//
ET_PUSH_COMPILER_WARNING_STATE()
//	(6326) MSVC doesn't like some of the compile-time constant comparison PhysX does. We can't fix this, but we can at least disable the warning.
	ET_SET_MSVC_WARNING_STATE( disable : 6326 )
#include <foundation/PxTransform.h>
ET_POP_COMPILER_WARNING_STATE()
//------------------------------------------------------------------//

namespace physx {
	class	PxArticulation;
	class	PxAggregate;
	class	PxScene;
}

namespace Eldritch2 {
namespace Physics {
namespace PhysX {
namespace AssetViews {

	class ArmatureAsset : public Assets::Asset {
	// - TYPE PUBLISHING ---------------------------------

	public:
		struct ConeTwistConstraint {
			float32	coneX;
			float32	coneY;
		};

	// ---

	public:
		struct SliderConstraint {
			float32	distance;
		};

	// ---

	public:
		struct Joint {
			physx::PxTransform	bindPose;
			uint8				parentIndex;

			union {
				ConeTwistConstraint	coneTwist;
				SliderConstraint	slider;
			}					constraint;
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref ArmatureAsset instance.
		ArmatureAsset( const Utf8Char* const filePath );
	//!	Disable copy construction.
		ArmatureAsset( const ArmatureAsset& ) = delete;

		~ArmatureAsset() override = default;

	// ---------------------------------------------------

	public:
		PhysxPointer<physx::PxArticulation>	CreateArticulation( physx::PxScene& scene ) const;

	// ---------------------------------------------------

	public:
		ErrorCode	BindResources( const Builder& builder ) override;

		void		FreeResources() override;

	// - DATA MEMBERS ------------------------------------

	private:
		ArrayList<Joint>	_joints;
	};

}	// namespace AssetViews
}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2