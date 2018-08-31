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
ETInlineHint ETForceInlineHint Stack<Value, Container>::Stack(const AllocatorType& allocator, const Stack<Value, Container>& stack) :
	c(allocator, stack.c) {}

// ---------------------------------------------------

template <typename Value, class Container>
template <class AllocatorType>
ETInlineHint ETForceInlineHint Stack<Value, Container>::Stack(const AllocatorType& allocator, Stack&& stack) :
	c(allocator, eastl::move(stack.c)) {}

// ---------------------------------------------------

template <typename Value, class Container>
template <class AllocatorType>
ETInlineHint ETForceInlineHint Stack<Value, Container>::Stack(const AllocatorType& allocator) :
	c(allocator) {}

// ---------------------------------------------------

template <typename Value, class Container>
ETInlineHint ETForceInlineHint Stack<Value, Container>::Stack(const ContainerType& stack) :
	c(stack) {}

// ---------------------------------------------------

template <typename Value, class Container>
ETInlineHint ETForceInlineHint Stack<Value, Container>::Stack(ContainerType&& stack) :
	c(eastl::move(stack)) {}

// ---------------------------------------------------

template <typename Value, class Container>
ETInlineHint ETForceInlineHint typename Stack<Value, Container>::SizeType Stack<Value, Container>::GetSize() const {
	return c.GetSize();
}

// ---------------------------------------------------

template <typename Value, class Container>
ETInlineHint ETForceInlineHint bool Stack<Value, Container>::IsEmpty() const {
	return c.IsEmpty();
}

// ---------------------------------------------------

template <typename Value, class Container>
ETInlineHint ETForceInlineHint Stack<Value, Container>::operator bool() const {
	return !c.IsEmpty();
}

// ---------------------------------------------------

template <typename Value, class Container>
ETInlineHint ETForceInlineHint typename Stack<Value, Container>::ConstReference Stack<Value, Container>::Top() const {
	return c.Back();
}

// ---------------------------------------------------

template <typename Value, class Container>
ETInlineHint ETForceInlineHint typename Stack<Value, Container>::Reference Stack<Value, Container>::Top() {
	return c.Back();
}

// ---------------------------------------------------

template <typename Value, class Container>
ETInlineHint ETForceInlineHint void Stack<Value, Container>::Push(const ValueType& value) {
	c.Append(value);
}

// ---------------------------------------------------

template <typename Value, class Container>
ETInlineHint ETForceInlineHint void Stack<Value, Container>::Push(ValueType&& value) {
	c.Append(eastl::move(value));
}

// ---------------------------------------------------

template <typename Value, class Container>
template <class... Arguments>
ETInlineHint ETForceInlineHint void Stack<Value, Container>::EmplaceBack(Arguments&&... arguments) {
	c.EmplaceBack(eastl::forward<Arguments>(args)...);
}

// ---------------------------------------------------

template <typename Value, class Container>
ETInlineHint ETForceInlineHint void Stack<Value, Container>::Pop() {
	c.Pop();
}

// ---------------------------------------------------

template <typename Value, class Container>
ETInlineHint ETForceInlineHint typename const Stack<Value, Container>::ContainerType& Stack<Value, Container>::GetContainer() const {
	return c;
}

// ---------------------------------------------------

template <typename Value, class Container>
ETInlineHint ETForceInlineHint typename Stack<Value, Container>::ContainerType& Stack<Value, Container>::GetContainer() {
	return c;
}

// ---------------------------------------------------

template <typename Value, class Container>
ETInlineHint ETForceInlineHint void Swap(Stack<Value, Container>& lhs, Stack<Value, Container>& rhs) {
	using ::Eldritch2::Swap;

	Swap(lhs.c, rhs.c);
}

} // namespace Eldritch2
