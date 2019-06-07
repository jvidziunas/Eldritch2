/*==================================================================*\
  ArrayList.inl
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {

template <typename ForwardIterator, typename UnaryPredicate>
ForwardIterator RemoveIf(ForwardIterator begin, ForwardIterator end, UnaryPredicate filter);

template <typename InputIterator, typename Value>
InputIterator Find(InputIterator begin, InputIterator end, const Value& value) ETNoexceptHintIf(IsNoThrowCallable<EqualTo<Value>, const Value&, typename DereferenceType<InputIterator>::Type>());

template <typename T, typename... U>
ETCpp14Constexpr ETPureFunctionHint ETNeverThrowsHint T Minimum(T, U...) ETNoexceptHint;

} // namespace Eldritch2

namespace Eldritch2 {

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint ArrayList<Value, Allocator>::ArrayList(const AllocatorType& allocator, SizeType capacityHint) :
	_container(allocator) {
	_container.reserve(capacityHint);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
template <typename InputIterator>
ETInlineHint ETForceInlineHint ArrayList<Value, Allocator>::ArrayList(const AllocatorType& allocator, InputIterator begin, InputIterator end) :
	_container(begin, end, allocator) {}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint ArrayList<Value, Allocator>::ArrayList(const AllocatorType& allocator, InitializerList<ValueType> values) :
	_container(values, allocator) {}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint ArrayList<Value, Allocator>::ArrayList(const AllocatorType& allocator, const ArrayList<Value, Allocator>& list) :
	_container(list.ConstBegin(), list.ConstEnd(), allocator) {}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint typename ArrayList<Value, Allocator>::ConstIterator ArrayList<Value, Allocator>::Find(ConstReference value) const ETNoexceptHint {
	return Eldritch2::Find(_container.cbegin(), _container.cend(), value);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint typename ArrayList<Value, Allocator>::Iterator ArrayList<Value, Allocator>::Find(ConstReference value) ETNoexceptHint {
	return Eldritch2::Find(_container.begin(), _container.end(), value);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint typename ArrayList<Value, Allocator>::ConstIterator ArrayList<Value, Allocator>::Find(ConstReference value, ConstIterator where) const ETNoexceptHint {
	return Eldritch2::Find(where, _container.cend(), value);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint typename ArrayList<Value, Allocator>::Iterator ArrayList<Value, Allocator>::Find(ConstReference value, Iterator where) ETNoexceptHint {
	return Eldritch2::Find(where, _container.end(), value);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint bool ArrayList<Value, Allocator>::Contains(ConstReference value) const ETNoexceptHint {
	return Eldritch2::Find(_container.cbegin(), _container.cend(), value) != _container.cend();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
template <typename UnaryPredicate>
ETInlineHint ETForceInlineHint void ArrayList<Value, Allocator>::EraseIf(UnaryPredicate condition) {
	const auto lastValid(Eldritch2::RemoveIf(eastl::make_move_iterator(_container.begin()), eastl::make_move_iterator(_container.end()), Move(condition)));
	_container.erase(lastValid.base(), _container.end());
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint typename ArrayList<Value, Allocator>::ConstIterator ArrayList<Value, Allocator>::ConstBegin() const ETNoexceptHint {
	return _container.cbegin();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint typename ArrayList<Value, Allocator>::ConstIterator ArrayList<Value, Allocator>::ConstEnd() const ETNoexceptHint {
	return _container.cend();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint typename ArrayList<Value, Allocator>::ConstReverseIterator ArrayList<Value, Allocator>::ConstReverseBegin() const ETNoexceptHint {
	return _container.crbegin();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint typename ArrayList<Value, Allocator>::ConstReverseIterator ArrayList<Value, Allocator>::ConstReverseEnd() const ETNoexceptHint {
	return _container.crend();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint typename ArrayList<Value, Allocator>::ConstReverseIterator ArrayList<Value, Allocator>::ReverseBegin() const ETNoexceptHint {
	return _container.rbegin();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint typename ArrayList<Value, Allocator>::ReverseIterator ArrayList<Value, Allocator>::ReverseBegin() ETNoexceptHint {
	return _container.rbegin();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint typename ArrayList<Value, Allocator>::ConstReverseIterator ArrayList<Value, Allocator>::ReverseEnd() const ETNoexceptHint {
	return _container.rend();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint typename ArrayList<Value, Allocator>::ReverseIterator ArrayList<Value, Allocator>::ReverseEnd() ETNoexceptHint {
	return _container.rend();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint typename ArrayList<Value, Allocator>::ConstIterator ArrayList<Value, Allocator>::Begin() const ETNoexceptHint {
	return _container.begin();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint typename ArrayList<Value, Allocator>::Iterator ArrayList<Value, Allocator>::Begin() ETNoexceptHint {
	return _container.begin();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint typename ArrayList<Value, Allocator>::ConstIterator ArrayList<Value, Allocator>::End() const ETNoexceptHint {
	return _container.end();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint typename ArrayList<Value, Allocator>::Iterator ArrayList<Value, Allocator>::End() ETNoexceptHint {
	return _container.end();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint typename ArrayList<Value, Allocator>::ConstReference ArrayList<Value, Allocator>::Front() const ETNoexceptHint {
	return _container.front();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint typename ArrayList<Value, Allocator>::Reference ArrayList<Value, Allocator>::Front() ETNoexceptHint {
	return _container.front();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint typename ArrayList<Value, Allocator>::ConstReference ArrayList<Value, Allocator>::Back() const ETNoexceptHint {
	return _container.back();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint typename ArrayList<Value, Allocator>::Reference ArrayList<Value, Allocator>::Back() ETNoexceptHint {
	return _container.back();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint void ArrayList<Value, Allocator>::Append(ConstReference value) {
	_container.push_back(value);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint void ArrayList<Value, Allocator>::Append(ValueType&& value) {
	_container.push_back(Move(value));
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
template <typename... Arguments>
ETInlineHint ETForceInlineHint typename ArrayList<Value, Allocator>::Reference ArrayList<Value, Allocator>::EmplaceBack(Arguments&&... arguments) {
	return _container.emplace_back(Forward<Arguments>(arguments)...);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint void ArrayList<Value, Allocator>::Pop() {
	_container.pop_back();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint typename ArrayList<Value, Allocator>::ConstSliceType ArrayList<Value, Allocator>::Slice(SizeType offset, SizeType length) const ETNoexceptHint {
	return { _container.cbegin() + offset, _container.cbegin() + Eldritch2::Minimum(_container.size() - offset, length) };
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint typename ArrayList<Value, Allocator>::SliceType ArrayList<Value, Allocator>::Slice(SizeType offset, SizeType length) ETNoexceptHint {
	return { _container.begin() + offset, _container.begin() + Eldritch2::Minimum(_container.size() - offset, length) };
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint ArrayList<Value, Allocator>::operator ConstSliceType() const ETNoexceptHint {
	return { _container.cbegin(), _container.cend() };
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint ArrayList<Value, Allocator>::operator SliceType() ETNoexceptHint {
	return { _container.begin(), _container.end() };
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint typename ArrayList<Value, Allocator>::ConstReference ArrayList<Value, Allocator>::operator[](SizeType index) const ETNoexceptHint {
	return _container[index];
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint typename ArrayList<Value, Allocator>::Reference ArrayList<Value, Allocator>::operator[](SizeType index) ETNoexceptHint {
	return _container[index];
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint typename ArrayList<Value, Allocator>::ConstReference ArrayList<Value, Allocator>::At(SizeType index) const {
	return _container.at(index);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint typename ArrayList<Value, Allocator>::Reference ArrayList<Value, Allocator>::At(SizeType index) {
	return _container.at(index);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint typename ArrayList<Value, Allocator>::ConstPointer ArrayList<Value, Allocator>::GetData() const ETNoexceptHint {
	return _container.data();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint typename ArrayList<Value, Allocator>::Pointer ArrayList<Value, Allocator>::GetData() ETNoexceptHint {
	return _container.data();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
template <typename InputIterator>
ETInlineHint ETForceInlineHint void ArrayList<Value, Allocator>::Assign(InputIterator begin, InputIterator end) {
	_container.assign(begin, end);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint void ArrayList<Value, Allocator>::Assign(InitializerList<ValueType> values) {
	_container.assign(values);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint void ArrayList<Value, Allocator>::Insert(Iterator where, SizeType count, ConstReference value) {
	_container.insert(where, count, value);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
template <typename InputIterator>
ETInlineHint ETForceInlineHint void ArrayList<Value, Allocator>::Insert(Iterator where, InputIterator first, InputIterator last) {
	_container.insert(where, first, last);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint typename ArrayList<Value, Allocator>::Iterator ArrayList<Value, Allocator>::Insert(Iterator where, ConstReference value) {
	return _container.insert(where, value);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint typename ArrayList<Value, Allocator>::Iterator ArrayList<Value, Allocator>::Insert(Iterator where, ValueType&& value) {
	return _container.insert(where, Move(value));
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
template <typename... Arguments>
ETInlineHint ETForceInlineHint typename ArrayList<Value, Allocator>::Iterator ArrayList<Value, Allocator>::Emplace(Iterator where, Arguments&&... arguments) {
	return _container.emplace(where, Forward<Arguments>(arguments)...);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint typename ArrayList<Value, Allocator>::ReverseIterator ArrayList<Value, Allocator>::Erase(ReverseIterator where) {
	return _container.erase(where);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint typename ArrayList<Value, Allocator>::Iterator ArrayList<Value, Allocator>::Erase(Iterator where) {
	return _container.erase(where);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint void ArrayList<Value, Allocator>::EraseUnordered(ReverseIterator where) {
	_container.erase_unsorted(where);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint void ArrayList<Value, Allocator>::EraseUnordered(Iterator where) {
	_container.erase_unsorted(where);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint typename ArrayList<Value, Allocator>::ReverseIterator ArrayList<Value, Allocator>::Erase(ReverseIterator begin, ReverseIterator end) {
	return _container.erase(begin, end);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint typename ArrayList<Value, Allocator>::Iterator ArrayList<Value, Allocator>::Erase(Iterator begin, Iterator end) {
	return _container.erase(begin, end);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint typename ArrayList<Value, Allocator>::Iterator ArrayList<Value, Allocator>::EraseFirst(ConstReference value) {
	return _container.erase_first(value);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint typename ArrayList<Value, Allocator>::Iterator ArrayList<Value, Allocator>::EraseLast(ConstReference value) {
	return _container.erase_last(value);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint typename ArrayList<Value, Allocator>::Iterator ArrayList<Value, Allocator>::EraseFirstUnordered(ConstReference value) {
	return _container.erase_first_unsorted(value);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint typename ArrayList<Value, Allocator>::Iterator ArrayList<Value, Allocator>::EraseLastUnordered(ConstReference value) {
	return _container.erase_last_unsorted(value);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
template <typename UnaryPredicate>
ETInlineHint ETForceInlineHint void ArrayList<Value, Allocator>::ClearAndDispose(const ReleaseMemorySemantics, UnaryPredicate disposer) ETNoexceptHintIf(IsNoThrowCallable<UnaryPredicate, Reference>()) {
	while (!_container.empty()) {
		disposer(_container.back());
		_container.pop_back();
	}

	_container.reset_lose_memory();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
template <typename UnaryPredicate>
ETInlineHint ETForceInlineHint void ArrayList<Value, Allocator>::ClearAndDispose(UnaryPredicate disposer) ETNoexceptHintIf(IsNoThrowCallable<UnaryPredicate, Reference>()) {
	while (!_container.empty()) {
		disposer(_container.back());
		_container.pop_back();
	}
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint void ArrayList<Value, Allocator>::Clear(const ReleaseMemorySemantics) ETNoexceptHint {
	_container.reset_lose_memory();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint void ArrayList<Value, Allocator>::Clear() ETNoexceptHint {
	_container.clear();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint void ArrayList<Value, Allocator>::Resize(SizeType sizeInElements, ConstReference filler) {
	_container.resize(sizeInElements, filler);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint void ArrayList<Value, Allocator>::Resize(SizeType sizeInElements) {
	_container.resize(sizeInElements);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint typename ArrayList<Value, Allocator>::SizeType ArrayList<Value, Allocator>::GetSize() const ETNoexceptHint {
	return _container.size();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint bool ArrayList<Value, Allocator>::IsEmpty() const ETNoexceptHint {
	return _container.empty();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint ArrayList<Value, Allocator>::operator bool() const ETNoexceptHint {
	return !_container.empty();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint typename ArrayList<Value, Allocator>::SizeType ArrayList<Value, Allocator>::GetCapacity() const ETNoexceptHint {
	return _container.capacity();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint void ArrayList<Value, Allocator>::SetCapacity(SizeType capacity) {
	_container.set_capacity(capacity);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint void ArrayList<Value, Allocator>::Reserve(SizeType minimumCapacity) {
	_container.reserve(minimumCapacity);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint ArrayList<Value, Allocator>& ArrayList<Value, Allocator>::ShrinkToFit() {
	_container.shrink_to_fit();
	return *this;
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint typename const ArrayList<Value, Allocator>::AllocatorType& ArrayList<Value, Allocator>::GetAllocator() const ETNoexceptHint {
	return _container.get_allocator();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint bool ArrayList<Value, Allocator>::Validate(ConstIterator iterator) const ETNoexceptHint {
	return _container.validate_iterator(iterator);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint void Swap(ArrayList<Value, Allocator>& lhs, ArrayList<Value, Allocator>& rhs) ETNoexceptHint {
	lhs._container.swap(rhs._container);
}

} // namespace Eldritch2
