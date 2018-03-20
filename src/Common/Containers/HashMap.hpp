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
#include <Common/Memory/EaStlAllocatorMixin.hpp>
#include <Common/Containers/RangeAdapters.hpp>
#include <Common/Memory/MallocAllocator.hpp>
#include <Common/EqualTo.hpp>
#include <Common/Hash.hpp>
#include <Common/Pair.hpp>
//------------------------------------------------------------------//
ET_PUSH_COMPILER_WARNING_STATE()
//	(4800) EASTL does some int to bool coercion MSVC doesn't like.
	ET_SET_MSVC_WARNING_STATE( disable : 4800 )
#	include <eastl/hash_map.h>
ET_POP_COMPILER_WARNING_STATE()
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename Key, typename Value, class HashPredicate = Hash<Key>, class KeyEqualityPredicate = EqualTo<Key>, class Allocator = MallocAllocator, bool cacheHashCode = false>
	class HashMap {
	// - TYPE PUBLISHING ---------------------------------

	private:
		using UnderlyingContainer		= eastl::hash_map<Key, Value, HashPredicate, KeyEqualityPredicate, EaStlAllocatorMixin<Allocator>, cacheHashCode>;

	public:
		using ValueType					= typename UnderlyingContainer::value_type;
		using KeyType					= typename UnderlyingContainer::key_type;
		using MappedType				= typename UnderlyingContainer::mapped_type;
		using NodeType					= typename UnderlyingContainer::node_type;
		using AllocatorType				= typename UnderlyingContainer::allocator_type::PublicType;
		using SizeType					= typename UnderlyingContainer::size_type;
		using HashPredicateType			= HashPredicate;
		using EqualityPredicateType		= KeyEqualityPredicate;
		using HashCode					= typename UnderlyingContainer::hash_code_t;

		using Iterator					= typename UnderlyingContainer::iterator;
		using ConstIterator				= typename UnderlyingContainer::const_iterator;
		using LocalIterator				= typename UnderlyingContainer::local_iterator;
		using ConstLocalIterator		= typename UnderlyingContainer::const_local_iterator;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	//! Constructs this @ref HashMap instance.
		explicit HashMap(
			SizeType bucketCount,
			const HashPredicateType& hash = HashPredicateType(),
			const EqualityPredicateType& equalityPredicate = EqualityPredicateType(),
			const AllocatorType& allocator = AllocatorType()
		);
	//! Constructs this @ref HashMap instance.
		HashMap( const HashPredicateType& hash, const EqualityPredicateType& equalityPredicate, const AllocatorType& allocator );
	//! Constructs this @ref HashMap instance.
		HashMap( HashMap&&, const AllocatorType& allocator );
	//! Constructs this @ref HashMap instance.
		explicit HashMap( const AllocatorType& allocator );
	//! Constructs this @ref HashMap instance.
		HashMap( const HashMap& );
	//! Constructs this @ref HashMap instance.
		HashMap( HashMap&& );

		~HashMap() = default;

	// - ALGORITHMS --------------------------------------

	public:
	//! Retrieves a @ref ConstIterator pointing to the first element with a key equal to the specified, or an iterator to the end element if no item is found.
		template <typename AlternateKey, typename AlternateHashPredicate, typename AlternateKeyEqualityPredicate>
		ConstIterator	Find( const AlternateKey& key, const AlternateHashPredicate& hashPredicate, const AlternateKeyEqualityPredicate& keyEqualityPredicate ) const;
	//! Retrieves an @ref Iterator pointing to the first element with a key equal to the specified, or an iterator to the end element if no item is found.
		template <typename AlternateKey, typename AlternateHashPredicate, typename AlternateKeyEqualityPredicate>
		Iterator		Find( const AlternateKey& key, const AlternateHashPredicate& hashPredicate, const AlternateKeyEqualityPredicate& keyEqualityPredicate );
	//! Retrieves a @ref ConstIterator pointing to the first element with a key equal to the specified, or an iterator to the end element if no item is found.
		ConstIterator	Find( const KeyType& key ) const;
	//! Retrieves an @ref Iterator pointing to the first element with a key equal to the specified, or an iterator to the end element if no item is found.
		Iterator		Find( const KeyType& key );

	//! Erases all elements for which the result of the predicate returns true.
		template <typename Predicate>
		void			EraseIf( Predicate predicate );

		template <typename AlternateKey, typename AlternateHashPredicate, typename AlternateKeyEqualityPredicate>
		bool			ContainsKey( const AlternateKey& key, const AlternateHashPredicate& hashPredicate, const AlternateKeyEqualityPredicate& keyEqualityPredicate ) const;
		bool			ContainsKey( const KeyType& key ) const;

	// - ELEMENT ITERATION -------------------------------

	public:
		ConstLocalIterator	ConstBegin( SizeType bucketIndex ) const;
	//!	Retrieves a @ref ConstIterator to the first element stored in this @ref HashMap.
		ConstIterator		ConstBegin() const;
	
		ConstLocalIterator	ConstEnd( SizeType bucketIndex ) const;
	//!	Retrieves a @ref ConstIterator to one past the end of all elements stored in this @ref HashMap.
		ConstIterator		ConstEnd() const;

		ConstLocalIterator	Begin( SizeType bucketIndex ) const;
		LocalIterator		Begin( SizeType bucketIndex );
	//!	Retrieves a @ref ConstIterator to the first element stored in this @ref HashMap.
		ConstIterator		Begin() const;
	//!	Retrieves an @ref Iterator to the first element stored in this @ref HashMap.
		Iterator			Begin();

		ConstLocalIterator	End( SizeType bucketIndex ) const;
		LocalIterator		End( SizeType bucketIndex );
	//! Retrieves a @ref ConstIterator to one past the end of all elements stored in this @ref HashMap.
		ConstIterator		End() const;
	//!	Retrieves an @ref Iterator to one past the end of all elements stored in this @ref HashMap.
		Iterator			End();

	// - ELEMENT ACCESS ----------------------------------

	public:
		MappedType&	operator[]( const KeyType& key );
		MappedType&	operator[]( KeyType&& key );

	// - CONTAINER DUPLICATION ---------------------------

	public:
		HashMap&	operator=( const HashMap& );
		HashMap&	operator=( HashMap&& );

	// - CONTAINER MANIPULATION --------------------------
	    
	public:
		Pair<Iterator, bool>	Insert( const ValueType& value );
		Pair<Iterator, bool>	Insert( ValueType&& value );

		template <typename... Arguments>
		Pair<Iterator, bool>	TryEmplace( const KeyType& key, Arguments&&... arguments );
		template <typename... Arguments>
		Pair<Iterator, bool>	TryEmplace( KeyType&& key, Arguments&&... arguments );

		template <typename... Arguments>
		Pair<Iterator, bool>	Emplace( Arguments&&... arguments );
		
		Iterator				Erase( Iterator first, Iterator last );
		Iterator				Erase( Iterator position );
		SizeType				Erase( const KeyType& key );

		template <typename Disposer>
		void					ClearAndDispose( Disposer disposer );

		void					Clear();

	// - CONTENT QUERY -----------------------------------

	public:
		SizeType			GetSize() const;

		bool				IsEmpty() const;

		explicit operator	bool() const;

	// - ALLOCATOR ACCESS --------------------------------

	public:
		const EqualityPredicateType&	GetKeyEqualityPredicate() const;

		const HashPredicateType&		GetHashPredicate() const;

		const AllocatorType&			GetAllocator() const;

	// - DATA MEMBERS ------------------------------------

	private:
		UnderlyingContainer	_container;

	// ---------------------------------------------------

		template <typename Key, typename Value, class HashPredicate, class KeyEqualityPredicate, class Allocator, bool cacheHashCode>
		friend void Swap( HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>&, HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, cacheHashCode>& );
	};

	template <typename Key, typename Value, class HashPredicate = Hash<Key>, class KeyEqualityPredicate = EqualTo<Key>, class Allocator = MallocAllocator>
	using CachingHashMap = HashMap<Key, Value, HashPredicate, KeyEqualityPredicate, Allocator, true>;

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Containers/HashMap.inl>
//------------------------------------------------------------------//