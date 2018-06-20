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
ETInlineHint ArrayList<Value, Allocator>::ArrayList(const ArrayList<Value, Allocator>& list) :
	_container(list._container) {}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ArrayList<Value, Allocator>::ArrayList(SizeType size, const AllocatorType& allocator) :
	_container(size, allocator) {}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ArrayList<Value, Allocator>::ArrayList(const AllocatorType& allocator) :
	_container(allocator) {}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ArrayList<Value, Allocator>::ArrayList(ArrayList<Value, Allocator>&& list) :
	_container(eastl::move(list._container)) {}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint typename ArrayList<Value, Allocator>::ConstIterator ArrayList<Value, Allocator>::Find(ConstReference item) const {
	return eastl::find(_container.begin(), _container.end(), item);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint typename ArrayList<Value, Allocator>::Iterator ArrayList<Value, Allocator>::Find(ConstReference item) {
	return eastl::find(_container.begin(), _container.end(), item);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint typename ArrayList<Value, Allocator>::ConstIterator ArrayList<Value, Allocator>::Find(ConstReference item, ConstIterator start) const {
	return eastl::find(start, _container.end(), item);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint typename ArrayList<Value, Allocator>::Iterator ArrayList<Value, Allocator>::Find(ConstReference item, Iterator start) {
	return eastl::find(start, _container.end(), item);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint bool ArrayList<Value, Allocator>::Contains(ConstReference item) const {
	return eastl::find(_container.begin(), _container.end(), item) != _container.end();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
template <typename Predicate>
ETInlineHint void ArrayList<Value, Allocator>::EraseIf(Predicate filter) {
	//	Reimplementing for now, as the eastl remove_if implementation does not use move semantics.
	_container.erase(std::remove_if(_container.begin(), _container.end(), eastl::forward<Predicate>(filter)), _container.end());
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
template <typename Predicate>
ETInlineHint void ArrayList<Value, Allocator>::Sort(Predicate sorter) {
	eastl::sort(_container.begin(), _container.end(), sorter);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint typename ArrayList<Value, Allocator>& ArrayList<Value, Allocator>::operator=(const ArrayList<Value, Allocator>& list) {
	_container = list._container;

	return *this;
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint typename ArrayList<Value, Allocator>& ArrayList<Value, Allocator>::operator=(ArrayList<Value, Allocator>&& list) {
	_container = eastl::move(list._container);

	return *this;
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint typename ArrayList<Value, Allocator>::ConstIterator ArrayList<Value, Allocator>::ConstBegin() const {
	return _container.cbegin();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint typename ArrayList<Value, Allocator>::ConstIterator ArrayList<Value, Allocator>::ConstEnd() const {
	return _container.cend();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint typename ArrayList<Value, Allocator>::ConstReverseIterator ArrayList<Value, Allocator>::ConstReverseBegin() const {
	return _container.crbegin();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint typename ArrayList<Value, Allocator>::ConstReverseIterator ArrayList<Value, Allocator>::ConstReverseEnd() const {
	return _container.crend();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint typename ArrayList<Value, Allocator>::ConstReverseIterator ArrayList<Value, Allocator>::ReverseBegin() const {
	return _container.rbegin();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint typename ArrayList<Value, Allocator>::ReverseIterator ArrayList<Value, Allocator>::ReverseBegin() {
	return _container.rbegin();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint typename ArrayList<Value, Allocator>::ConstReverseIterator ArrayList<Value, Allocator>::ReverseEnd() const {
	return _container.rend();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint typename ArrayList<Value, Allocator>::ReverseIterator ArrayList<Value, Allocator>::ReverseEnd() {
	return _container.rend();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint typename ArrayList<Value, Allocator>::ConstIterator ArrayList<Value, Allocator>::Begin() const {
	return _container.begin();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint typename ArrayList<Value, Allocator>::Iterator ArrayList<Value, Allocator>::Begin() {
	return _container.begin();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint typename ArrayList<Value, Allocator>::ConstIterator ArrayList<Value, Allocator>::End() const {
	return _container.end();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint typename ArrayList<Value, Allocator>::Iterator ArrayList<Value, Allocator>::End() {
	return _container.end();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint typename ArrayList<Value, Allocator>::ConstReference ArrayList<Value, Allocator>::Front() const {
	return _container.front();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint typename ArrayList<Value, Allocator>::Reference ArrayList<Value, Allocator>::Front() {
	return _container.front();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint typename ArrayList<Value, Allocator>::ConstReference ArrayList<Value, Allocator>::Back() const {
	return _container.back();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint typename ArrayList<Value, Allocator>::Reference ArrayList<Value, Allocator>::Back() {
	return _container.back();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint void ArrayList<Value, Allocator>::Append(const ValueType& item) {
	_container.push_back(item);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint void ArrayList<Value, Allocator>::Append(ValueType&& item) {
	_container.push_back(eastl::forward<ValueType>(item));
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
template <typename... ElementConstructorArguments>
ETInlineHint void ArrayList<Value, Allocator>::EmplaceBack(ElementConstructorArguments&&... arguments) {
	_container.emplace_back(eastl::forward<ElementConstructorArguments>(arguments)...);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint void ArrayList<Value, Allocator>::Pop() {
	_container.pop_back();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint typename ArrayList<Value, Allocator>::ConstReference ArrayList<Value, Allocator>::operator[](SizeType index) const {
	return _container[index];
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint typename ArrayList<Value, Allocator>::Reference ArrayList<Value, Allocator>::operator[](SizeType index) {
	return _container[index];
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint typename ArrayList<Value, Allocator>::ConstReference ArrayList<Value, Allocator>::At(SizeType index) const {
	return _container.at(index);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint typename ArrayList<Value, Allocator>::Reference ArrayList<Value, Allocator>::At(SizeType index) {
	return _container.at(index);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint typename ArrayList<Value, Allocator>::ConstPointer ArrayList<Value, Allocator>::GetData() const {
	return _container.data();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint typename ArrayList<Value, Allocator>::Pointer ArrayList<Value, Allocator>::GetData() {
	return _container.data();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
template <typename InputIterator>
ETInlineHint void ArrayList<Value, Allocator>::Assign(InputIterator begin, InputIterator end) {
	_container.assign(begin, end);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint void ArrayList<Value, Allocator>::Insert(Iterator start, SizeType itemCount, const ValueType& item) {
	_container.insert(initialIndex, itemCount, item);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
template <typename InputIterator>
ETInlineHint void ArrayList<Value, Allocator>::Insert(Iterator position, InputIterator first, InputIterator last) {
	_container.insert(position, first, last);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint typename ArrayList<Value, Allocator>::Iterator ArrayList<Value, Allocator>::Insert(Iterator position, const ValueType& item) {
	return _container.insert(position, item);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint typename ArrayList<Value, Allocator>::Iterator ArrayList<Value, Allocator>::Insert(Iterator position, ValueType&& item) {
	return _container.insert(position, eastl::forward<ValueType>(item));
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
template <typename... ElementConstructorArguments>
ETInlineHint typename ArrayList<Value, Allocator>::Iterator ArrayList<Value, Allocator>::Emplace(Iterator position, ElementConstructorArguments&&... arguments) {
	return _container.emplace(position, eastl::forward<ElementConstructorArguments>(arguments)...);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint typename ArrayList<Value, Allocator>::ReverseIterator ArrayList<Value, Allocator>::Erase(ReverseIterator position) {
	return _container.erase(position);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint typename ArrayList<Value, Allocator>::Iterator ArrayList<Value, Allocator>::Erase(Iterator position) {
	return _container.erase(position);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint void ArrayList<Value, Allocator>::Erase(ReverseIterator position, const UnorderedSemantics) {
	_container.erase_unsorted(position);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint void ArrayList<Value, Allocator>::Erase(Iterator position, const UnorderedSemantics) {
	_container.erase_unsorted(position);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint typename ArrayList<Value, Allocator>::ReverseIterator ArrayList<Value, Allocator>::Erase(ReverseIterator begin, ReverseIterator end) {
	return _container.erase(begin, end);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint typename ArrayList<Value, Allocator>::Iterator ArrayList<Value, Allocator>::Erase(Iterator begin, Iterator end) {
	return _container.erase(begin, end);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint void ArrayList<Value, Allocator>::Clear() {
	_container.clear();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint void ArrayList<Value, Allocator>::Clear(const ReleaseMemorySemantics) {
	_container.reset_lose_memory();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint void ArrayList<Value, Allocator>::Resize(SizeType sizeInElements, const ValueType& item) {
	_container.resize(sizeInElements, item);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint void ArrayList<Value, Allocator>::Resize(SizeType sizeInElements) {
	_container.resize(sizeInElements);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint typename ArrayList<Value, Allocator>::SizeType ArrayList<Value, Allocator>::GetSize() const {
	return _container.size();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint bool ArrayList<Value, Allocator>::IsEmpty() const {
	return _container.empty();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint ArrayList<Value, Allocator>::operator bool() const {
	return !_container.empty();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint typename ArrayList<Value, Allocator>::SizeType ArrayList<Value, Allocator>::GetCapacity() const {
	return _container.capacity();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint void ArrayList<Value, Allocator>::SetCapacity(SizeType capacityInElements) {
	_container.set_capacity(capacityInElements);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint void ArrayList<Value, Allocator>::Reserve(SizeType capacityHintInElements) {
	_container.reserve(capacityHintInElements);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint void ArrayList<Value, Allocator>::ShrinkToFit() {
	_container.shrink_to_fit();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint typename const ArrayList<Value, Allocator>::AllocatorType& ArrayList<Value, Allocator>::GetAllocator() const {
	return _container.get_allocator();
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint bool ArrayList<Value, Allocator>::ValidateIterator(ConstIterator iterator) const {
	return _container.validate_iterator(iterator);
}

// ---------------------------------------------------

template <typename Value, typename Allocator>
ETInlineHint void Swap(ArrayList<Value, Allocator>& list0, ArrayList<Value, Allocator>& list1) {
	eastl::swap(list0._container, list1._container);
}

} // namespace Eldritch2
