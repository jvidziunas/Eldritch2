/*==================================================================*\
  Physics.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/PhysX/PhysXPointer.hpp>
#include <Animation/AnimationApi.hpp>
//------------------------------------------------------------------//
#include <PxAggregate.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Physics { namespace PhysX { namespace AssetViews {
	class PhysicsAsset;
}}}} // namespace Eldritch2::Physics::PhysX::AssetViews

namespace Eldritch2 { namespace Physics { namespace PhysX {

	class PhysicsClip : public Animation::Clip {
		// - TYPE PUBLISHING ---------------------------------

	public:
		enum : uint32 { TracksPerBone = 8u };

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref PhysicsClip instance.
		PhysicsClip(const physx::PxAggregate& aggregate);
		//!	Constructs this @ref PhysicsClip instance.
		PhysicsClip(const PhysicsClip&) = default;

		~PhysicsClip() = default;

		// ---------------------------------------------------

	public:
		void FetchKnots(Animation::KnotCache& knots, Animation::BoneIndex maximumBone, uint64 time) override;

		void Attach(Animation::KnotCache& knots) override;

		// - DATA MEMBERS ------------------------------------

	public:
		const physx::PxAggregate* _aggregate;
	};

	// ---

	class Physics {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref Physics instance.
		Physics(const AssetViews::PhysicsAsset& asset, PhysicsClip& clip) ETNoexceptHint;
		//! Disable copy construction.
		Physics(const Physics&) = delete;

		~Physics() = default;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		Physics& operator=(const Physics&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		const AssetViews::PhysicsAsset* _asset;
		PhysicsClip*                    _clip;
	};

}}} // namespace Eldritch2::Physics::PhysX
