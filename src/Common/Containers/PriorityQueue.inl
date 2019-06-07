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
//------------------------------------------------------------------//

namespace Eldritch2 {

template <typename Value, class Container, typename Comparator>
template <typename InputIterator>
ETInlineHint ETForceInlineHint PriorityQueue<Value, Container, Comparator>::PriorityQueue(const ContainerType& queue, const ComparatorType& sort, InputIterator first, InputIterator last) :
	c(queue),
	comp(sort) {
	c.Insert(c.End(), first, last);
	eastl::make_heap(c.Begin(), c.End(), comp);
}

// ---------------------------------------------------

template <typename Value, class Container, typename Comparator>
template <class InputIterator>
ETInlineHint ETForceInlineHint PriorityQueue<Value, Container, Comparator>::PriorityQueue(ContainerType&& queue, const ComparatorType& sort, InputIterator first, InputIterator last) :
	c(Move(queue)),
	comp(sort) {
	c.Insert(c.End(), first, last);
	eastl::make_heap(c.Begin(), c.End(), comp);
}

// ---------------------------------------------------

template <typename Value, class Container, typename Comparator>
template <class Allocator, typename InputIterator>
ETInlineHint ETForceInlineHint PriorityQueue<Value, Container, Comparator>::PriorityQueue(const Allocator& allocator, const ComparatorType& sort, InputIterator first, InputIterator last) :
	c(allocator, first, last),
	comp(sort) {}

// ---------------------------------------------------

template <typename Value, class Container, typename Comparator>
template <class Allocator>
ETInlineHint ETForceInlineHint PriorityQueue<Value, Container, Comparator>::PriorityQueue(const Allocator& allocator, const ComparatorType& sort) :
	c(allocator),
	comp(sort) {}

// ---------------------------------------------------

template <typename Value, class Container, typename Comparator>
ETInlineHint ETForceInlineHint PriorityQueue<Value, Container, Comparator>::PriorityQueue(const ContainerType& queue, const ComparatorType& sort) :
	c(queue),
	comp(sort) {}

// ---------------------------------------------------

template <typename Value, class Container, typename Comparator>
ETInlineHint ETForceInlineHint PriorityQueue<Value, Container, Comparator>::PriorityQueue(ContainerType&& queue, const ComparatorType& sort) :
	c(Move(queue)),
	comp(sort) {}

// ---------------------------------------------------

template <typename Value, class Container, typename Comparator>
template <class Allocator>
ETInlineHint ETForceInlineHint PriorityQueue<Value, Container, Comparator>::PriorityQueue(const Allocator& allocator, const PriorityQueue& queue) :
	c(allocator, queue.c),
	comp(queue.comp) {}

// ---------------------------------------------------

template <typename Value, class Container, typename Comparator>
template <class Allocator>
ETInlineHint ETForceInlineHint PriorityQueue<Value, Container, Comparator>::PriorityQueue(const Allocator& allocator, PriorityQueue&& queue) :
	c(allocator, Move(queue.c)),
	comp(Move(queue.comp)) {}

// ---------------------------------------------------

template <typename Value, class Container, typename Comparator>
ETInlineHint ETForceInlineHint typename PriorityQueue<Value, Container, Comparator>::SizeType PriorityQueue<Value, Container, Comparator>::GetSize() const {
	return c.GetSize();
}

// ---------------------------------------------------

template <typename Value, class Container, typename Comparator>
ETInlineHint ETForceInlineHint bool PriorityQueue<Value, Container, Comparator>::IsEmpty() const {
	return c.IsEmpty();
}

// ---------------------------------------------------

template <typename Value, class Container, typename Comparator>
ETInlineHint ETForceInlineHint PriorityQueue<Value, Container, Comparator>::operator bool() const {
	return bool(c);
}

// ---------------------------------------------------

template <typename Value, class Container, typename Comparator>
ETInlineHint ETForceInlineHint typename PriorityQueue<Value, Container, Comparator>::ConstReference PriorityQueue<Value, Container, Comparator>::Top() const {
	return c.Front();
}

// ---------------------------------------------------

template <typename Value, class Container, typename Comparator>
ETInlineHint ETForceInlineHint typename PriorityQueue<Value, Container, Comparator>::Reference PriorityQueue<Value, Container, Comparator>::Top() {
	return c.Front();
}

// ---------------------------------------------------

template <typename Value, class Container, typename Comparator>
ETInlineHint ETForceInlineHint void PriorityQueue<Value, Container, Comparator>::Push(const ValueType& value) {
	c.Append(value);
	eastl::push_heap(c.Begin(), c.End(), comp);
}

// ---------------------------------------------------

template <typename Value, class Container, typename Comparator>
ETInlineHint ETForceInlineHint void PriorityQueue<Value, Container, Comparator>::Push(ValueType&& value) {
	c.Append(Move(value));
	eastl::push_heap(c.Begin(), c.End(), comp);
}

// ---------------------------------------------------

template <typename Value, class Container, typename Comparator>
template <class... Args>
ETInlineHint ETForceInlineHint void PriorityQueue<Value, Container, Comparator>::Emplace(Args&&... args) {
	c.EmplaceBack(Forward<Args>(args)...);
	eastl::push_heap(c.Begin(), c.End(), comp);
}

// ---------------------------------------------------

template <typename Value, class Container, typename Comparator>
ETInlineHint ETForceInlineHint void PriorityQueue<Value, Container, Comparator>::Pop(ValueType& outValue) {
	outValue = Move(c.Front());
	eastl::pop_heap(c.Begin(), c.End(), comp);
	c.Pop();
}

// ---------------------------------------------------

template <typename Value, class Container, typename Comparator>
ETInlineHint ETForceInlineHint void PriorityQueue<Value, Container, Comparator>::Pop() {
	eastl::pop_heap(c.Begin(), c.End(), comp);
	c.Pop();
}

// ---------------------------------------------------

template <typename Value, class Container, typename Comparator>
ETInlineHint ETForceInlineHint void PriorityQueue<Value, Container, Comparator>::Change(SizeType index) {
	eastl::change_heap(c.Begin(), c.End(), index, comp);
}

// ---------------------------------------------------

template <typename Value, class Container, typename Comparator>
ETInlineHint ETForceInlineHint void PriorityQueue<Value, Container, Comparator>::Remove(SizeType index) {
	eastl::remove_heap(c.Begin(), c.End(), index, comp);
}

// ---------------------------------------------------

template <typename Value, class Container, typename Comparator>
ETInlineHint ETForceInlineHint typename const PriorityQueue<Value, Container, Comparator>::ContainerType& PriorityQueue<Value, Container, Comparator>::GetContainer() const {
	return c;
}

// ---------------------------------------------------

template <typename Value, class Container, typename Comparator>
ETInlineHint ETForceInlineHint typename PriorityQueue<Value, Container, Comparator>::ContainerType& PriorityQueue<Value, Container, Comparator>::GetContainer() {
	return c;
}

// ---------------------------------------------------

template <typename Value, class Container, typename Comparator>
ETInlineHint ETForceInlineHint void Swap(PriorityQueue<Value, Container, Comparator>& queue0, PriorityQueue<Value, Container, Comparator>& queue1) {
	using ::Eldritch2::Swap;

	Swap(queue0.c, queue1.c);
	Swap(queue0.comp, queue1.comp);
}

} // namespace Eldritch2
