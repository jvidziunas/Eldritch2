/*==================================================================*\
  UnorderedMap.hpp
  ------------------------------------------------------------------
  Purpose:
  
  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Memory/RDESTLAllocatorAdapterMixin.hpp>
#include <Utility/Memory/ChildAllocator.hpp>
//------------------------------------------------------------------//
#include <rdestl/hash_map.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename Key, typename StoredObject, class Hasher = ::rde::hash<Key>, class KeyEqualityComparator = ::rde::equal_to<Key>, class Allocator = ::Eldritch2::ChildAllocator, int loadFactor = 6>
	class UnorderedMap {
	// - TYPE PUBLISHING ---------------------------------

	protected:
		using UnderlyingContainer	= ::rde::hash_map<Key, StoredObject, Hasher, loadFactor, KeyEqualityComparator, ::Eldritch2Detail::RDESTLAllocatorAdapterMixin<Allocator>>;

	public:
		using ValueType				= typename UnderlyingContainer::value_type;
		using KeyType				= typename UnderlyingContainer::key_type;
		using MappedType			= typename UnderlyingContainer::mapped_type;
		using AllocatorType			= Allocator;
		using SizeType				= typename UnderlyingContainer::size_type;
		using Iterator				= typename UnderlyingContainer::iterator;
		using ConstIterator			= typename UnderlyingContainer::const_iterator;
		using InsertResult			= ::rde::pair<Iterator, bool>;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		//! Constructs this @ref UnorderedMap instance.
		template <typename... AllocatorConstructorArguments>
		ETInlineHint explicit UnorderedMap( const SizeType initialBucketCount, AllocatorConstructorArguments&&... allocatorConstructorArguments );
		//! Constructs this @ref UnorderedMap instance.
		template <typename... AllocatorConstructorArguments>
		ETInlineHint UnorderedMap( const SizeType initialBucketCount, const Hasher& hasher, AllocatorConstructorArguments&&... allocatorConstructorArguments );
		//! Constructs this @ref UnorderedMap instance.
		template <class AlternateAllocator, int alternateLoadFactor, typename... AllocatorConstructorArguments>
		ETInlineHint UnorderedMap( const ::Eldritch2::UnorderedMap<Key, StoredObject, Hasher, KeyEqualityComparator, AlternateAllocator, alternateLoadFactor>& containerTemplate, AllocatorConstructorArguments&&... allocatorConstructorArguments );

		//! Destroys this @ref UnorderedMap instance.
		ETInlineHint ~UnorderedMap();

	// - ALGORITHMS --------------------------------------

		//! Retrieves a @ref ConstIterator pointing to the first element with a key equal to the specified, or an iterator to the end element if no item is found.
		ETInlineHint ConstIterator	Find( const KeyType& key ) const;
		//! Retrieves an @ref Iterator pointing to the first element with a key equal to the specified, or an iterator to the end element if no item is found.
		ETInlineHint Iterator		Find( const KeyType& key );

		//! Erases all elements for which the result of the predicate returns true.
		template <typename Predicate>
		ETInlineHint void	RemoveIf( Predicate predicate );

	// - ELEMENT ITERATION -------------------------------

		// Retrieves a @ref ConstIterator to the first element stored in this UnorderedMap.
		ETInlineHint ConstIterator	ConstBegin() const;

		// Retrieves a const iterator to the first element stored in this UnorderedMap.
		ETInlineHint ConstIterator	Begin() const;
		// Retrieves an iterator to the first element stored in this UnorderedMap.
		ETInlineHint Iterator		Begin();

		// Retrieves a const iterator to one past the end of all elements stored in this UnorderedMap.
		ETInlineHint ConstIterator	ConstEnd() const;

		// Retrieves a const iterator to one past the end of all elements stored in this UnorderedMap.
		ETInlineHint ConstIterator	End() const;
		// Retrieves an iterator to one past the end of all elements stored in this UnorderedMap.
		ETInlineHint Iterator		End();

	// - ELEMENT ACCESS ----------------------------------

		ETInlineHint MappedType&	operator[]( KeyType&& key );
		ETInlineHint MappedType&	operator[]( const KeyType& key );

	// - CONTAINER DUPLICATION ---------------------------

		ETInlineHint UnorderedMap<Key, StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>&	operator=( const UnorderedMap<Key, StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>& containerTemplate );

		ETInlineHint void	CloneFrom( const UnorderedMap<Key, StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>& containerTemplate );

		ETInlineHint void	Swap( UnorderedMap<Key, StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>& rhs );

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
#include <Utility/Containers/UnorderedMap.inl>
//------------------------------------------------------------------//