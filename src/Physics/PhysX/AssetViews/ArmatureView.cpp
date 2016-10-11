/*==================================================================*\
  ArmatureView.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/PhysX/AssetViews/ArmatureView.hpp>
#include <Utility/Containers/Range.hpp>
#include <Assets/AssetLibrary.hpp>
#include <Utility/ErrorCode.hpp>
//------------------------------------------------------------------//
#include <FlatBuffers/Armature_generated.h>
//------------------------------------------------------------------//
#if ET_COMPILER_IS_MSVC
#	pragma warning( push )
//	(6326) MSVC doesn't like some of the compile-time constant comparison PhysX does. We can't fix this, but we can at least disable the warning.
#	pragma warning( disable : 6326 )
#endif
#include <PxRigidDynamic.h>
#include <PxArticulation.h>
#include <PxAggregate.h>
#include <PxPhysics.h>
#if ET_COMPILER_IS_MSVC
#	pragma warning( pop )
#endif
//------------------------------------------------------------------//

using namespace ::Eldritch2::Logging;
using namespace ::Eldritch2::Assets;
using namespace ::flatbuffers;
using namespace ::physx;

namespace Eldritch2 {
namespace Physics {
namespace PhysX {
namespace AssetViews {

	ArmatureView::ArmatureView( const Utf8Char* const name, Eldritch2::UniquePointer<PxTransform[]> bindPose, Eldritch2::UniquePointer<JointDefinition[]> joints ) : AssetView( name ),
																																									 _bindPose( eastl::move( bindPose ) ),
																																									 _joints( eastl::move( joints ) ) {}

// ---------------------------------------------------

	UniquePointer<PxArticulation> ArmatureView::CreateArticulation( PxPhysics& physics ) const {
		PxArticulation* const	articulation( physics.createArticulation() );

		for( const auto& boneTransform : _bindPose ) {
			const auto	link( articulation->createLink( nullptr, boneTransform ) );
		}

		return articulation;
	}

// ---------------------------------------------------

	UniquePointer<PxAggregate> ArmatureView::CreateAggregate( PxPhysics& physics, bool selfCollisions ) const {
		PxAggregate* const	aggregate( physics.createAggregate( static_cast<PxU32>(_bindPose.GetSize()), selfCollisions ) );

		for( const auto& boneTransform : _bindPose ) {
			PxRigidDynamic* const	boneActor( physics.createRigidDynamic( boneTransform ) );

			aggregate->addActor( *boneActor );
		}

		for( const auto& joint : _joints ) {
		}

		return aggregate;
	}

// ---------------------------------------------------

	Result<Eldritch2::UniquePointer<AssetView>> ArmatureView::CreateView( Allocator& allocator, const AssetLibrary& library, const Utf8Char* const name, Range<const char*> rawBytes ) {
		Verifier	verifier( reinterpret_cast<const uint8_t*>( rawBytes.Begin() ), rawBytes.GetSize() );

	//	Ensure we're working with data that can plausibly represent an armature.
		if( !Animation::FlatBuffers::ArmatureBufferHasIdentifier( rawBytes.Begin() ) || !Animation::FlatBuffers::VerifyArmatureBuffer( verifier ) ) {
			library.GetLog()( MessageSeverity::Error, "Armature '{}' is malformed." ET_UTF8_NEWLINE_LITERAL, name );
			return Error::InvalidParameter;
		}

		const Animation::FlatBuffers::Armature&	armatureDefinition( *Animation::FlatBuffers::GetArmature( rawBytes.Begin() ) );

		Eldritch2::UniquePointer<PxTransform[]>	bindPose;
#	if 0
		for( const auto& boneDefinition : *armatureDefinition.Bones() ) {
			bindPose.EmplaceBack();
		}
#	endif

		Eldritch2::UniquePointer<JointDefinition[]>	joints;
#	if 0
		for( const auto& jointDefinition : *armatureDefinition.Joints() ) {
			bindPose.EmplaceBack();
		}
#	endif

		auto	resultView( MakeUnique<ArmatureView>( allocator, name, eastl::move( bindPose ), eastl::move( joints ) ) );
		if( !resultView ) {
			return Error::OutOfMemory;
		}
		
		return eastl::move( resultView );
	}

// ---------------------------------------------------

	ETPureFunctionHint Utf8Literal ArmatureView::GetExtension() {
		return Utf8Literal( Animation::FlatBuffers::ArmatureExtension() );
	}

}	// namespace AssetViews
}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2