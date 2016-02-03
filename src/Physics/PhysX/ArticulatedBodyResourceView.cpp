/*==================================================================*\
  ArticulatedBodyResourceView.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/PhysX/ArticulatedBodyResourceView.hpp>
#include <Utility/Containers/Range.hpp>
#include <Utility/ErrorCode.hpp>
//------------------------------------------------------------------//
#include <Physics/ArticulatedBody_generated.h>
//------------------------------------------------------------------//
#include <PxPhysics.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2::FileSystem;
using namespace ::Eldritch2::Physics;
using namespace ::Eldritch2;

using namespace ::physx;

namespace Eldritch2 {
namespace Physics {
namespace PhysX {

	ArticulatedBodyResourceView::Body::Body( UniquePointer<PxMaterial>&& material ) : material( ::std::move( material ) ) {}

// ---------------------------------------------------

	ArticulatedBodyResourceView::Body::Body( Body&& body ) : material( ::std::move( body.material ) ) {}

// ---------------------------------------------------

	ArticulatedBodyResourceView::ArticulatedBodyResourceView( ContentLibrary& owningLibrary, ContentPackage& package, const UTF8Char* const name, Allocator& nameAllocator, Allocator& bodyDefinitionAllocator ) : ResourceView( owningLibrary, package, name, nameAllocator ),
																																																				   _bodies( { bodyDefinitionAllocator, UTF8L("PhysX Articulated Body Resource View Body Definition Allocator") } ) {}

// ---------------------------------------------------

	ErrorCode ArticulatedBodyResourceView::UpdateFromByteStream( const Range<const char*> bytes ) {
		auto&	physics( PxGetPhysics() );
		auto	asset( FlatBuffers::GetArticulatedBody( bytes.first ) );

		_bodies.Clear();

		if( !asset || !asset->Bodies() ) {
			return Error::InvalidParameter;
		}

		_bodies.Reserve( asset->Bodies()->size() );

		for( auto definition( asset->Bodies()->begin() ), end( asset->Bodies()->end() ); definition != end; ++definition ) {
			UniquePointer<PxMaterial>	newMaterial( physics.createMaterial( definition->Friction(), definition->RollingFriction(), definition->Restitution() ) );

			if( !newMaterial ) {
				return Error::Unspecified;
			}

			_bodies.EmplaceBack( ::std::move( newMaterial ) );
		}

		return Error::None;
	}

// ---------------------------------------------------

	ETNoAliasHint const UTF8Char* const ArticulatedBodyResourceView::GetSerializedDataTag() {
		return FlatBuffers::ArticulatedBodyExtension();
	}

}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2