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
//	(6326) MSVC doesn't like some of the compile-time constant comparison PhysX does. We can't fix this, but we can at least disable the warning.
ET_PUSH_MSVC_WARNING_STATE(disable : 6326)
#include <foundation/PxTransform.h>
ET_POP_MSVC_WARNING_STATE()
//------------------------------------------------------------------//

namespace physx {
class PxArticulation;
class PxAggregate;
class PxScene;
} // namespace physx

namespace Eldritch2 { namespace Physics { namespace PhysX { namespace AssetViews {

	class ArmatureAsset : public Assets::Asset {
		// - TYPE PUBLISHING ---------------------------------

	public:
		struct ConeTwistConstraint {
			float32 coneX;
			float32 coneY;
		};

		// ---

	public:
		struct SliderConstraint {
			float32 distance;
		};

		// ---

	public:
		struct Joint {
			physx::PxTransform bindPose;
			uint8              parentIndex;

			union {
				ConeTwistConstraint coneTwist;
				SliderConstraint    slider;
			} constraint;
		};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		ArmatureAsset(const ArmatureAsset&) = delete;
		//!	Constructs this @ref ArmatureAsset instance.
		ArmatureAsset(StringView path);

		~ArmatureAsset() override = default;

		// ---------------------------------------------------

	public:
		PhysxPointer<physx::PxArticulation> CreateArticulation(physx::PxScene& scene) const;

		// ---------------------------------------------------

	public:
		ErrorCode BindResources(const Builder& builder) override;

		void FreeResources() override;

		// ---------------------------------------------------

	public:
		static ETPureFunctionHint StringView GetExtension() ETNoexceptHint;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		ArmatureAsset& operator=(const ArmatureAsset&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		ArrayList<Joint> _joints;
	};

}}}} // namespace Eldritch2::Physics::PhysX::AssetViews
