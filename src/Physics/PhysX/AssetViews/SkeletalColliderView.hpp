/*==================================================================*\
  SkeletalColliderView.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/PhysX/SmartPointers.hpp>
#include <Animation/AnimationTypes.hpp>
#include <Utility/UniquePointer.hpp>
#include <Utility/StringLiteral.hpp>
#include <Utility/MPL/Compiler.hpp>
#include <Assets/AssetView.hpp>
//------------------------------------------------------------------//
#if ET_COMPILER_IS_MSVC
#	pragma warning( push )
//	(6326) MSVC doesn't like some of the compile-time constant comparison PhysX does. We can't fix this, but we can at least disable the warning.
#	pragma warning( disable : 6326 )
#endif
#include <PxShape.h>
#if ET_COMPILER_IS_MSVC
#	pragma warning( pop )
#endif
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Assets {
		class	AssetLibrary;
	}

	template <typename>
	class	Result;
	template <typename>
	class	Range;
}

namespace physx {
	class	PxArticulation;
	class	PxAggregate;
}

namespace Eldritch2 {
namespace Physics {
namespace PhysX {
namespace AssetViews {

	class SkeletalColliderView : public Assets::AssetView {
	// - TYPE PUBLISHING ---------------------------------

	public:
		class RigidShape {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
		//!	Constructs this @ref RigidBody instance.
		/*!	@param[in] shape PhysX shape describing the geometry, positioning and material data for the rigid body.
			@param[in] actorIndex Index of the actor the shape should be attached to within an aggregate. */
			RigidShape( PhysX::UniquePointer<physx::PxShape> shape, Animation::BoneIndex localBoneIndex );
		//!	Disable copying.
			RigidShape( const RigidShape& ) = delete;

			~RigidShape() = default;

		// - DATA MEMBERS ------------------------------------

		public:
			PhysX::UniquePointer<physx::PxShape>	physicsShape;
			Animation::BoneIndex					localBoneIndex;
		};

	// ---

	public:
		class ClothShape {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
		//!	Constructs this @ref ClothShape instance.
			ClothShape() = default;

			~ClothShape() = default;
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref SkeletalColliderView instance.
	/*!	@param[in] name Null-terminated UTF-8 encoded character sequence containing the name of the resource.
		@param[in] rigidShapes */
		SkeletalColliderView( const Eldritch2::Utf8Char* const name, Eldritch2::UniquePointer<RigidShape[]> rigidShapes, Eldritch2::UniquePointer<ClothShape[]> clothShapes );
	//!	Disable copying.
		SkeletalColliderView( const SkeletalColliderView& ) = delete;

		~SkeletalColliderView() = default;

	// ---------------------------------------------------

	public:
		void	AttachTo( physx::PxArticulation& articulation ) const;
		void	AttachTo( physx::PxAggregate& aggregate ) const;

		void	DetachFrom( physx::PxArticulation& articulation ) const;
		void	DetachFrom( physx::PxAggregate& aggregate ) const;

	// ---------------------------------------------------

	public:
		static Eldritch2::Result<Eldritch2::UniquePointer<Assets::AssetView>>	CreateView( Eldritch2::Allocator& allocator, physx::PxPhysics& physics, const Assets::AssetLibrary& library, const Eldritch2::Utf8Char* const name, Eldritch2::Range<const char*> assetBytes );

		static ETPureFunctionHint Eldritch2::Utf8Literal						GetExtension();

	// ---------------------------------------------------

	//!	Disable copying.
		SkeletalColliderView&	operator=( const SkeletalColliderView& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		Eldritch2::UniquePointer<RigidShape[]>	_rigidShapes;
		Eldritch2::UniquePointer<ClothShape[]>	_clothShapes;
	};

}	// namespace AssetViews
}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2