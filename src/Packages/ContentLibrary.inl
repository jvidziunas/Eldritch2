/*==================================================================*\
  ContentLibrary.inl
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Concurrency/Lock.hpp>
#include <Utility/Containers/Range.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace FileSystem {

	ETInlineHint ContentLibrary::ResourceViewKey::ResourceViewKey( const ::Eldritch2::UTF8Char* const resourceName, const ::std::type_info* resourceType ) : Pair<const ::Eldritch2::UTF8Char*, const ::std::type_info*>( resourceName, resourceType ) {}

// ---------------------------------------------------

	template <typename View>
	const View& ContentLibrary::ResolveViewByName( const UTF8Char* const name, const View& defaultView ) const {
		static_assert( ::std::is_base_of<FileSystem::ResourceView, View>::value, "ResolveViewByName() must be used to convert to ResourceView-derived types!" );

	// ---

		Utility::ScopedReaderLock	_( *_resourceViewDirectoryMutex );
		const auto					candidate( _resourceViewDirectory.Find( { name, &typeid(defaultView) } ) );

		return (candidate != _resourceViewDirectory.End()) ? (*static_cast<const View*>(candidate->second)) : defaultView;
	}

// ---------------------------------------------------

	ETInlineHint FileSystem::ContentProvider& ContentLibrary::GetContentProvider() const {
		return _contentProvider;
	}

// ---------------------------------------------------

	ETInlineHint ::Eldritch2::Range<ContentLibrary::ResourceViewFactoryCollection::Iterator> ContentLibrary::GetFactoriesForResourceType( const ::Eldritch2::UTF8Char* const resourceTypeName ) {
		const auto	factoryCollectionCandidate( _resourceFactoryDirectory.Find( resourceTypeName ) );
		
		if( factoryCollectionCandidate != _resourceFactoryDirectory.End() ) {
			return { factoryCollectionCandidate->second.Begin(), factoryCollectionCandidate->second.End() };
		}

		return { _nullFactoryCollection.Begin(), _nullFactoryCollection.End() };
	}

// ---------------------------------------------------

	ETInlineHint ETNoAliasHint size_t GetHashCode( const ContentLibrary::ResourceViewKey& key, const size_t seed ) {
		return key.second->hash_code() * ::Eldritch2::Hash<const ::Eldritch2::UTF8Char*>()( key.first, seed );
	}

// ---------------------------------------------------

	ETInlineHint ETNoAliasHint bool operator==( const ContentLibrary::ResourceViewKey& left, const ContentLibrary::ResourceViewKey& right ) {
		return (*left.second == *right.second) && ::Eldritch2::EqualityCompareString( left.first, right.first );
	}

}	// namespace FileSystem
}	// namespace Eldritch2