/*==================================================================*\
  EngineService.MeshViewFactory.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/PhysX/MeshResourceView.hpp>
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

	void EngineService::MeshViewFactory::AcceptInitializationVisitor( ConfigurationPublishingInitializationVisitor& /*visitor*/ ) {}

// ---------------------------------------------------

	ETNoAliasHint const UTF8Char* const EngineService::MeshViewFactory::GetSerializedDataTag() {
		return MeshResourceView::GetSerializedDataTag();
	}

// ---------------------------------------------------

	Result<ResourceView> EngineService::MeshViewFactory::AllocateResourceView( Allocator& allocator, const UTF8Char* const name ) const {
		if( auto view = new(allocator, Allocator::AllocationDuration::Normal) MeshResourceView( name, allocator ) ) {
			return { ::std::move( view ) };
		}

		return { Error::OutOfMemory };
	}

}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2



