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

	ETInlineHint ContentLibrary::ResourceViewFactoryKey::ResourceViewFactoryKey( const ::Eldritch2::UTF8Char* const begin, const ::Eldritch2::UTF8Char* const end ) : ::std::pair<const ::Eldritch2::UTF8Char*, const ::Eldritch2::UTF8Char*>( begin, end ) {}

// ---------------------------------------------------

	ETInlineHint ETNoAliasHint size_t ContentLibrary::ResourceViewFactoryKey::Hash::operator()( const ResourceViewFactoryKey& key, const size_t seed ) const {
		return StringHash::operator()( key.first, key.second, seed );
	}

// ---------------------------------------------------

	ETInlineHint ETNoAliasHint size_t ContentLibrary::ResourceViewKey::Hash::operator()( const ResourceViewKey& key, const size_t seed ) const {
		using StringHash = Utility::StringHash;

	// ---

		return StringHash::operator()( key.first, StringHash::operator()( key.second->name(), seed ) );
	}

// ---------------------------------------------------

	ETInlineHint ContentLibrary::ResourceViewKey::ResourceViewKey( const ::Eldritch2::UTF8Char* const resourceName, const ::std::type_info* resourceType ) : ::std::pair<const ::Eldritch2::UTF8Char*, const ::std::type_info*>( resourceName, resourceType ) {}

// ---------------------------------------------------

	ETInlineHint ETNoAliasHint bool ContentLibrary::ResourceViewKey::operator==( const ContentLibrary::ResourceViewKey& other ) const {
		return (*second == *other.second) && Utility::StringEqualComparator<>()( first, other.first );
	}

// ---------------------------------------------------

	template <typename View>
	const View& ContentLibrary::ResolveViewByName( const UTF8Char* const name, const View& defaultView ) const {
		static_assert( ::std::is_base_of<FileSystem::ResourceView, View>::value, "ResolveViewByName must be used to convert to ResourceView-derived types!" );

	// ---

		Utility::ScopedReaderLock	_( *_resourceViewCollectionMutex );
		const auto					candidate( _resourceViewCollection.Find( { name, &typeid(defaultView) } ) );

		return (candidate != _resourceViewCollection.End()) ? (*static_cast<const View*>(candidate->second)) : defaultView;
	}

// ---------------------------------------------------

	ETInlineHint FileSystem::ContentProvider& ContentLibrary::GetContentProvider() const {
		return _contentProvider;
	}

}	// namespace FileSystem
}	// namespace Eldritch2