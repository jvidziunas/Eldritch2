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
ETInlineHint PriorityQueue<Value, Container, Comparator>::PriorityQueue(const ContainerType& queue, const ComparatorType& sort, InputIterator first, InputIterator last) :
	c(queue),
	comp(sort) {
	c.Insert(c.End(), first, last);
	eastl::make_heap(c.Begin(), c.End(), comp);
}

// ---------------------------------------------------

template <typename Value, class Container, typename Comparator>
template <class InputIterator>
ETInlineHint PriorityQueue<Value, Container, Comparator>::PriorityQueue(ContainerType&& queue, const ComparatorType& sort, InputIterator first, InputIterator last) :
	c(eastl::move(queue)),
	comp(sort) {
	c.Insert(c.End(), first, last);
	eastl::make_heap(c.Begin(), c.End(), comp);
}

// ---------------------------------------------------

template <typename Value, class Container, typename Comparator>
template <class Allocator, typename InputIterator>
ETInlineHint PriorityQueue<Value, Container, Comparator>::PriorityQueue(const Allocator& allocator, const ComparatorType& sort, InputIterator first, InputIterator last) :
	c(allocator, first, last),
	comp(sort) {}

// ---------------------------------------------------

template <typename Value, class Container, typename Comparator>
template <class Allocator>
ETInlineHint PriorityQueue<Value, Container, Comparator>::PriorityQueue(const Allocator& allocator, const ComparatorType& sort) :
	c(allocator),
	comp(sort) {}

// ---------------------------------------------------

template <typename Value, class Container, typename Comparator>
ETInlineHint PriorityQueue<Value, Container, Comparator>::PriorityQueue(const ContainerType& queue, const ComparatorType& sort) :
	c(queue),
	comp(sort) {}

// ---------------------------------------------------

template <typename Value, class Container, typename Comparator>
ETInlineHint PriorityQueue<Value, Container, Comparator>::PriorityQueue(ContainerType&& queue, const ComparatorType& sort) :
	c(eastl::move(queue)),
	comp(sort) {}

// ---------------------------------------------------

template <typename Value, class Container, typename Comparator>
template <class Allocator>
ETInlineHint PriorityQueue<Value, Container, Comparator>::PriorityQueue(const Allocator& allocator, const PriorityQueue& queue) :
	c(allocator, queue.c),
	comp(queue.comp) {}

// ---------------------------------------------------

template <typename Value, class Container, typename Comparator>
template <class Allocator>
ETInlineHint PriorityQueue<Value, Container, Comparator>::PriorityQueue(const Allocator& allocator, PriorityQueue&& queue) :
	c(allocator, eastl::move(queue.c)),
	comp(eastl::move(queue.comp)) {}

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
ETInlineHint void PriorityQueue<Value, Container, Comparator>::Push(const ValueType& value) {
	c.Append(value);
	std::push_heap(c.Begin(), c.End(), comp);
}

// ---------------------------------------------------

template <typename Value, class Container, typename Comparator>
ETInlineHint void PriorityQueue<Value, Container, Comparator>::Push(ValueType&& value) {
	c.Append(eastl::move(value));
	std::push_heap(c.Begin(), c.End(), comp);
}

// ---------------------------------------------------

template <typename Value, class Container, typename Comparator>
template <class... Args>
ETInlineHint void PriorityQueue<Value, Container, Comparator>::Emplace(Args&&... args) {
	c.EmplaceBack(eastl::forward<Args>(args)...);
	std::push_heap(c.Begin(), c.End(), comp);
}

// ---------------------------------------------------

template <typename Value, class Container, typename Comparator>
ETInlineHint void PriorityQueue<Value, Container, Comparator>::Pop(ValueType& value) {
	value = eastl::move(c.Front());
	c.Pop();
}

// ---------------------------------------------------

template <typename Value, class Container, typename Comparator>
ETInlineHint void PriorityQueue<Value, Container, Comparator>::Pop() {
	eastl::pop_heap(c.Begin(), c.End(), comp);
	c.Pop();
}

// ---------------------------------------------------

template <typename Value, class Container, typename Comparator>
ETInlineHint void PriorityQueue<Value, Container, Comparator>::Change(SizeType index) {
	eastl::change_heap(c.Begin(), c.End(), index, comp);
}

// ---------------------------------------------------

template <typename Value, class Container, typename Comparator>
ETInlineHint void PriorityQueue<Value, Container, Comparator>::Remove(SizeType index) {
	eastl::remove_heap(c.Begin(), c.End(), index, comp);
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
ETInlineHint void Swap(PriorityQueue<Value, Container, Comparator>& queue0, PriorityQueue<Value, Container, Comparator>& queue1) {
	using ::Eldritch2::Swap;

	Swap(queue0.c, queue1.c);
	Swap(queue0.comp, queue1.comp);
}

} // namespace Eldritch2
