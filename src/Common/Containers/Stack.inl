/*==================================================================*\
  Stack.inl
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

template <typename Value, class Container>
template <class AllocatorType>
ETInlineHint Stack<Value, Container>::Stack(const Stack<Value, Container>& stack, const AllocatorType& allocator) :
	c(stack.c, allocator) {}

// ---------------------------------------------------

template <typename Value, class Container>
template <class AllocatorType>
ETInlineHint Stack<Value, Container>::Stack(Stack&& stack, const AllocatorType& allocator) :
	c(eastl::move(stack.c), allocator) {}

// ---------------------------------------------------

template <typename Value, class Container>
template <class AllocatorType>
ETInlineHint Stack<Value, Container>::Stack(const AllocatorType& allocator) :
	c(allocator) {}

// ---------------------------------------------------

template <typename Value, class Container>
ETInlineHint Stack<Value, Container>::Stack(const ContainerType& stack) :
	c(stack) {}

// ---------------------------------------------------

template <typename Value, class Container>
ETInlineHint Stack<Value, Container>::Stack(ContainerType&& stack) :
	c(eastl::move(stack)) {}

// ---------------------------------------------------

template <typename Value, class Container>
ETInlineHint typename Stack<Value, Container>::SizeType Stack<Value, Container>::GetSize() const {
	return c.GetSize();
}

// ---------------------------------------------------

template <typename Value, class Container>
ETInlineHint bool Stack<Value, Container>::IsEmpty() const {
	return c.IsEmpty();
}

// ---------------------------------------------------

template <typename Value, class Container>
ETInlineHint typename Stack<Value, Container>::ConstReference Stack<Value, Container>::Top() const {
	return c.Back();
}

// ---------------------------------------------------

template <typename Value, class Container>
ETInlineHint typename Stack<Value, Container>::Reference Stack<Value, Container>::Top() {
	return c.Back();
}

// ---------------------------------------------------

template <typename Value, class Container>
ETInlineHint void Stack<Value, Container>::Push(const ValueType& value) {
	c.Append(value);
}

// ---------------------------------------------------

template <typename Value, class Container>
ETInlineHint void Stack<Value, Container>::Push(ValueType&& x) {
	c.Append(eastl::move(value));
}

// ---------------------------------------------------

template <typename Value, class Container>
template <class... Args>
ETInlineHint void Stack<Value, Container>::EmplaceBack(Args&&... args) {
	c.EmplaceBack(eastl::forward<Args>(args)...);
}

// ---------------------------------------------------

template <typename Value, class Container>
ETInlineHint void Stack<Value, Container>::Pop() {
	c.Pop();
}

// ---------------------------------------------------

template <typename Value, class Container>
ETInlineHint typename const Stack<Value, Container>::ContainerType& Stack<Value, Container>::GetContainer() const {
	return c;
}

// ---------------------------------------------------

template <typename Value, class Container>
ETInlineHint typename Stack<Value, Container>::ContainerType& Stack<Value, Container>::GetContainer() {
	return c;
}

// ---------------------------------------------------

template <typename Value, class Container>
ETInlineHint void Swap(Stack<Value, Container>& stack0, Stack<Value, Container>& stack1) {
	Swap(stack0.c, stack1.c);
}

} // namespace Eldritch2
