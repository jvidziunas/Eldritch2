/*==================================================================*\
  ArrayMap.hpp
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
#include <Utility/LessThan.hpp>
#include <Utility/Pair.hpp>
//------------------------------------------------------------------//
#if ET_COMPILER_IS_MSVC && !defined( EA_COMPILER_HAS_C99_FORMAT_MACROS )
//	MSVC complains about *lots* of macro redefinitions in eabase/inttypes.h.
#	define EA_COMPILER_HAS_C99_FORMAT_MACROS
#endif
#include <eastl/vector_map.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename Key, typename Value, class OrderingPredicate = Eldritch2::LessThan<Key>, class Allocator = Eldritch2::ChildAllocator>
	class ArrayMap {
	// - TYPE PUBLISHING ---------------------------------

	private:
		using UnderlyingContainer	= eastl::vector_map<Key, Value, OrderingPredicate, Detail::EaStlAllocatorAdapterMixin<Allocator>, eastl::vector<eastl::pair<Key, Value>, Detail::EaStlAllocatorAdapterMixin<Allocator>>>;

	public:
		using ValueType				= typename UnderlyingContainer::value_type;
		using KeyType				= typename UnderlyingContainer::key_type;
		using MappedType			= typename UnderlyingContainer::mapped_type;
		using OrderingPredicateType = typename UnderlyingContainer::key_compare;
		using AllocatorType			= typename UnderlyingContainer::allocator_type::PublicType;
		using Iterator				= typename UnderlyingContainer::iterator;
		using ReverseIterator		= typename UnderlyingContainer::reverse_iterator;
		using ConstIterator			= typename UnderlyingContainer::const_iterator;
		using ConstReverseIterator	= typename UnderlyingContainer::const_reverse_iterator;
		using SizeType				= typename UnderlyingContainer::size_type;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------
	
	public:
	//! Constructs this @ref ArrayMap instance.
		ArrayMap( const SizeType reservedSizeInElements, const OrderingPredicateType& orderingPredicate, const AllocatorType& allocator = AllocatorType() );
	//! Constructs this @ref ArrayMap instance.
		template <typename InputIterator>
		ArrayMap( InputIterator begin, InputIterator end, const OrderingPredicateType& orderingPredicate, const AllocatorType& allocator = AllocatorType() );
	//! Constructs this @ref ArrayMap instance.
		template <class = eastl::enable_if<eastl::is_copy_constructible<ValueType>::value>::type>
		explicit ArrayMap( const ArrayMap& containerTemplate, const AllocatorType& allocator = AllocatorType() );
	//! Constructs this @ref ArrayMap instance.
		explicit ArrayMap( const AllocatorType& allocator = AllocatorType() );
	//! Constructs this @ref ArrayMap instance.
		ArrayMap( ArrayMap&& );

		~ArrayMap() = default;

	// - ALGORITHMS --------------------------------------

	public:
		ConstIterator	LowerBound( const KeyType& key ) const;
		Iterator		LowerBound( const KeyType& key );

		Iterator		UpperBound( const KeyType& key ) const;
		Iterator		UpperBound( const KeyType& key );
		
		template <typename AlternateKeyType, typename AlternateOrderingPredicate>
		ConstIterator	Find( const AlternateKeyType& key, AlternateOrderingPredicate orderingPredicate ) const;
		template <typename AlternateKeyType, typename AlternateOrderingPredicate>
		Iterator		Find( const AlternateKeyType& key, AlternateOrderingPredicate orderingPredicate );
		ConstIterator	Find( const KeyType& key ) const;
		Iterator		Find( const KeyType& key );

	// - ELEMENT ITERATION -------------------------------

	public:
		ConstReverseIterator	ConstReverseBegin() const;

		ConstReverseIterator	ConstReverseEnd() const;

		ConstReverseIterator	ReverseBegin() const;
		ReverseIterator			ReverseBegin();

		ConstReverseIterator	ReverseEnd() const;
		ReverseIterator			ReverseEnd();

		ConstIterator			ConstBegin() const;

		ConstIterator			ConstEnd() const;

		ConstIterator			Begin() const;
		Iterator				Begin();

		ConstIterator			End() const;
		Iterator				End();

	// - CONTAINER MANIPULATION --------------------------

	public:
		template <class = eastl::enable_if<eastl::is_copy_constructible<ValueType>::value>::type>
		Eldritch2::Pair<Iterator, bool>	Insert( const ValueType& value );
		Eldritch2::Pair<Iterator, bool>	Insert( ValueType&& value );

		template <typename... Args>
		Eldritch2::Pair<Iterator, bool>	Emplace( ConstIterator position, Args&&... args );
		template <typename... Args>
		Eldritch2::Pair<Iterator, bool>	Emplace( Args&&... args );
		
		Iterator						Erase( Iterator begin, Iterator end );
		Iterator						Erase( Iterator position );
		SizeType						Erase( const KeyType& key );

		void							Clear();

	// ---------------------------------------------------

	public:
		template <class = eastl::enable_if<eastl::is_copy_constructible<ValueType>::value>::type>
		ArrayMap&	operator=( const ArrayMap& );
		ArrayMap&	operator=( ArrayMap&& );

	// ---------------------------------------------------

	public:
		void	SetCapacity( SizeType sizeInElements );

		void	Reserve( SizeType sizeInElements );

	// - CONTENT QUERY -----------------------------------

	public:
		SizeType			GetSize();

		bool				IsEmpty() const;

		explicit operator	bool() const;

	// - ORDERING PREDICATE ACCESS -----------------------

	public:
		const OrderingPredicateType&	GetOrderingPredicate() const;
		OrderingPredicateType&			GetOrderingPredicate();

	// - ALLOCATOR ACCESS --------------------------------

	public:
		const AllocatorType&	GetAllocator();

	// - DATA MEMBERS ------------------------------------

	private:
		UnderlyingContainer	_underlyingContainer;
	};

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/Containers/ArrayMap.inl>
//------------------------------------------------------------------//