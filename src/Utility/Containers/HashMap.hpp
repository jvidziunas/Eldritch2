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
#include <Utility/Memory/EaStlAllocatorAdapterMixin.hpp>
#include <Utility/Containers/RangeAdapters.hpp>
#include <Utility/Memory/ChildAllocator.hpp>
#include <Utility/EqualTo.hpp>
#include <Utility/Hash.hpp>
#include <Utility/Pair.hpp>
//------------------------------------------------------------------//
#if ET_COMPILER_IS_MSVC && !defined( EA_COMPILER_HAS_C99_FORMAT_MACROS )
//	MSVC complains about *lots* of macro redefinitions in eabase/inttypes.h.
#	define EA_COMPILER_HAS_C99_FORMAT_MACROS
#endif
#include <eastl/hash_map.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename Key, typename Value, class HashPredicate = Eldritch2::Hash<Key>, class KeyEqualityPredicate = Eldritch2::EqualTo<Key>, class Allocator = Eldritch2::ChildAllocator, bool cacheHashCode = false>
	class HashMap {
	// - TYPE PUBLISHING ---------------------------------

	private:
		using UnderlyingContainer		= eastl::hash_map<Key, Value, HashPredicate, KeyEqualityPredicate, Detail::EaStlAllocatorAdapterMixin<Allocator>, cacheHashCode>;

	public:
		using ValueType					= typename UnderlyingContainer::value_type;
		using KeyType					= typename UnderlyingContainer::key_type;
		using MappedType				= typename UnderlyingContainer::mapped_type;
		using NodeType					= typename UnderlyingContainer::node_type;
		using AllocatorType				= typename UnderlyingContainer::allocator_type::PublicType;
		using SizeType					= typename UnderlyingContainer::size_type;
		using HashPredicateType			= HashPredicate;
		using KeyEqualityPredicateType	= KeyEqualityPredicate;
		using HashCode					= typename UnderlyingContainer::hash_code_t;

		using Iterator					= typename UnderlyingContainer::iterator;
		using ConstIterator				= typename UnderlyingContainer::const_iterator;
		using LocalIterator				= typename UnderlyingContainer::local_iterator;
		using ConstLocalIterator		= typename UnderlyingContainer::const_local_iterator;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	//! Constructs this @ref HashMap instance.
		HashMap( const HashPredicateType& hashPredicate, const KeyEqualityPredicateType& keyEqualityPredicate, const AllocatorType& allocatorType = AllocatorType() );
	//! Constructs this @ref HashMap instance.
		explicit HashMap( SizeType sizeHint, const AllocatorType& allocatorType = AllocatorType() );
	//! Constructs this @ref HashMap instance.
		explicit HashMap( const AllocatorType& allocatorType = AllocatorType() );
	//! Constructs this @ref HashMap instance.
		template <class = eastl::enable_if<eastl::is_copy_constructible<ValueType>::value>::type>
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
		void			RemoveIf( Predicate predicate );

		template <typename AlternateKey, typename AlternateHashPredicate, typename AlternateKeyEqualityPredicate>
		bool			ContainsKey( const AlternateKey& key, const AlternateHashPredicate& hashPredicate, const AlternateKeyEqualityPredicate& keyEqualityPredicate ) const;
		bool			ContainsKey( const KeyType& key ) const;

	// - ELEMENT ITERATION -------------------------------

	public:
	//!	Retrieves a @ref ConstLocalIterator to the first element stored in the specified bucket of this @ref HashMap.
		ConstLocalIterator	ConstBegin( SizeType bucketIndex ) const;
	//!	Retrieves a @ref ConstIterator to the first element stored in this @ref HashMap.
		ConstIterator		ConstBegin() const;
	
	//!	Retrieves a @ref ConstLocalIterator to one past the end of all elements stored in this @ref HashMap.
		ConstLocalIterator	ConstEnd( SizeType bucketIndex ) const;
	//!	Retrieves a @ref ConstIterator to one past the end of all elements stored in this @ref HashMap.
		ConstIterator		ConstEnd() const;

	//!	Retrieves a @ref ConstLocalIterator to the first element stored in this @ref HashMap.
		ConstLocalIterator	Begin( SizeType bucketIndex ) const;
	//!	Retrieves a @ref LocalIterator to the first element stored in this @ref HashMap.
		LocalIterator		Begin( SizeType bucketIndex );
	//!	Retrieves a @ref ConstIterator to the first element stored in this @ref HashMap.
		ConstIterator		Begin() const;
	//!	Retrieves an @ref Iterator to the first element stored in this @ref HashMap.
		Iterator			Begin();

	//! Retrieves a @ref ConstLocalIterator to one past the end of all elements stored in this @ref HashMap.
		ConstLocalIterator	End( SizeType bucketIndex ) const;
	//!	Retrieves an @ref LocalIterator to one past the end of all elements stored in this @ref HashMap.
		LocalIterator		End( SizeType bucketIndex );
	//! Retrieves a @ref ConstIterator to one past the end of all elements stored in this @ref HashMap.
		ConstIterator		End() const;
	//!	Retrieves an @ref Iterator to one past the end of all elements stored in this @ref HashMap.
		Iterator			End();

	// - ELEMENT ACCESS ----------------------------------

	public:
		template <class = eastl::is_default_constructible<ValueType>::value>
		MappedType&	operator[]( const KeyType& key );
		template <class = eastl::is_default_constructible<ValueType>::value>
		MappedType&	operator[]( KeyType&& key );

	// - CONTAINER DUPLICATION ---------------------------

	public:
		template <class = eastl::enable_if<eastl::is_copy_constructible<ValueType>::value>::type>
		HashMap&	operator=( const HashMap& );
		HashMap&	operator=( HashMap&& );

		void		Swap( HashMap& rhs );

	// - CONTAINER MANIPULATION --------------------------
	    
	public:
		template <class = eastl::enable_if<eastl::is_copy_constructible<ValueType>::value>::type>
		Eldritch2::Pair<Iterator, bool>	Insert( const ValueType& value );
		Eldritch2::Pair<Iterator, bool>	Insert( ValueType&& value );

		template <typename... Arguments>
		Eldritch2::Pair<Iterator, bool>	Emplace( Arguments&&... arguments );
		
		Iterator						Erase( Iterator begin, Iterator end );
		Iterator						Erase( Iterator position );
		SizeType						Erase( const KeyType& key );

		template <typename Disposer>
		void							ClearAndDispose( Disposer disposer );

		void							Clear();

	// - CONTENT QUERY -----------------------------------

	public:
		SizeType			GetSize() const;

		SizeType			IsEmpty() const;

		explicit operator	bool() const;

	// - ALLOCATOR ACCESS --------------------------------

	public:
		const KeyEqualityPredicateType&	GetKeyEqualityPredicate() const;

		const HashPredicateType&		GetHashPredicate() const;

		const AllocatorType&			GetAllocator() const;

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