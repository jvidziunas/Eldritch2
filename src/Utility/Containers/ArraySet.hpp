/*==================================================================*\
  ArraySet.hpp
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
#include <eastl/vector_set.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename Value, class OrderingPredicate = Eldritch2::LessThan<Value>, class Allocator = Eldritch2::ChildAllocator>
	class ArraySet {
	// - TYPE PUBLISHING ---------------------------------

	protected:
		using UnderlyingContainer	= eastl::vector_set<Value, OrderingPredicate, Detail::EaStlAllocatorAdapterMixin<Allocator>>;

	public:
		using ValueType				= typename UnderlyingContainer::value_type;
		using AllocatorType			= typename UnderlyingContainer::allocator_type::PublicType;
		using ConstIterator			= typename UnderlyingContainer::const_iterator;
		using Iterator				= typename UnderlyingContainer::iterator;
		using SizeType				= typename UnderlyingContainer::size_type;
		using OrderingPredicateType	= OrderingPredicate;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref ArraySet instance.
		ArraySet( SizeType reservedSizeInElements, const OrderingPredicateType& orderingPredicate = OrderingPredicateType(), const AllocatorType& allocator = AllocatorType() );
	//!	Constructs this @ref ArraySet instance.
		template <typename InputIterator>
		ArraySet( InputIterator begin, InputIterator end, const OrderingPredicateType& orderingPredicate = OrderingPredicateType(), const AllocatorType& allocator = AllocatorType() );
	//!	Constructs this @ref ArraySet instance.
		template <class = eastl::enable_if<eastl::is_copy_constructible<ValueType>::value>::type>
		ArraySet( const ArraySet& containerTemplate, const AllocatorType& allocator = AllocatorType() );
	//!	Constructs this @ref ArraySet instance.
		explicit ArraySet( const AllocatorType& allocator = AllocatorType() );
	//!	Constructs this @ref ArraySet instance.
		ArraySet( ArraySet&& );

		~ArraySet() = default;

	// - ALGORITHMS --------------------------------------

	public:
		ConstIterator	LowerBound( const ValueType& value ) const;
		Iterator		LowerBound( const ValueType& value );

		ConstIterator	UpperBound( const ValueType& value ) const;
		Iterator		UpperBound( const ValueType& value );

		template <typename AlternateValue, typename BinaryPredicate>
		ConstIterator	Find( const AlternateValue& value, BinaryPredicate comparisonPredicate ) const;
		template <typename AlternateValue, typename BinaryPredicate>
		Iterator		Find( const AlternateValue& value, BinaryPredicate comparisonPredicate );
		ConstIterator	Find( const ValueType& value ) const;
		Iterator		Find( const ValueType& value );

		template <typename AlternateValue, typename BinaryPredicate>
		bool			Contains( const AlternateValue& value, BinaryPredicate comparisonPredicate ) const;
		bool			Contains( const ValueType& value ) const;

		template <typename Predicate>
		void			RemoveIf( Predicate predicate );

	// - ELEMENT ITERATION -------------------------------

	public:
		ConstIterator	ConstBegin() const;

		ConstIterator	ConstEnd() const;

		ConstIterator	Begin() const;
		Iterator		Begin();

		ConstIterator	End() const;
		Iterator		End();

	// - CONTAINER DUPLICATION ---------------------------

	public:
		template <class = eastl::enable_if<eastl::is_copy_constructible<ValueType>::value>::type>
		ArraySet&	operator=( const ArraySet& );
		ArraySet&	operator=( ArraySet&& );

		void		Swap( ArraySet& other );

	// - CONTAINER MANIPULATION --------------------------

	public:
		template <typename... Arguments>
		Eldritch2::Pair<Iterator, bool>	Emplace( Arguments&&... arguments );

		template <class = eastl::enable_if<eastl::is_copy_constructible<ValueType>::value>::type>
		Eldritch2::Pair<Iterator, bool>	Insert( const ValueType& value );
		Eldritch2::Pair<Iterator, bool>	Insert( ValueType&& value );

		Iterator						Erase( Iterator beginPosition, Iterator endPosition );
		Iterator						Erase( Iterator position );
		SizeType						Erase( const ValueType& value );

		void							Clear();

	// - CONTENT QUERY -----------------------------------

	public:
		SizeType			GetSize() const;

		bool				IsEmpty() const;

		explicit operator	bool() const;

	// - CAPACITY QUERY ----------------------------------

	public:
		SizeType	GetCapacity() const;

		void		SetCapacity( SizeType capacity );

		void		Reserve( SizeType sizeHint );

	// - ORDERING PREDICATE ACCESS -----------------------

	public:
		const OrderingPredicateType&	GetOrderingPredicate() const;

	// - ALLOCATOR ACCESS --------------------------------

	public:
		const AllocatorType&	GetAllocator() const;

	// - DATA MEMBERS ------------------------------------

	private:
		UnderlyingContainer	_underlyingContainer;
	};

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/Containers/ArraySet.inl>
//------------------------------------------------------------------//