/*==================================================================*\
  PriorityQueue.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <eastl/heap.h>
#include <algorithm>
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename Value, class Container, typename Comparator>
	template <typename InputIterator>
	ETInlineHint PriorityQueue<Value, Container, Comparator>::PriorityQueue(
		InputIterator first,
		InputIterator last,
		const ComparatorType& sorter,
		const ContainerType& container
	) : PriorityQueue( sorter, container ) {
		c.Insert( c.End(), first, last );
		eastl::make_heap( c.Begin(), c.End(), comp );
	}

// ---------------------------------------------------

	template <typename Value, class Container, typename Comparator>
	template <class InputIterator>
	ETInlineHint PriorityQueue<Value, Container, Comparator>::PriorityQueue(
		InputIterator first,
		InputIterator last,
		const ComparatorType& sorter,
		ContainerType&& container
	) : PriorityQueue( sorter, eastl::move( container ) ) {
		c.Insert( c.End(), first, last );
		eastl::make_heap( c.Begin(), c.End(), comp );
	}

// ---------------------------------------------------

	template <typename Value, class Container, typename Comparator>
	template <typename InputIterator>
	ETInlineHint PriorityQueue<Value, Container, Comparator>::PriorityQueue(
		InputIterator first,
		InputIterator last,
		const ComparatorType& sorter
	) : c( first, last ),
		comp( sorter ) {
		eastl::make_heap( c.Begin(), c.End(), comp );
	}

// ---------------------------------------------------

	template <typename Value, class Container, typename Comparator>
	template <typename InputIterator>
	ETInlineHint PriorityQueue<Value, Container, Comparator>::PriorityQueue(
		InputIterator first,
		InputIterator last
	) : c( first, last ),
		comp() {}

// ---------------------------------------------------

	template <typename Value, class Container, typename Comparator>
	ETInlineHint PriorityQueue<Value, Container, Comparator>::PriorityQueue(
		const ComparatorType& sorter,
		ContainerType&& container
	) : c( eastl::move( container ) ),
		comp( sorter ) {}

// ---------------------------------------------------

	template <typename Value, class Container, typename Comparator>
	ETInlineHint PriorityQueue<Value, Container, Comparator>::PriorityQueue(
		const ComparatorType& sorter,
		const ContainerType& container
	) : c( container ),
		comp( sorter ) {}

// ---------------------------------------------------

	template <typename Value, class Container, typename Comparator>
	template <class Allocator>
	ETInlineHint PriorityQueue<Value, Container, Comparator>::PriorityQueue(
		const PriorityQueue<Value, Container, Comparator>& queue,
		const Allocator& allocator
	) : c( queue.c, allocator ),
		comp( queue.comp ) {}

// ---------------------------------------------------

	template <typename Value, class Container, typename Comparator>
	template <class Allocator>
	ETInlineHint PriorityQueue<Value, Container, Comparator>::PriorityQueue(
		PriorityQueue<Value, Container, Comparator>&& queue,
		const Allocator& allocator
	) : c( eastl::move( queue.c ), allocator ),
		comp( eastl::move( comp ) ) {}

// ---------------------------------------------------

	template <typename Value, class Container, typename Comparator>
	template <class Allocator>
	ETInlineHint PriorityQueue<Value, Container, Comparator>::PriorityQueue(
		const Allocator& allocator
	) : c( allocator ),
		comp() {}

// ---------------------------------------------------

	template <typename Value, class Container, typename Comparator>
	ETInlineHint PriorityQueue<Value, Container, Comparator>::PriorityQueue(
		const ComparatorType& sorter
	) : c(),
		comp( sorter ) {}

// ---------------------------------------------------

	template <typename Value, class Container, typename Comparator>
	ETInlineHint PriorityQueue<Value, Container, Comparator>::PriorityQueue() : c(), comp() {}

// ---------------------------------------------------

	template <typename Value, class Container, typename Comparator>
	ETInlineHint typename PriorityQueue<Value, Container, Comparator>::SizeType PriorityQueue<Value, Container, Comparator>::GetSize() const {
		return c.GetSize();
	}

// ---------------------------------------------------

	template <typename Value, class Container, typename Comparator>
	ETInlineHint bool PriorityQueue<Value, Container, Comparator>::IsEmpty() const {
		return c.IsEmpty();
	}

// ---------------------------------------------------

	template <typename Value, class Container, typename Comparator>
	ETInlineHint PriorityQueue<Value, Container, Comparator>::operator bool() const {
		return static_cast<bool>(c);
	}

// ---------------------------------------------------

	template <typename Value, class Container, typename Comparator>
	ETInlineHint typename PriorityQueue<Value, Container, Comparator>::ConstReference PriorityQueue<Value, Container, Comparator>::Top() const {
		return c.Front();
	}

// ---------------------------------------------------

	template <typename Value, class Container, typename Comparator>
	ETInlineHint typename PriorityQueue<Value, Container, Comparator>::Reference PriorityQueue<Value, Container, Comparator>::Top() {
		return c.Front();
	}

// ---------------------------------------------------

	template <typename Value, class Container, typename Comparator>
	ETInlineHint void PriorityQueue<Value, Container, Comparator>::Push( const ValueType& value ) {
		c.Append( value );
		std::push_heap( c.Begin(), c.End(), comp );
	}

// ---------------------------------------------------

	template <typename Value, class Container, typename Comparator>
	ETInlineHint void PriorityQueue<Value, Container, Comparator>::Push( ValueType&& value ) {
		c.Append( eastl::move( value ) );
		std::push_heap( c.Begin(), c.End(), comp );
	}

// ---------------------------------------------------

	template <typename Value, class Container, typename Comparator>
	template <class... Args>
	ETInlineHint void PriorityQueue<Value, Container, Comparator>::Emplace( Args&&... args ) {
		c.EmplaceBack( eastl::forward<Args>( args )... );
		std::push_heap( c.Begin(), c.End(), comp );
	}

// ---------------------------------------------------

	template <typename Value, class Container, typename Comparator>
	ETInlineHint void PriorityQueue<Value, Container, Comparator>::Pop( ValueType& value ) {
		value = eastl::move( c.Front() );
		c.Pop();
	}

// ---------------------------------------------------

	template <typename Value, class Container, typename Comparator>
	ETInlineHint void PriorityQueue<Value, Container, Comparator>::Pop() {
		eastl::pop_heap( c.Begin(), c.End(), comp );
		c.Pop();
	}

// ---------------------------------------------------

	template <typename Value, class Container, typename Comparator>
	ETInlineHint void PriorityQueue<Value, Container, Comparator>::Change( SizeType index ) {
		eastl::change_heap( c.Begin(), c.End(), index, comp );
	}

// ---------------------------------------------------

	template <typename Value, class Container, typename Comparator>
	ETInlineHint void PriorityQueue<Value, Container, Comparator>::Remove( SizeType index ) {
		eastl::remove_heap( c.Begin(), c.End(), index, comp );
	}

// ---------------------------------------------------

	template <typename Value, class Container, typename Comparator>
	ETInlineHint typename const PriorityQueue<Value, Container, Comparator>::ContainerType& PriorityQueue<Value, Container, Comparator>::GetContainer() const {
		return c;
	}

// ---------------------------------------------------

	template <typename Value, class Container, typename Comparator>
	ETInlineHint typename PriorityQueue<Value, Container, Comparator>::ContainerType& PriorityQueue<Value, Container, Comparator>::GetContainer() {
		return c;
	}

// ---------------------------------------------------

	template <typename Value, class Container, typename Comparator>
	ETInlineHint void Swap( PriorityQueue<Value, Container, Comparator>& queue0, PriorityQueue<Value, Container, Comparator>& queue1 ) {
		Swap( queue0.c,		queue1.c );
		Swap( queue0.comp,	queue1.comp );
	}

}	// namespace Eldritch2