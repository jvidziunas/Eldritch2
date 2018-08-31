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
//	(6326) MSVC doesn't like some of the compile-time constant comparison PhysX does. We can't fix this, but we can at least disable the warning.
ET_PUSH_MSVC_WARNING_STATE(disable : 6326)
#include <cloth/PxCloth.h>
#include <PxShape.h>
ET_POP_MSVC_WARNING_STATE()
//------------------------------------------------------------------//

namespace physx {
class PxArticulation;
class PxAggregate;
} // namespace physx

namespace Eldritch2 { namespace Physics { namespace PhysX { namespace AssetViews {

	class PhysicsAsset : public Assets::Asset {
		// - TYPE PUBLISHING ---------------------------------

	public:
		class RigidShape {
			// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref RigidShape instance.
			/*!	@param[in] shape PhysX shape describing the geometry, positioning and material data for the rigid body.
			@param[in] linkIndex Index of the actor the shape should be attached to within an aggregate. */
			RigidShape(PhysxPointer<physx::PxShape> shape, Animation::BoneIndex linkIndex);
			//!	Disable copy construction.
			RigidShape(const RigidShape&) = delete;
			//!	Constructs this @ref RigidShape instance.
			RigidShape(RigidShape&&) ETNoexceptHint = default;
			//!	Constructs this @ref RigidShape instance.
			RigidShape() ETNoexceptHint = default;

			~RigidShape() = default;

			// - DATA MEMBERS ------------------------------------

		public:
			PhysxPointer<physx::PxShape> shape;
			Animation::BoneIndex         linkIndex;
		};

		// ---

	public:
		class ClothShape {
			// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Disable copy construction.
			ClothShape(const ClothShape&) = delete;
			//!	Constructs this @ref ClothShape instance.
			ClothShape(ClothShape&&) ETNoexceptHint = default;
			//!	Constructs this @ref ClothShape instance.
			ClothShape() ETNoexceptHint = default;

			~ClothShape() = default;
		};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		PhysicsAsset(const PhysicsAsset&) = delete;
		//!	Constructs this @ref PhysicsAsset instance.
		/*!	@param[in] path UTF-8-encoded character sequence containing the file system path to the resource. */
		PhysicsAsset(StringView path);

		~PhysicsAsset() override = default;

		// ---------------------------------------------------

	public:
		const ArrayList<RigidShape>& GetRigidShapes() const;

		const ArrayList<ClothShape>& GetClothShapes() const;

		// ---------------------------------------------------

	public:
		ErrorCode BindResources(const Builder& builder) override;

		void FreeResources() override;

		// ---------------------------------------------------

	public:
		static ETPureFunctionHint StringView GetExtension() ETNoexceptHint;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		PhysicsAsset& operator=(const PhysicsAsset&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		ArrayList<RigidShape> _rigidShapes;
		ArrayList<ClothShape> _clothShapes;
	};

}}}} // namespace Eldritch2::Physics::PhysX::AssetViews
