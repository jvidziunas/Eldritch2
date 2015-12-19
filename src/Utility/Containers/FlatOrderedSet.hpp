/*==================================================================*\
  FlatOrderedSet.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/ResizableArray.hpp>
#include <Utility/Less.hpp>
#include <Utility/Pair.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename StoredObject, class OrderingPredicate = ::Eldritch2::Less<StoredObject>, class Allocator = ::Eldritch2::ChildAllocator>
	class FlatOrderedSet : private ::Eldritch2::ResizableArray<StoredObject, Allocator> {
	// - TYPE PUBLISHING ---------------------------------

	protected:
		using UnderlyingContainer					= ::Eldritch2::ResizableArray<StoredObject, Allocator>;

	public:
		using UnderlyingContainer::ValueType;
		using OrderingPredicateType					= OrderingPredicate;
		using UnderlyingContainer::AllocatorType;
		using UnderlyingContainer::Iterator;
		using UnderlyingContainer::ConstIterator;
		using InsertResult							= ::Eldritch2::Pair<Iterator, bool>;
		using UnderlyingContainer::SizeType;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		ETInlineHint explicit FlatOrderedSet( AllocatorType&& allocator = AllocatorType() );
		//!	Constructs this @ref FlatOrderedSet instance.
		ETInlineHint FlatOrderedSet( const SizeType reservedSizeInElements, OrderingPredicateType&& orderingPredicate = OrderingPredicateType(), AllocatorType&& allocator = AllocatorType() );
		//!	Constructs this @ref FlatOrderedSet instance.
		template <typename InputIterator>
		ETInlineHint FlatOrderedSet( InputIterator begin, InputIterator end, OrderingPredicateType&& orderingPredicate = OrderingPredicateType(), AllocatorType&& allocator = AllocatorType() );
		//!	Constructs this @ref FlatOrderedSet instance.
		template <class AlternateAllocator>
		ETInlineHint FlatOrderedSet( const ::Eldritch2::FlatOrderedSet<StoredObject, OrderingPredicate, AlternateAllocator>& containerTemplate, AllocatorType&& allocator = AllocatorType() );
		//!	Constructs this @ref FlatOrderedSet instance.
		ETInlineHint FlatOrderedSet( ::Eldritch2::FlatOrderedSet<StoredObject, OrderingPredicate, Allocator>&& moveSource );

		ETInlineHint ~FlatOrderedSet() = default;

	// - ALGORITHMS --------------------------------------

		ETInlineHint Iterator		LowerBound( const ValueType& value );
		ETInlineHint ConstIterator	LowerBound( const ValueType& value ) const;

		ETInlineHint Iterator		UpperBound( const ValueType& value );
		ETInlineHint Iterator		UpperBound( const ValueType& value ) const;

		ETInlineHint Iterator		Find( const ValueType& value );
		ETInlineHint ConstIterator	Find( const ValueType& value ) const;

		ETInlineHint bool			Contains( const ValueType& value ) const;

		using UnderlyingContainer::RemoveIf;

	// - ELEMENT ITERATION -------------------------------

		using UnderlyingContainer::Begin;
		using UnderlyingContainer::ConstBegin;
		using UnderlyingContainer::End;
		using UnderlyingContainer::ConstEnd;

	// - CONTAINER DUPLICATION ---------------------------

		ETInlineHint void	CloneFrom( const ::Eldritch2::FlatOrderedSet<StoredObject, OrderingPredicate, Allocator>& containerTemplate );

		ETInlineHint void	Swap( ::Eldritch2::FlatOrderedSet<StoredObject, OrderingPredicate, Allocator>& other );

	// - CONTAINER MANIPULATION --------------------------

		ETInlineHint InsertResult	Insert( const ValueType& value );

		ETInlineHint SizeType		Erase( const ValueType& value );
		ETInlineHint Iterator		Erase( const Iterator position );
		ETInlineHint void			Erase( Iterator beginPosition, Iterator endPosition );

		using UnderlyingContainer::EraseAndDispose;

		using UnderlyingContainer::Clear;
		using UnderlyingContainer::ClearAndDispose;

	// - CONTENT QUERY -----------------------------------

		using UnderlyingContainer::Size;
		using UnderlyingContainer::IsEmpty;
		using UnderlyingContainer::operator bool;

	// - CAPACITY QUERY ----------------------------------

		using UnderlyingContainer::GetCapacity;
		using UnderlyingContainer::SetCapacity;
		using UnderlyingContainer::Reserve;
		using UnderlyingContainer::GetHighWatermark;

	// - ORDERING PREDICATE ACCESS -----------------------

		ETInlineHint OrderingPredicateType&			GetOrderingPredicate();
		ETInlineHint const OrderingPredicateType&	GetOrderingPredicate() const;

	// - ALLOCATOR ACCESS --------------------------------

		using UnderlyingContainer::GetAllocator;

	// - DATA MEMBERS ------------------------------------

	private:
		OrderingPredicateType	_orderingPredicate;
	};

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/Containers/FlatOrderedSet.inl>
//------------------------------------------------------------------//