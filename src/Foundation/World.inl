/*==================================================================*\
  World.inl
  ------------------------------------------------------------------
  Purpose:
  
  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Concurrency/Lock.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Foundation {

	template <typename PropertyValue>
	PropertyValue World::GetPropertyByKey( const ::Eldritch2::UTF8Char* const key, PropertyValue defaultValue ) const {
		{	Utility::ScopedReaderLock	_( *_propertyMutex );
			const auto candidate( _properties.Find( key ) );

			if( _properties.End() != candidate ) {
				candidate->second.ParseInto( defaultValue );
			}
		}	// End of lock scope.

		return defaultValue;
	}

// ---------------------------------------------------

	ETInlineHint const ::Eldritch2::IntrusiveForwardList<Foundation::WorldView>& World::GetAttachedViews() const {
		return _attachedViews;
	}

// ---------------------------------------------------

	ETInlineHint::Eldritch2::IntrusiveForwardList<Foundation::WorldView>& World::GetAttachedViews() {
		return _attachedViews;
	}

// ---------------------------------------------------

	ETInlineHint const FileSystem::ContentPackage& World::GetRootPackage() const {
		return *static_cast<const FileSystem::ContentPackage*>(_package);
	}

// ---------------------------------------------------

	ETInlineHint const FileSystem::ContentLibrary& World::GetContentLibrary() const {
		return _contentLibrary;
	}

// ---------------------------------------------------

	ETInlineHint ::Eldritch2::Allocator& World::GetAllocator() {
		return _allocator;
	}

// ---------------------------------------------------

	ETInlineHint void World::RaiseFatalError() const {
		_executeFlag.store( false, ::std::memory_order_release );
	}

// ---------------------------------------------------

	ETInlineHint void World::Terminate() const {
		_executeFlag.store( false, ::std::memory_order_release );
	}

// ---------------------------------------------------

	ETInlineHint bool World::CanExecute() const {
		return _executeFlag.load( ::std::memory_order_acquire );
	}

// ---------------------------------------------------

	ETInlineHint bool World::IsLoading() const {
		return !IsLoaded();
	}

// ---------------------------------------------------

	ETInlineHint bool World::IsRunningGame() const {
		return !IsPaused();
	}

}	// namespace Foundation
}	// namespace Eldritch2