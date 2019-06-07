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
#include <Animation/AnimationApi.hpp>
#include <Core/Asset.hpp>
//------------------------------------------------------------------//
//	(6326) MSVC doesn't like some of the compile-time constant comparison PhysX does. We can't fix this, but we can at least disable the warning.
ET_PUSH_MSVC_WARNING_STATE(disable : 6326)
#include <cloth/PxClothTypes.h>
#include <PxShape.h>
ET_POP_MSVC_WARNING_STATE()
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Physics { namespace PhysX { namespace AssetViews {

	class PhysicsAsset : public Core::Asset {
		// - TYPE PUBLISHING ---------------------------------

	public:
		class RigidShape {
			// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref RigidShape instance.
			/*!	@param[in] shape PhysX shape describing the geometry, positioning and material data for the rigid body.
				@param[in] linkIndex Index of the actor the shape should be attached to within an aggregate. */
			RigidShape(PhysxPointer<physx::PxShape> shape, Animation::BoneIndex bone) ETNoexceptHint;
			//!	Disable copy construction.
			RigidShape(const RigidShape&) = delete;
			//!	Constructs this @ref RigidShape instance.
			RigidShape(RigidShape&&) ETNoexceptHint = default;
			//!	Constructs this @ref RigidShape instance.
			RigidShape() ETNoexceptHint = default;

			~RigidShape() = default;

			// ---------------------------------------------------

			//!	Disable copy construction.
			RigidShape& operator=(const RigidShape&) = delete;

			// - DATA MEMBERS ------------------------------------

		public:
			PhysxPointer<physx::PxShape> shape;
			Animation::BoneIndex         bone;
		};

		// ---

	public:
		class ClothShape {
			// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref ClothShape instance.
			ClothShape(PhysxPointer<PxClothFabric> fabric, ArrayList<PxClothParticle> particles) ETNoexceptHint;
			//!	Disable copy construction.
			ClothShape(const ClothShape&) = delete;
			//!	Constructs this @ref ClothShape instance.
			ClothShape(ClothShape&&) ETNoexceptHint = default;
			//!	Constructs this @ref ClothShape instance.
			ClothShape() ETNoexceptHint = default;

			~ClothShape() = default;

			// ---------------------------------------------------

			//!	Disable copy construction.
			ClothShape& operator=(const ClothShape&) = delete;

			// ---------------------------------------------------

		public:
			PhysxPointer<PxClothFabric> fabric;
			ArrayList<PxClothParticle>  particles;
		};

		// ---

	public:
		using RigidShapeList = ArrayList<RigidShape>;
		using ClothShapeList = ArrayList<ClothShape>;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		PhysicsAsset(const PhysicsAsset&) = delete;
		//!	Constructs this @ref PhysicsAsset instance.
		/*!	@param[in] path UTF-8-encoded character sequence containing the file system path to the resource. */
		PhysicsAsset(StringSpan path) ETNoexceptHint;

		~PhysicsAsset() override = default;

		// ---------------------------------------------------

	public:
		RigidShapeList::ConstSliceType GetRigidShapes() const ETNoexceptHint;

		ClothShapeList::ConstSliceType GetClothShapes() const ETNoexceptHint;

		// ---------------------------------------------------

	public:
		Result BindResources(Logging::Log& log, const Core::AssetBuilder& builder) override;

		void FreeResources() ETNoexceptHint override;

		// ---------------------------------------------------

	public:
		static ETPureFunctionHint StringSpan GetExtension() ETNoexceptHint;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		PhysicsAsset& operator=(const PhysicsAsset&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		RigidShapeList _rigidShapes;
		ClothShapeList _clothShapes;
	};

}}}} // namespace Eldritch2::Physics::PhysX::AssetViews
