/*==================================================================*\
  Cache.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename Key, typename CachedObject, class LoadPolicy>
	Cache<Key, CachedObject, LoadPolicy>::Cache( SizeType maximumSize, LoadPolicyType&& loadPolicy, AllocatorType&& allocator ) : _accessTicket( 0u ),
																																  _maximumSize( maximumSize ),
																																  _loadPolicy( eastl::forward<LoadPolicyType>( loadPolicy ) ),
																																  _lookupTable( eastl::forward<AllocatorType>( allocator ) ) {}

// ---------------------------------------------------

	template <typename Key, typename CachedObject, class LoadPolicy>
	Cache<Key, CachedObject, LoadPolicy>::Cache( LoadPolicy&& loadPolicy, AllocatorType&& allocator ) : Cache( eastl::numeric_limits<SizeType>::max(), eastl::forward<LoadPolicyType>( loadPolicy ), eastl::forward<AllocatorType>( allocator ) ) {}

// ---------------------------------------------------

	template <typename Key, typename CachedObject, class LoadPolicy>
	Cache<Key, CachedObject, LoadPolicy>::Cache( AllocatorType&& allocator ) : Cache( LoadPolicy(), eastl::forward<AllocatorType>( allocator ) ) {}

// ---------------------------------------------------

	template <typename Key, typename CachedObject, class LoadPolicy>
	template <typename AlternateLoadPolicy>
	typename Cache<Key, CachedObject, LoadPolicy>::MappedType& Cache<Key, CachedObject, LoadPolicy>::Find( const KeyType& key, const AlternateLoadPolicy& loadPolicy ) {
		const auto	candidate( _lookupTable.Find( key ) );

		if( candidate != _lookupTable.End() ) {
			candidate->second.second = _accessTicket++;

			return candidate->second.first;
		}

		return loadPolicy( key );
	}

// ---------------------------------------------------

	template <typename Key, typename CachedObject, class LoadPolicy>
	typename Cache<Key, CachedObject, LoadPolicy>::MappedType& Cache<Key, CachedObject, LoadPolicy>::Find( const KeyType& key ) {
		const auto	candidate( _lookupTable.Find( key ) );

		if( candidate != _lookupTable.End() ) {
			candidate->second.second = _accessTicket++;

			return candidate->second.first;
		}

		return Insert( key, GetLoadPolicy()( key ) );
	}

// ---------------------------------------------------

	template <typename Key, typename CachedObject, class LoadPolicy>
	typename Cache<Key, CachedObject, LoadPolicy>::MappedType& Cache<Key, CachedObject, LoadPolicy>::Insert( const KeyType& key, const MappedType& value ) {
		if( _lookupTable.GetSize() < _maximumSize ) {
		//	LRU eviction.
			_lookupTable.Erase( eastl ::min_element( _lookupTable.Begin(), _lookupTable.End(), [] ( const typename LookupTableType::ValueType& element0, const typename LookupTableType::ValueType& element1 ) {
				return element0.second.second < element1.second.second;
			} ) );
		}

		return _lookupTable.Insert( { key, typename LookupTableType::MappedType( value, _accessTicket++ ) } ).first->second.first;
	}

// ---------------------------------------------------

	template <typename Key, typename CachedObject, class LoadPolicy>
	typename Cache<Key, CachedObject, LoadPolicy>::MappedType& Cache<Key, CachedObject, LoadPolicy>::Insert( const KeyType& key, MappedType&& value ) {
		if( _lookupTable.GetSize() < _maximumSize ) {
		//	LRU eviction.
			_lookupTable.Erase( eastl::min_element( _lookupTable.Begin(), _lookupTable.End(), [] ( const typename LookupTableType::ValueType& element0, const typename LookupTableType::ValueType& element1 ) {
				return element0.second.second < element1.second.second;
			} ) );
		}

		return _lookupTable.Insert( { Key( key ), typename LookupTableType::MappedType( eastl::forward<MappedType>( value ), _accessTicket++ ) } ).first->second.first;
	}

// ---------------------------------------------------

	template <typename Key, typename CachedObject, class LoadPolicy>
	void Cache<Key, CachedObject, LoadPolicy>::Erase( const KeyType& key ) {
		_lookupTable.Erase( key );
	}

// ---------------------------------------------------

	template <typename Key, typename CachedObject, class LoadPolicy>
	void Cache<Key, CachedObject, LoadPolicy>::Clear() {
		_lookupTable.Clear();
		_accessTicket = 0u;
	}

// ---------------------------------------------------

	template <typename Key, typename CachedObject, class LoadPolicy>
	ETInlineHint const typename Cache<Key, CachedObject, LoadPolicy>::LoadPolicyType& Cache<Key, CachedObject, LoadPolicy>::GetLoadPolicy() {
		return _loadPolicy;
	}

}	// namespace Eldritch2

