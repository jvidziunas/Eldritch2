/*==================================================================*\
  EngineService.MeshViewFactory.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/BulletDynamics/MeshResourceView.hpp>
#include <Physics/BulletDynamics/EngineService.hpp>
#include <Utility/Memory/InstanceNew.hpp>
#include <Utility/Containers/Range.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Configuration;
using namespace ::Eldritch2::FileSystem;
using namespace ::Eldritch2::Physics;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Physics {
namespace BulletDynamics {

	void EngineService::MeshViewFactory::AcceptInitializationVisitor( ConfigurationPublishingInitializationVisitor& /*visitor*/ ) {}

// ---------------------------------------------------

	ErrorCode EngineService::MeshViewFactory::AllocateResourceView( Allocator& allocator, ContentLibrary& contentLibrary, ContentPackage& package, const UTF8Char* const name, const Range<const char*> sourceAsset ) {
		if( auto view = new(allocator, Allocator::AllocationDuration::Normal) MeshResourceView( contentLibrary, package, name, allocator ) ) {
			return view->UpdateFromByteStream( sourceAsset );
		}

		return Error::OutOfMemory;
	}

}	// namespace BulletDynamics
}	// namespace Physics
}	// namespace Eldritch2