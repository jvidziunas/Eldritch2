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
ETInlineHint ETForceInlineHint ArrayList<Value, Allocator>::ArrayList(const AllocatorType& allocator, std::initializer_list<ValueType> list) :
	_container(list, allocator) {
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint ArrayList<Value, Allocator>::ArrayList(const AllocatorType& allocator, const ArrayList<Value, Allocator>& list) :
	_container(list.ConstBegin(), list.ConstEnd(), allocator) {}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint typename ArrayList<Value, Allocator>::ConstIterator ArrayList<Value, Allocator>::Find(ConstReference value) const {
	return eastl::find(_container.begin(), _container.end(), value);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint typename ArrayList<Value, Allocator>::Iterator ArrayList<Value, Allocator>::Find(ConstReference item) {
	return eastl::find(_container.begin(), _container.end(), item);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint typename ArrayList<Value, Allocator>::ConstIterator ArrayList<Value, Allocator>::Find(ConstReference value, ConstIterator where) const {
	return eastl::find(where, _container.end(), value);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint typename ArrayList<Value, Allocator>::Iterator ArrayList<Value, Allocator>::Find(ConstReference value, Iterator where) {
	return eastl::find(where, _container.end(), value);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint bool ArrayList<Value, Allocator>::Contains(ConstReference value) const {
	return eastl::find(_container.cbegin(), _container.cend(), value) != _container.cend();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
template <typename UnaryPredicate>
ETInlineHint void ArrayList<Value, Allocator>::EraseIf(UnaryPredicate condition) {
	const auto lastValid(eastl::remove_if(eastl::make_move_iterator(_container.begin()), eastl::make_move_iterator(_container.end()), eastl::move(condition)));
	_container.erase(lastValid.base(), _container.end());
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
template <typename UnaryPredicate>
ETInlineHint void ArrayList<Value, Allocator>::Sort(UnaryPredicate sort) {
	eastl::sort(_container.begin(), _container.end(), eastl::move(sort));
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
ETInlineHint ETForceInlineHint void ArrayList<Value, Allocator>::Append(const ValueType& value) {
	_container.push_back(value);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint void ArrayList<Value, Allocator>::Append(ValueType&& value) {
	_container.push_back(eastl::move(value));
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
template <typename... Arguments>
ETInlineHint ETForceInlineHint void ArrayList<Value, Allocator>::EmplaceBack(Arguments&&... arguments) {
	_container.emplace_back(eastl::forward<Arguments>(arguments)...);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint void ArrayList<Value, Allocator>::Pop() {
	_container.pop_back();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint typename ArrayList<Value, Allocator>::ConstReference ArrayList<Value, Allocator>::operator[](SizeType offset) const ETNoexceptHint {
	return _container[offset];
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint typename ArrayList<Value, Allocator>::Reference ArrayList<Value, Allocator>::operator[](SizeType offset) ETNoexceptHint {
	return _container[offset];
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
ETInlineHint ETForceInlineHint void ArrayList<Value, Allocator>::Assign(std::initializer_list<ValueType> list) {
	_container.assign(list);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint void ArrayList<Value, Allocator>::Insert(Iterator where, SizeType count, const ValueType& value) {
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
ETInlineHint ETForceInlineHint typename ArrayList<Value, Allocator>::Iterator ArrayList<Value, Allocator>::Insert(Iterator where, const ValueType& value) {
	return _container.insert(where, value);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint typename ArrayList<Value, Allocator>::Iterator ArrayList<Value, Allocator>::Insert(Iterator where, ValueType&& value) {
	return _container.insert(where, eastl::move(value));
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
template <typename... Arguments>
ETInlineHint ETForceInlineHint typename ArrayList<Value, Allocator>::Iterator ArrayList<Value, Allocator>::Emplace(Iterator where, Arguments&&... arguments) {
	return _container.emplace(where, eastl::forward<Arguments>(arguments)...);
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
ETInlineHint ETForceInlineHint typename ArrayList<Value, Allocator>::Iterator ArrayList<Value, Allocator>::EraseFirst(const ValueType& value) {
	return _container.erase_first(value);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint typename ArrayList<Value, Allocator>::Iterator ArrayList<Value, Allocator>::EraseLast(const ValueType& value) {
	return _container.erase_last(value);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint typename ArrayList<Value, Allocator>::Iterator ArrayList<Value, Allocator>::EraseFirstUnordered(const ValueType& value) {
	return _container.erase_first_unsorted(value);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint typename ArrayList<Value, Allocator>::Iterator ArrayList<Value, Allocator>::EraseLastUnordered(const ValueType& value) {
	return _container.erase_last_unsorted(value);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint void ArrayList<Value, Allocator>::Clear() {
	_container.clear();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint void ArrayList<Value, Allocator>::Clear(const ReleaseMemorySemantics) {
	_container.reset_lose_memory();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint void ArrayList<Value, Allocator>::Resize(SizeType sizeInElements, const ValueType& filler) {
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
ETInlineHint ETForceInlineHint void ArrayList<Value, Allocator>::SetCapacity(SizeType capacityInElements) {
	_container.set_capacity(capacityInElements);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint void ArrayList<Value, Allocator>::Reserve(SizeType capacityHintInElements) {
	_container.reserve(capacityHintInElements);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint void ArrayList<Value, Allocator>::ShrinkToFit() {
	_container.shrink_to_fit();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint typename const ArrayList<Value, Allocator>::AllocatorType& ArrayList<Value, Allocator>::GetAllocator() const ETNoexceptHint {
	return _container.get_allocator();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint bool ArrayList<Value, Allocator>::ValidateIterator(ConstIterator iterator) const {
	return _container.validate_iterator(iterator);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ETForceInlineHint void Swap(ArrayList<Value, Allocator>& lhs, ArrayList<Value, Allocator>& rhs) {
	lhs._container.swap(rhs._container);
}

} // namespace Eldritch2
