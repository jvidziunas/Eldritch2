/*==================================================================*\
  TerrainView.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/PhysX/AssetViews/TerrainView.hpp>
#include <Utility/Containers/Range.hpp>
#include <Assets/AssetLibrary.hpp>
#include <Utility/ErrorCode.hpp>
//------------------------------------------------------------------//
#if ET_COMPILER_IS_MSVC
#	pragma warning( push )
//	(6326) MSVC doesn't like some of the compile-time constant comparison PhysX does. We can't fix this, but we can at least disable the warning.
#	pragma warning( disable : 6326 )
#endif
#include <geometry/PxHeightFieldDesc.h>
#include <geometry/PxHeightField.h>
#include <cooking/PxCooking.h>
#include <PxRigidActor.h>
#include <PxMaterial.h>
#include <PxPhysics.h>
#if ET_COMPILER_IS_MSVC
#	pragma warning( pop )
#endif
//------------------------------------------------------------------//

using namespace ::Eldritch2::Assets;
using namespace ::physx;

namespace Eldritch2 {
namespace Physics {
namespace PhysX {
namespace AssetViews {

	TerrainView::TerrainView( const Utf8Char* const name, UniquePointer<PxShape> shape ) : AssetView( name ), _shape( eastl::move( shape ) ) {}

// ---------------------------------------------------

	Result<Eldritch2::UniquePointer<AssetView>> TerrainView::CreateView( Allocator& allocator, PxCooking& cooking, PxPhysics& physics, const AssetLibrary& library, const Utf8Char* const name, Range<const char*> rawBytes ) {
		PxHeightFieldDesc	heightfieldDesc;

		heightfieldDesc.nbRows		= 0u;
		heightfieldDesc.nbColumns	= 0u;

		UniquePointer<PxHeightField>	heightfield( cooking.createHeightField( heightfieldDesc, physics.getPhysicsInsertionCallback() ) );
		if( !heightfield ) {
			return Error::Unspecified;
		}
		
		PxHeightFieldGeometry	heightfieldGeometry;
		heightfieldGeometry.heightField = heightfield.Get();

		UniquePointer<PxMaterial>	material;
		if( !heightfield ) {
			return Error::Unspecified;
		}

		UniquePointer<PxShape>	shape( physics.createShape( heightfieldGeometry, *material ) );

	//	Commit changes to the view.
		auto	resultView( MakeUnique<TerrainView>( allocator, name, eastl::move( shape ) ) );
		if( !resultView ) {
			return Error::OutOfMemory;
		}

		return eastl::move( resultView );
	}

// ---------------------------------------------------

	ETPureFunctionHint Utf8Literal TerrainView::GetExtension() {
		return ".E2Terrain";
	}

// ---------------------------------------------------

	void TerrainView::AttachToActor( PxRigidActor& actor ) const {
		actor.attachShape( *_shape );
	}

// ---------------------------------------------------

	void TerrainView::DetachFromActor( PxRigidActor& actor ) const {
		actor.detachShape( *_shape );
	}

}	// namespace AssetViews
}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2
