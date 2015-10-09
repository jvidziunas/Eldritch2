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

	namespace FileSystem	= ::Eldritch2::FileSystem;
	namespace Utility		= ::Eldritch2::Utility;

// ---------------------------------------------------

	ETInlineHint ContentLibrary::ResourceViewFactoryKey::ResourceViewFactoryKey( const ::Eldritch2::UTF8Char* const begin, const ::Eldritch2::UTF8Char* const end ) : ::std::pair<const ::Eldritch2::UTF8Char*, const ::Eldritch2::UTF8Char*>( begin, end ) {}

// ---------------------------------------------------

	ETInlineHint ContentLibrary::ResourceViewFactoryKey::ResourceViewFactoryKey() : ResourceViewFactoryKey( nullptr, nullptr ) {}

// ---------------------------------------------------

	ETInlineHint ETNoAliasHint size_t ContentLibrary::ResourceViewFactoryKey::Hash::operator()( const ResourceViewFactoryKey string, const size_t seed ) const {
		return StringHash::operator()( string.first, string.second, seed );
	}

// ---------------------------------------------------

	template <typename View>
	const View& ContentLibrary::ResolveViewByName( const UTF8Char* const name, const View& defaultView ) const {
		using ViewLibrary = decltype(_resourceViewLibrary);

	// ---

		static_assert( ::std::is_base_of<FileSystem::ResourceView, View>::value, "ResolveViewByName must be used to convert to ResourceView-derived types!" );

		Utility::ScopedReaderLock	_( _resourceViewLibraryMutex );
		ViewLibrary::ConstIterator	resourceCandidate( _resourceViewLibrary.Find( name ) );

		if( resourceCandidate != _resourceViewLibrary.End() ) {
			if( auto* const	concreteView = dynamic_cast<const View*>(resourceCandidate->second) ) {
				return *concreteView;
			}
		}

		return defaultView;
	}

}	// namespace FileSystem
}	// namespace Eldritch2