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

	ETInlineHint const FileSystem::ContentPackage& World::GetRootPackage() const {
		return *static_cast<const FileSystem::ContentPackage*>(_package);
	}

// ---------------------------------------------------

	ETInlineHint ::Eldritch2::Allocator& World::GetAllocator() {
		return _allocator;
	}

// ---------------------------------------------------

	ETInlineHint void World::RaiseFatalError() const {
		_fatalErrorCount.fetch_add( 1u, ::std::memory_order_acq_rel );
	}

// ---------------------------------------------------

	ETInlineHint ::Eldritch2::uint32 World::EncounteredFatalError() const {
		return 0u != _fatalErrorCount.load( ::std::memory_order_acquire );
	}

}	// namespace Foundation
}	// namespace Eldritch2