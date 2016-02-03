/*==================================================================*\
  EngineService.ArticulatedBodyViewFactory.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/PhysX/ArticulatedBodyResourceView.hpp>
#include <Utility/Memory/InstanceDeleters.hpp>
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

	EngineService::ArticulatedBodyViewFactory::ArticulatedBodyViewFactory( Allocator& allocator ) : _allocator( allocator, UTF8L("PhysX Articulated Body View Factory Extra Data Allocator") ) {}

// ---------------------------------------------------

	void EngineService::ArticulatedBodyViewFactory::AcceptInitializationVisitor( ConfigurationPublishingInitializationVisitor& /*visitor*/ ) {}

// ---------------------------------------------------

	ETNoAliasHint const UTF8Char* const EngineService::ArticulatedBodyViewFactory::GetSerializedDataTag() {
		return ArticulatedBodyResourceView::GetSerializedDataTag();
	}

// ---------------------------------------------------

	ErrorCode EngineService::ArticulatedBodyViewFactory::AllocateResourceView( Allocator& allocator, ContentLibrary& contentLibrary, ContentPackage& package, const UTF8Char* const name, const Range<const char*> sourceAsset ) {
		if( const auto view = new(allocator, Allocator::AllocationDuration::Normal) ArticulatedBodyResourceView( contentLibrary, package, name, allocator, _allocator ) ) {
			return view->UpdateFromByteStream( sourceAsset );
		}

		return Error::OutOfMemory;
	}

}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2



