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

	ETInlineHint bool ContentLibrary::NameEqual::operator()( const ::Eldritch2::UTF8Char* const name0, const ::Eldritch2::UTF8Char* const name1 ) const {
		return ::Eldritch2::EqualityCompareString( name0, name1 );
	}

// ---------------------------------------------------

	ETInlineHint bool ContentLibrary::ViewKeyEqual::operator()( const ResourceViewKey& left, const ResourceViewKey& right ) const {
		return ( *left.second == *right.second ) && ::Eldritch2::EqualityCompareString( left.first, right.first );
	}

// ---------------------------------------------------

	template <typename View>
	const View* ContentLibrary::ResolveViewByName( const UTF8Char* const name ) const {
		static_assert( ::std::is_base_of<FileSystem::ResourceView, View>::value, "ResolveViewByName() must be used to convert to ResourceView-derived types!" );

	// ---

		Utility::ScopedReaderLock	_( *_resourceViewDirectoryMutex );
		const auto					candidate( _resourceViewDirectory.Find( { name, &typeid(View) } ) );

		return static_cast<const View*>(candidate != _resourceViewDirectory.End() ? candidate->second : nullptr);
	}

// ---------------------------------------------------

	ETInlineHint FileSystem::ContentProvider& ContentLibrary::GetContentProvider() const {
		return _contentProvider;
	}

// ---------------------------------------------------

	ETInlineHint const ::Eldritch2::IntrusiveForwardList<FileSystem::ResourceViewFactory>& ContentLibrary::GetFactoriesForResourceType( const ::Eldritch2::UTF8Char* const resourceTypeName ) const {
		const auto	factoryCollectionCandidate( _resourceFactoryDirectory.Find( resourceTypeName ) );
		
		return factoryCollectionCandidate != _resourceFactoryDirectory.End() ? factoryCollectionCandidate->second : _orphanContentFactories;
	}

}	// namespace FileSystem
}	// namespace Eldritch2