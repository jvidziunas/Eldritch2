/*==================================================================*\
  FlatOrderedSet.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/ResizableArray.hpp>
//------------------------------------------------------------------//
#include <rdestl/functional.h>
#include <rdestl/pair.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename StoredObject, class OrderingPredicate = ::rde::less<StoredObject>, class Allocator = ::Eldritch2::ChildAllocator>
	class FlatOrderedSet : private ::Eldritch2::ResizableArray<StoredObject, Allocator> {
	// - TYPE PUBLISHING ---------------------------------

	protected:
		typedef ::Eldritch2::ResizableArray<StoredObject, Allocator>	UnderlyingContainer;

	public:
		using UnderlyingContainer::ValueType;
		typedef typename OrderingPredicate								OrderingPredicateType;
		using UnderlyingContainer::AllocatorType;
		using UnderlyingContainer::Iterator;
		using UnderlyingContainer::ConstIterator;
		typedef ::rde::pair<Iterator, bool>								InsertResult;
		using UnderlyingContainer::SizeType;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		// Constructs this OrderedSet instance.
		template <typename... AllocatorConstructorArguments>
		ETInlineHint FlatOrderedSet( const SizeType reservedSizeInElements, OrderingPredicateType orderingPredicate, AllocatorConstructorArguments&&... allocatorConstructorArguments );
		// Constructs this OrderedSet instance.
		template <typename InputIterator, typename... AllocatorConstructorArguments>
		ETInlineHint FlatOrderedSet( InputIterator begin, InputIterator end, OrderingPredicateType orderingPredicate, AllocatorConstructorArguments&&... allocatorConstructorArguments );
		// Constructs this OrderedSet instance.
		template <class AlternateAllocator, typename... AllocatorConstructorArguments>
		ETInlineHint FlatOrderedSet( const ::Eldritch2::FlatOrderedSet<StoredObject, OrderingPredicate, AlternateAllocator>& containerTemplate, AllocatorConstructorArguments&&... allocatorConstructorArguments );

		// Destroys this OrderedSet instance.
		ETInlineHint ~FlatOrderedSet();

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