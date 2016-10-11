/*==================================================================*\
  HashMultiMap.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Memory/EaStlAllocatorAdapterMixin.hpp>
#include <Utility/Containers/RangeAdapters.hpp>
#include <Utility/Memory/ChildAllocator.hpp>
#include <Utility/Containers/Range.hpp>
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
	class HashMultiMap {
	// - TYPE PUBLISHING ---------------------------------

	private:
		using UnderlyingContainer		= eastl::hash_multimap<Key, Value, HashPredicate, KeyEqualityPredicate, Detail::EaStlAllocatorAdapterMixin<Allocator>, cacheHashCode>;

	public:
		using ValueType					= typename UnderlyingContainer::value_type;
		using KeyType					= typename UnderlyingContainer::key_type;
		using MappedType				= typename UnderlyingContainer::mapped_type;
		using AllocatorType				= typename UnderlyingContainer::allocator_type::PublicType;
		using SizeType					= typename UnderlyingContainer::size_type;
		using Iterator					= typename UnderlyingContainer::iterator;
		using ConstIterator				= typename UnderlyingContainer::const_iterator;
		using LocalIterator				= typename UnderlyingContainer::local_iterator;
		using ConstLocalIterator		= typename UnderlyingContainer::const_local_iterator;
		using HashPredicateType			= HashPredicate;
		using KeyEqualityPredicateType	= KeyEqualityPredicate;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//! Constructs this @ref HashMultiMap instance.
		HashMultiMap( const HashPredicateType& hashPredicate, const KeyEqualityPredicateType& keyEqualityPredicate, const AllocatorType& allocatorType = AllocatorType() );
	//! Constructs this @ref HashMultiMap instance.
		template <class = eastl::enable_if<eastl::is_copy_constructible<ValueType>::value>::type>
		HashMultiMap( const HashMultiMap&, const AllocatorType& allocatorType = AllocatorType() );
	//! Constructs this @ref HashMultiMap instance.
		explicit HashMultiMap( SizeType sizeHint, const AllocatorType& allocatorType = AllocatorType() );
	//! Constructs this @ref HashMultiMap instance.
		explicit HashMultiMap( const AllocatorType& allocatorType = AllocatorType() );
	//! Constructs this @ref HashMultiMap instance.
		HashMultiMap( HashMultiMap&& );

		~HashMultiMap() = default;

	// - ALGORITHMS --------------------------------------

	public:
		template <typename AlternateKey, typename AlternateHashPredicate, typename AlternateKeyEqualityPredicate>
		Eldritch2::Range<ConstIterator>	EqualRange( const AlternateKey& key, const AlternateHashPredicate& hashPredicate, const AlternateKeyEqualityPredicate& keyEqualityPredicate ) const;
		template <typename AlternateKey, typename AlternateHashPredicate, typename AlternateKeyEqualityPredicate>
		Eldritch2::Range<Iterator>		EqualRange( const AlternateKey& key, const AlternateHashPredicate& hashPredicate, const AlternateKeyEqualityPredicate& keyEqualityPredicate );
		Eldritch2::Range<ConstIterator>	EqualRange( const KeyType& key ) const;
		Eldritch2::Range<Iterator>		EqualRange( const KeyType& key );

	//! Erases all elements for which the result of the predicate returns true.
		template <typename Predicate>
		void							RemoveIf( Predicate predicate );

	// - ELEMENT ITERATION -------------------------------

	public:
	//!	Retrieves a @ref ConstLocalIterator to the first element stored in the specified bucket of this @ref HashMultiMap.
		ConstLocalIterator	ConstBegin( SizeType bucketIndex ) const;
	//!	Retrieves a @ref ConstIterator to the first element stored in this @ref HashMultiMap.
		ConstIterator		ConstBegin() const;
	
	//!	Retrieves a @ref ConstLocalIterator to one past the end of all elements stored in this @ref HashMultiMap.
		ConstLocalIterator	ConstEnd( SizeType bucketIndex ) const;
	//!	Retrieves a @ref ConstIterator to one past the end of all elements stored in this @ref HashMultiMap.
		ConstIterator		ConstEnd() const;

	//!	Retrieves a @ref ConstLocalIterator to the first element stored in this @ref HashMultiMap.
		ConstLocalIterator	Begin( SizeType bucketIndex ) const;
	//!	Retrieves a @ref LocalIterator to the first element stored in this @ref HashMultiMap.
		LocalIterator		Begin( SizeType bucketIndex );
	//!	Retrieves a @ref ConstIterator to the first element stored in this @ref HashMultiMap.
		ConstIterator		Begin() const;
	//!	Retrieves an @ref Iterator to the first element stored in this @ref HashMultiMap.
		Iterator			Begin();

	//! Retrieves a @ref ConstLocalIterator to one past the end of all elements stored in this @ref HashMultiMap.
		ConstLocalIterator	End( SizeType bucketIndex ) const;
	//!	Retrieves an @ref LocalIterator to one past the end of all elements stored in this @ref HashMultiMap.
		LocalIterator		End( SizeType bucketIndex );
	//! Retrieves a @ref ConstIterator to one past the end of all elements stored in this @ref HashMultiMap.
		ConstIterator		End() const;
	//!	Retrieves an @ref Iterator to one past the end of all elements stored in this @ref HashMultiMap.
		Iterator			End();

	// - CONTAINER DUPLICATION ---------------------------

	public:
		template <class = eastl::enable_if<eastl::is_copy_constructible<ValueType>::value>::type>
		HashMultiMap&	operator=( const HashMultiMap& );
		HashMultiMap&	operator=( HashMultiMap&& );

		void			Swap( HashMultiMap& rhs );

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
		SizeType	GetSize() const;

		SizeType	IsEmpty() const;

		explicit	operator bool() const;

	// - ALLOCATOR ACCESS --------------------------------

	public:
		const HashPredicateType&	GetHashPredicate() const;

		const AllocatorType&		GetAllocator() const;

	// - DATA MEMBERS ------------------------------------

	private:
		UnderlyingContainer	_underlyingContainer;
	};

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/Containers/HashMultiMap.inl>
//------------------------------------------------------------------//

