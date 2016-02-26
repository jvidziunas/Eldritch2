/*==================================================================*\
  EngineService.TerrainViewFactory.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/PhysX/TerrainResourceView.hpp>
#include <Physics/PhysX/EngineService.hpp>
#include <Utility/Memory/InstanceNew.hpp>
#include <Utility/Containers/Range.hpp>
#include <Utility/ErrorCode.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Configuration;
using namespace ::Eldritch2::FileSystem;
using namespace ::Eldritch2::Physics;
using namespace ::Eldritch2;

using namespace ::physx;

namespace Eldritch2 {
namespace Physics {
namespace PhysX {

	void EngineService::TerrainViewFactory::AcceptInitializationVisitor( ConfigurationPublishingInitializationVisitor& /*visitor*/ ) {}

// ---------------------------------------------------

	ETNoAliasHint const UTF8Char* const EngineService::TerrainViewFactory::GetSerializedDataTag() {
		return TerrainResourceView::GetSerializedDataTag();
	}

// ---------------------------------------------------

	ErrorCode EngineService::TerrainViewFactory::AllocateResourceView( Allocator& allocator, ContentLibrary& contentLibrary, ContentPackage& package, const UTF8Char* const name, const Range<const char*> sourceAsset ) {
		if( const auto view = new(allocator, Allocator::AllocationDuration::Normal) TerrainResourceView( contentLibrary, package, name, allocator ) ) {
			return view->UpdateFromByteStream( sourceAsset );
		}

		return Error::OutOfMemory;
	}

}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2



