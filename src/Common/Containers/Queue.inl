/*==================================================================*\
  Queue.inl
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {

template <typename Value, typename Allocator, typename Container>
ETInlineHint Queue<Value, Allocator, Container>::Queue(const AllocatorType& allocator, const Queue<Value, Allocator, Container>& queue) :
	c(allocator, queue.c) {}

// ---------------------------------------------------

template <typename Value, typename Allocator, typename Container>
ETInlineHint Queue<Value, Allocator, Container>::Queue(const AllocatorType& allocator) :
	c(allocator) {}

// ---------------------------------------------------

template <typename Value, typename Allocator, typename Container>
ETInlineHint typename const Queue<Value, Allocator, Container>::ValueType& Queue<Value, Allocator, Container>::Front() const {
	return c.Front();
}

// ---------------------------------------------------

template <typename Value, typename Allocator, typename Container>
ETInlineHint typename Queue<Value, Allocator, Container>::ValueType& Queue<Value, Allocator, Container>::Front() {
	return c.Front();
}

// ---------------------------------------------------

template <typename Value, typename Allocator, typename Container>
ETInlineHint typename const Queue<Value, Allocator, Container>::ValueType& Queue<Value, Allocator, Container>::Back() const {
	return c.Back();
}

// ---------------------------------------------------

template <typename Value, typename Allocator, typename Container>
ETInlineHint typename Queue<Value, Allocator, Container>::ValueType& Queue<Value, Allocator, Container>::Back() {
	return c.Back();
}

// ---------------------------------------------------

template <typename Value, typename Allocator, typename Container>
template <typename... Arguments>
ETInlineHint void Queue<Value, Allocator, Container>::EmplaceBack(Arguments&&... arguments) {
	c.EmplaceBack(eastl::forward<Arguments>(arguments)...);
}

// ---------------------------------------------------

template <typename Value, typename Allocator, typename Container>
ETInlineHint void Queue<Value, Allocator, Container>::Push(const ValueType& value) {
	c.Append(value);
}

// ---------------------------------------------------

template <typename Value, typename Allocator, typename Container>
ETInlineHint void Queue<Value, Allocator, Container>::Push(ValueType&& value) {
	c.Append(eastl::move(value));
}

// ---------------------------------------------------

template <typename Value, typename Allocator, typename Container>
ETInlineHint void Queue<Value, Allocator, Container>::Pop() {
	c.PopFront();
}

// ---------------------------------------------------

template <typename Value, typename Allocator, typename Container>
ETInlineHint typename const Queue<Value, Allocator, Container>::ContainerType& Queue<Value, Allocator, Container>::GetContainer() const {
	return c;
}

// ---------------------------------------------------

template <typename Value, typename Allocator, typename Container>
ETInlineHint typename Queue<Value, Allocator, Container>::ContainerType& Queue<Value, Allocator, Container>::GetContainer() {
	return c;
}

// ---------------------------------------------------

template <typename Value, typename Allocator, typename Container>
ETInlineHint typename const Queue<Value, Allocator, Container>::AllocatorType& Queue<Value, Allocator, Container>::GetAllocator() const {
	return c.GetAllocator();
}

// ---------------------------------------------------

template <typename Value, typename Allocator, typename Container>
ETInlineHint typename Queue<Value, Allocator, Container>::SizeType Queue<Value, Allocator, Container>::GetSize() const {
	return c.GetSize();
}

// ---------------------------------------------------

template <typename Value, typename Allocator, typename Container>
ETInlineHint bool Queue<Value, Allocator, Container>::IsEmpty() const {
	return c.IsEmpty();
}

// ---------------------------------------------------

template <typename Value, typename Allocator, typename Container>
ETInlineHint void Swap(Queue<Value, Allocator, Container>& lhs, Queue<Value, Allocator, Container>& rhs) {
	using ::Eldritch2::Swap;

	Swap(c, queue.c);
}

} // namespace Eldritch2
