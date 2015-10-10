/*==================================================================*\
  ResourceViewFactoryPublishingInitializationVisitor.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  �2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Packages/ResourceViewFactoryPublishingInitializationVisitor.hpp>
#include <Packages/ContentLibrary.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::FileSystem;
using namespace ::Eldritch2::Utility;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace FileSystem {

	ResourceViewFactoryPublishingInitializationVisitor::ResourceViewFactoryPublishingInitializationVisitor( ContentLibrary& contentLibrary ) : _contentLibrary( contentLibrary ) {}

// ---------------------------------------------------

	ResourceViewFactoryPublishingInitializationVisitor& ResourceViewFactoryPublishingInitializationVisitor::PublishFactory( const UTF8Char* const className, void* const parameter, ResultPair<ResourceView> (*factory)( Allocator&, const ResourceView::Initializer&, void* ) ) {
		using ResourceViewFactoryLibrary	= decltype(_contentLibrary._resourceViewFactoryCollection);
		using MappedType					= ResourceViewFactoryLibrary::MappedType;
		using KeyType						= ResourceViewFactoryLibrary::KeyType;

	// ---

		auto&		factoryCollection( _contentLibrary._resourceViewFactoryCollection );
		const auto	key( KeyType( className, FindEndOfString( className ) ) );
		const auto	candidate( factoryCollection.Find( key ) );

		// This is a little yuck, but the default operator[] will just default-construct the element and we explicitly don't want that.
		auto&		destination( (candidate != factoryCollection.End() ? candidate : factoryCollection.Insert( { key, MappedType( 1u, _contentLibrary._allocator, UTF8L("Resource View") ) } ).first)->second );

		destination.PushBack( { factory, parameter } );

		return *this;
	}

}	// namespace FileSystem
}	// namespace Eldritch2
