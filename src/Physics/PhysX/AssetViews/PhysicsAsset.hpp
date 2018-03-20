/*==================================================================*\
  PhysicsAsset.hpp
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
#include <Animation/AnimationTypes.hpp>
#include <Assets/Asset.hpp>
//------------------------------------------------------------------//
ET_PUSH_COMPILER_WARNING_STATE()
//	(6326) MSVC doesn't like some of the compile-time constant comparison PhysX does. We can't fix this, but we can at least disable the warning.
	ET_SET_MSVC_WARNING_STATE( disable : 6326 )
#	include <cloth/PxCloth.h>
#	include <PxShape.h>
ET_POP_COMPILER_WARNING_STATE()
//------------------------------------------------------------------//

namespace physx {
	class	PxArticulation;
	class	PxAggregate;
}

namespace Eldritch2 {
namespace Physics {
namespace PhysX {
namespace AssetViews {

	class PhysicsAsset : public Assets::Asset {
	// - TYPE PUBLISHING ---------------------------------

	public:
		class RigidShape {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
		//!	Constructs this @ref RigidShape instance.
		/*!	@param[in] shape PhysX shape describing the geometry, positioning and material data for the rigid body.
			@param[in] linkIndex Index of the actor the shape should be attached to within an aggregate. */
			RigidShape( PhysxPointer<physx::PxShape> shape, Animation::BoneIndex linkIndex );
		//!	Disable copy construction.
			RigidShape( const RigidShape& ) = delete;
		//!	Constructs this @ref RigidShape instance.
			RigidShape( RigidShape&& ) ETNoexceptHint = default;
		//!	Constructs this @ref RigidShape instance.
			RigidShape() = default;

			~RigidShape() = default;

		// - DATA MEMBERS ------------------------------------

		public:
			PhysxPointer<physx::PxShape>	shape;
			Animation::BoneIndex			linkIndex;
		};

	// ---

	public:
		class ClothShape {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
		//!	Disable copy construction.
			ClothShape( const ClothShape& ) = delete;
		//!	Constructs this @ref ClothShape instance.
			ClothShape( ClothShape&& ) ETNoexceptHint = default;
		//!	Constructs this @ref ClothShape instance.
			ClothShape() = default;

			~ClothShape() = default;
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref PhysicsAsset instance.
	/*!	@param[in] filePath Null-terminated, UTF-8-encoded character sequence containing the file system path to the resource. */
		PhysicsAsset( const Utf8Char* const filePath );
	//!	Disable copy construction.
		PhysicsAsset( const PhysicsAsset& ) = delete;

		~PhysicsAsset() override = default;

	// ---------------------------------------------------

	public:
		const ArrayList<RigidShape>&	GetRigidShapes() const;

		const ArrayList<ClothShape>&	GetClothShapes() const;

	// ---------------------------------------------------

	public:
		ErrorCode	BindResources( const Builder& builder ) override;

		void		FreeResources() override;

	// ---------------------------------------------------

	//!	Disable copy assignment.
		PhysicsAsset&	operator=( const PhysicsAsset& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		ArrayList<RigidShape>	_rigidShapes;
		ArrayList<ClothShape>	_clothShapes;
	};

}	// namespace AssetViews
}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2