/*==================================================================*\
  HashSet.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
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
#include <rdestl/hash_set.h>
//------------------------------------------------------------------//


namespace Eldritch2 {

	template <typename StoredObject, class Hasher = ::Eldritch2::Hash<StoredObject>, class KeyEqualityComparator = ::Eldritch2::Equals<StoredObject>, class Allocator = ::Eldritch2::ChildAllocator, int loadFactor = 6>
	class HashSet {
	// - TYPE PUBLISHING ---------------------------------

	protected:
		using PrivateAllocator		= Detail::RDESTLAllocatorAdapterMixin<Allocator>;
		using UnderlyingContainer	= ::rde::hash_set<StoredObject, Hasher, loadFactor, KeyEqualityComparator, PrivateAllocator>;

	public:
		using ValueType				= typename UnderlyingContainer::value_type;
		using KeyType				= typename UnderlyingContainer::key_type;
		using AllocatorType			= Allocator;
		using SizeType				= typename UnderlyingContainer::size_type;
		using Iterator				= typename UnderlyingContainer::iterator;
		using ConstIterator			= typename UnderlyingContainer::const_iterator;
		using InsertResult			= ::Eldritch2::Pair<Iterator, bool>;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		//! Constructs this @ref HashSet instance.
		ETInlineHint explicit HashSet( AllocatorType&& allocatorType = AllocatorType() );
		//! Constructs this @ref HashSet instance.
		ETInlineHint HashSet( Hasher&& hasher, AllocatorType&& allocatorType = AllocatorType() );
		//! Constructs this @ref HashSet instance.
		template <class AlternateAllocator, int alternateLoadFactor>
		ETInlineHint HashSet( const HashSet<StoredObject, Hasher, KeyEqualityComparator, AlternateAllocator, alternateLoadFactor>& containerTemplate, AllocatorType&& allocatorType = AllocatorType() );
		//! Constructs this @ref HashSet instance.
		ETInlineHint HashSet( HashSet<StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>&& moveSource );

		ETInlineHint ~HashSet() = default;

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

		//!	Retrieves a @ref ConstIterator to the first element stored in this @ref HashSet.
		ETInlineHint ConstIterator	ConstBegin() const;

		//!	Retrieves a @ref ConstIterator to the first element stored in this @ref HashSet.
		ETInlineHint ConstIterator	Begin() const;
		//!	Retrieves an @ref Iterator to the first element stored in this @ref HashSet.
		ETInlineHint Iterator		Begin();

		//!	Retrieves a const iterator to one past the end of all elements stored in this HashSet.
		ETInlineHint ConstIterator	ConstEnd() const;

		// Retrieves a const iterator to one past the end of all elements stored in this HashSet.
		ETInlineHint ConstIterator	End() const;
		// Retrieves an iterator to one past the end of all elements stored in this HashSet.
		ETInlineHint Iterator		End();

	// - CONTAINER DUPLICATION ---------------------------

		ETInlineHint HashSet<StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>&	operator=( const HashSet<StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>& containerTemplate );

		ETInlineHint void	CloneFrom( const HashSet<StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>& containerTemplate );

		ETInlineHint void	Swap( HashSet<StoredObject, Hasher, KeyEqualityComparator, Allocator, loadFactor>& rhs );

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
#include <Utility/Containers/HashSet.inl>
//------------------------------------------------------------------//



