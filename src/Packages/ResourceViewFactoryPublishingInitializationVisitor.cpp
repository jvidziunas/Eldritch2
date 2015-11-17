/*==================================================================*\
  ResourceViewFactoryPublishingInitializationVisitor.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
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
using namespace ::std;

namespace Eldritch2 {
namespace FileSystem {

	ResourceViewFactoryPublishingInitializationVisitor::ResourceViewFactoryPublishingInitializationVisitor( ContentLibrary& contentLibrary ) : _contentLibrary( contentLibrary ) {}

// ---------------------------------------------------

	ResourceViewFactoryPublishingInitializationVisitor& ResourceViewFactoryPublishingInitializationVisitor::PublishFactory( const UTF8Char* const className, void* const parameter, FactoryFunctionPointer factory ) {
		using ResourceViewFactoryLibrary	= decltype(_contentLibrary._resourceViewFactoryCollection);
		using MappedType					= ResourceViewFactoryLibrary::MappedType;
		using ValueType						= ResourceViewFactoryLibrary::ValueType;
		using KeyType						= ResourceViewFactoryLibrary::KeyType;

	// ---

		KeyType	key( className, FindEndOfString( className ) );
		auto&	factoryCollection( _contentLibrary._resourceViewFactoryCollection );
		auto	candidate( factoryCollection.Find( key ) );

		// This is a little yuck, but the default operator[] will just default-construct the element and we explicitly don't want that.
		if( candidate == factoryCollection.End() ) {
			MappedType	newCollection( { _contentLibrary._allocator, UTF8L("Resource View Factory Collection Allocator") } );
			
			candidate = factoryCollection.Insert( { move( key ), move( newCollection ) } ).first;
		}

		candidate->second.PushBack( { factory, parameter } );

		return *this;
	}

// ---------------------------------------------------

	ResourceViewFactoryPublishingInitializationVisitor&	ResourceViewFactoryPublishingInitializationVisitor::PublishFactory( const ::Eldritch2::UTF8Char* const className, FactoryFunctionPointer factory ) {
		return PublishFactory( className, nullptr, factory );
	}

}	// namespace FileSystem
}	// namespace Eldritch2
