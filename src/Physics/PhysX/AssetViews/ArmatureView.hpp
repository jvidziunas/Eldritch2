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
#include <Physics/PhysX/SmartPointers.hpp>
#include <Utility/Mpl/FloatTypes.hpp>
#include <Utility/StringLiteral.hpp>
#include <Assets/AssetView.hpp>
//------------------------------------------------------------------//
#if ET_COMPILER_IS_MSVC
#	pragma warning( push )
//	(6326) MSVC doesn't like some of the compile-time constant comparison PhysX does. We can't fix this, but we can at least disable the warning.
#	pragma warning( disable : 6326 )
#endif
#include <foundation/PxTransform.h>
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
	class	PxPhysics;
}

namespace Eldritch2 {
namespace Physics {
namespace PhysX {
namespace AssetViews {

	class ArmatureView : public Assets::AssetView {
	// - TYPE PUBLISHING ---------------------------------

	private:
		struct JointDefinition {
			Eldritch2::uint8			bone0Index;
			Eldritch2::uint8			bone1Index;

			union {
				struct {
					Eldritch2::float32	coneX;
					Eldritch2::float32	coneY;
				}						coneTwist;
				struct {
					Eldritch2::float32	distance;
				}						slider;
			}							constraintData;
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref ArmatureView instance.
		ArmatureView( const Eldritch2::Utf8Char* const name, Eldritch2::UniquePointer<physx::PxTransform[]> bindPose, Eldritch2::UniquePointer<JointDefinition[]> joints );
	//!	Disable copying.
		ArmatureView( const ArmatureView& ) = delete;

		~ArmatureView() = default;

	// ---------------------------------------------------

	public:
		static Eldritch2::Result<Eldritch2::UniquePointer<AssetView>>	CreateView( Eldritch2::Allocator& allocator, const Assets::AssetLibrary& library, const Eldritch2::Utf8Char* const name, Eldritch2::Range<const char*> assetBytes );

		static ETPureFunctionHint Eldritch2::Utf8Literal				GetExtension();

	// ---------------------------------------------------

	public:
		PhysX::UniquePointer<physx::PxArticulation>	CreateArticulation( physx::PxPhysics& physics ) const;

		PhysX::UniquePointer<physx::PxAggregate>	CreateAggregate( physx::PxPhysics& physics, bool selfCollisions = false ) const;

	// - DATA MEMBERS ------------------------------------

	private:
		Eldritch2::UniquePointer<physx::PxTransform[]>	_bindPose;
		Eldritch2::UniquePointer<JointDefinition[]>		_joints;
	};

}	// namespace AssetViews
}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2