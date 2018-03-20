/*==================================================================*\
  ArmatureAsset.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/PhysX/AssetViews/ArmatureAsset.hpp>
//------------------------------------------------------------------//
#include <FlatBuffers/Armature_generated.h>
//------------------------------------------------------------------//
ET_PUSH_COMPILER_WARNING_STATE()
//	(6326) MSVC doesn't like some of the compile-time constant comparison PhysX does. We can't fix this, but we can at least disable the warning.
	ET_SET_MSVC_WARNING_STATE( disable : 6326 )
#	include <PxArticulation.h>
#	include <PxPhysics.h>
#	include <PxScene.h>
ET_POP_COMPILER_WARNING_STATE()
//------------------------------------------------------------------//

using namespace ::Eldritch2::Animation::FlatBuffers;
using namespace ::Eldritch2::Logging;
using namespace ::Eldritch2::Assets;
using namespace ::flatbuffers;
using namespace ::physx;

namespace Eldritch2 {
namespace Physics {
namespace PhysX {
namespace AssetViews {

	ArmatureAsset::ArmatureAsset( const Utf8Char* const filePath ) : Asset( filePath ), _joints( MallocAllocator( "PhysX Armature Asset Joint Definition Allocator" ) ) {}

// ---------------------------------------------------

	PhysxPointer<PxArticulation> ArmatureAsset::CreateArticulation( PxScene& scene ) const {
		PxArticulation* const	articulation( scene.getPhysics().createArticulation() );
		auto					links( static_cast<PxArticulationLink**>(_alloca( sizeof(PxArticulationLink*) * (_joints.GetSize() + 1) )) );
		uint32					linkIndex( 1u );

		for (const Joint& joint : _joints) {
			links[linkIndex++] = articulation->createLink( links[joint.parentIndex], joint.bindPose );
		}

		scene.addArticulation( *articulation );

		return articulation;
	}

// ---------------------------------------------------

	ErrorCode ArmatureAsset::BindResources( const Builder& builder ) {
		const auto&	bytes( builder.GetRawBytes() );

	//	Ensure we're working with data that can plausibly represent an armature.
		Verifier verifier( reinterpret_cast<const uint8_t*>(bytes.Begin()), bytes.GetSize() );
		if (!VerifyArmatureBuffer( verifier )) {
			builder.WriteLog( MessageType::Error, "{} is malformed." UTF8_NEWLINE, GetPath() );
			return Error::InvalidParameter;
		}

		const Armature&	asset( *GetArmature( bytes.Begin() ) );

		ArrayList<Joint>	joints( _joints.GetAllocator() );

		Swap( _joints, joints );

		return Error::None;
	}

// ---------------------------------------------------

	void ArmatureAsset::FreeResources() {
		_joints.Clear( ReleaseMemorySemantics() );
	}

}	// namespace AssetViews
}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2