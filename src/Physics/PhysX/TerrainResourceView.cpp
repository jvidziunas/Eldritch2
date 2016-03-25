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

	TerrainResourceView::TerrainResourceView( const UTF8Char* const name ) : ResourceView( name ) {}

// ---------------------------------------------------

	TerrainResourceView::~TerrainResourceView() {}

// ---------------------------------------------------

	ErrorCode TerrainResourceView::AttachToPackage( const Range<const char*> /*bytes*/, ContentPackage& /*package*/, ContentLibrary& library ) {
		PxHeightFieldDesc	descriptor;

		PublishToLibraryAs<decltype(*this)>( library );

		return Error::None;
	}

// ---------------------------------------------------

	void TerrainResourceView::DetachFromPackage( ContentPackage& /*package*/, ContentLibrary& library ) const {
		RemoveFromLibraryAs<decltype(*this)>( library );
	}

// ---------------------------------------------------

	ETNoAliasHint const UTF8Char* const TerrainResourceView::GetSerializedDataTag() {
		return UTF8L("E2Terrain");
	}
}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2
