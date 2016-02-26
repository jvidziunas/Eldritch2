/*==================================================================*\
  TerrainResourceView.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/PhysX/TerrainResourceView.hpp>
#include <Utility/Containers/Range.hpp>
#include <Utility/ErrorCode.hpp>
//------------------------------------------------------------------//
#include <geometry/PxHeightFieldDesc.h>
#include <geometry/PxHeightField.h>
#include <PxShape.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2::FileSystem;
using namespace ::Eldritch2::Physics;
using namespace ::Eldritch2;

using namespace ::physx;

namespace Eldritch2 {
namespace Physics {
namespace PhysX {

	TerrainResourceView::TerrainResourceView( ContentLibrary& library, ContentPackage& package, const UTF8Char* const name, Allocator& allocator ) : ResourceView( library, package, name, allocator ) {}

// ---------------------------------------------------

	TerrainResourceView::~TerrainResourceView() {}

// ---------------------------------------------------

	ErrorCode TerrainResourceView::UpdateFromByteStream( const Range<const char*> bytes ) {
		PxHeightFieldDesc	descriptor;

		return Error::UnsupportedOperation;
	}

// ---------------------------------------------------

	ETNoAliasHint const UTF8Char* const TerrainResourceView::GetSerializedDataTag() {
		return UTF8L("e2Terrain");
	}

}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2
