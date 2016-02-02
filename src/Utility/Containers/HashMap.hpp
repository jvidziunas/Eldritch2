/*==================================================================*\
  HashMap.hpp
  ------------------------------------------------------------------
  Purpose:
  
  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Memory/RDESTLAllocatorAdapterMixin.hpp>
#include <Utility/Memory/ChildAllocator.hpp>
#include <Utility/Equals.hpp>
#include <Utility/Hash.hpp>
#include <Utility/Pair.hpp>
//------------------------------------------------------------------//
#include <rdestl/hash_map.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename Key, typename StoredObject, class Hasher = ::Eldritch2::Hash<Key>, class KeyEqualityComparator = ::Eldritch2::Equals<Key>, class Allocator = ::Eldritch2::ChildAllocator, int loadFactor = 6>
	class HashMap {
	// - TYPE PUBLISHING ---------------------------------

	protected:
		using PrivateAllocator		= Detail::RDESTLAllocatorAdapterMixin<Allocator>;
		using UnderlyingContainer	= ::rde::hash_map<Key, StoredObject, Hasher, loadFactor, KeyEqualityComparator, PrivateAllocator>;

	public:
		using ValueType				= typename UnderlyingContainer::value_type;
		using KeyType				= typename UnderlyingContainer::key_type;
		using MappedType			= typename UnderlyingContainer::mapped_type;
		using AllocatorType			= Allocator;
		using SizeType				= typename UnderlyingContainer::size_type;
		using Iterator				= typename UnderlyingContainer::iterator;
		using ConstIterator			= typename UnderlyingContainer::const_iterator;
		using InsertResult			= ::Eldritch2::Pair<Iterator, bool>;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		//! Constructs this @ref HashMap instance.
		ETInlineHint explicit HashMap( AllocatorType&& allocatorType = AllocatorType() );
		//! Constructs this @ref HashMap instance.
		ETInlineHint HashMap( Hasher&& hasher, AllocatorType&& allocatorType = AllocatorType() );
		//! Constructs this @ref HashMap instance.
		template <class AlternateAllocator, int alternateLoadFactor>
		ETInlineHint HashMap( const ::Eldritch2::HashMap<Key, StoredObject, Hasher, KeyEqualityComparator, AlternateAllocator, alternateLoadFactor>& containerTemplate, AllocatorType&& allocatorType = AllocatorType() );
		//! Constructs this @ref HashMap instance.
		ETInlineHint HashMap( ::Eldritch2::HashMap<Key, StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>&& moveSource );

		ETInlineHint ~HashMap() = default;

	// - ALGORITHMS --------------------------------------

		//! Retrieves a @ref ConstIterator pointing to the first element with a key equal to the specified, or an iterator to the end element if no item is found.
		ETInlineHint ConstIterator	Find( const KeyType& key ) const;
		//! Retrieves a @ref ConstIterator pointing to the first element with a key equal to the specified, or an iterator to the end element if no item is found.
		template <typename AlternateKey>
		ETInlineHint ConstIterator	Find( const AlternateKey& key ) const;
		//! Retrieves an @ref Iterator pointing to the first element with a key equal to the specified, or an iterator to the end element if no item is found.
		ETInlineHint Iterator		Find( const KeyType& key );
		//! Retrieves an @ref Iterator pointing to the first element with a key equal to the specified, or an iterator to the end element if no item is found.
		template <typename AlternateKey>
		ETInlineHint Iterator		Find( const AlternateKey& key );

		//! Erases all elements for which the result of the predicate returns true.
		template <typename Predicate>
		ETInlineHint void	RemoveIf( Predicate predicate );

	// - ELEMENT ITERATION -------------------------------

		//!	Retrieves a @ref ConstIterator to the first element stored in this @ref HashMap.
		ETInlineHint ConstIterator	ConstBegin() const;

		//!	Retrieves a @ref ConstIterator to the first element stored in this @ref HashMap.
		ETInlineHint ConstIterator	Begin() const;
		//!	Retrieves an @ref Iterator to the first element stored in this @ref HashMap.
		ETInlineHint Iterator		Begin();

		//!	Retrieves a const iterator to one past the end of all elements stored in this HashMap.
		ETInlineHint ConstIterator	ConstEnd() const;

		// Retrieves a const iterator to one past the end of all elements stored in this HashMap.
		ETInlineHint ConstIterator	End() const;
		// Retrieves an iterator to one past the end of all elements stored in this HashMap.
		ETInlineHint Iterator		End();

	// - ELEMENT ACCESS ----------------------------------

		ETInlineHint MappedType&	operator[]( KeyType&& key );
		ETInlineHint MappedType&	operator[]( const KeyType& key );

	// - CONTAINER DUPLICATION ---------------------------

		ETInlineHint HashMap<Key, StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>&	operator=( const HashMap<Key, StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>& containerTemplate );

		ETInlineHint void	CloneFrom( const HashMap<Key, StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>& containerTemplate );

		ETInlineHint void	Swap( HashMap<Key, StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>& rhs );

	// - CONTAINER MANIPULATION --------------------------
	    
		ETInlineHint InsertResult	Insert( ValueType&& value );
		ETInlineHint InsertResult	Insert( const ValueType& value );

		ETInlineHint SizeType		Erase( const KeyType& key );
		ETInlineHint void			Erase( Iterator position );
		ETInlineHint void			Erase( Iterator begin, Iterator end );

		ETInlineHint void	Clear();

		template <typename Disposer>
		ETInlineHint void	ClearAndDispose( Disposer disposer );

	// - CONTENT QUERY -----------------------------------

		ETInlineHint SizeType	Size() const;

		ETInlineHint SizeType	Empty() const;

		ETInlineHint			operator bool() const;

	// - CAPACITY QUERY ----------------------------------

		ETInlineHint void		Reserve( const SizeType minimumSizeHint );

		ETInlineHint SizeType	BucketCount() const;
		
		ETInlineHint SizeType	NonemptyBucketCount() const;

		ETInlineHint SizeType	UsedMemory() const;

	// - ALLOCATOR ACCESS --------------------------------

		ETInlineHint const AllocatorType&	GetAllocator() const;

	// - DATA MEMBERS ------------------------------------

	private:
		UnderlyingContainer	_underlyingContainer;
	};

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/Containers/HashMap.inl>
//------------------------------------------------------------------//