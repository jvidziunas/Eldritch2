/*==================================================================*\
  SoaList.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename T, typename... U>
	ETCpp14Constexpr ETPureFunctionHint ETNeverThrowsHint T Minimum(T, U...) ETNoexceptHint;

} // namespace Eldritch2

namespace Eldritch2 {

template <class Allocator, typename... Types>
ETInlineHint ETForceInlineHint SoaListAlloc<Allocator, Types...>::SoaListAlloc(const AllocatorType& allocator, SizeType capacityHint) :
	_container(allocator) {
	_container.reserve(capacityHint);
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
ETInlineHint ETForceInlineHint SoaListAlloc<Allocator, Types...>::SoaListAlloc(const AllocatorType& allocator, const SoaListAlloc<Allocator, Types...>& list) :
	_container(list._container, allocator) {
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
template <size_t Index>
ETInlineHint ETForceInlineHint typename SoaListAlloc<Allocator, Types...>::LocalConstReverseIterator<typename SoaListAlloc<Allocator, Types...>::ValueType<Index>> SoaListAlloc<Allocator, Types...>::ConstReverseBegin() const ETNoexceptHint {
	return eastl::make_reverse_iterator(this->ConstEnd<Index>());
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
template <typename Type>
ETInlineHint ETForceInlineHint typename SoaListAlloc<Allocator, Types...>::LocalConstReverseIterator<Type> SoaListAlloc<Allocator, Types...>::ConstReverseBegin() const ETNoexceptHint {
	return eastl::make_reverse_iterator(this->ConstEnd<Type>());
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
ETInlineHint ETForceInlineHint typename SoaListAlloc<Allocator, Types...>::ConstReverseIterator SoaListAlloc<Allocator, Types...>::ConstReverseBegin() const ETNoexceptHint {
	return _container.crend();
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
template <size_t Index>
ETInlineHint ETForceInlineHint typename SoaListAlloc<Allocator, Types...>::LocalConstReverseIterator<typename SoaListAlloc<Allocator, Types...>::ValueType<Index>> SoaListAlloc<Allocator, Types...>::ConstReverseEnd() const ETNoexceptHint {
	return eastl::make_reverse_iterator(this->ConstBegin<Index>());
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
template <typename Type>
ETInlineHint ETForceInlineHint typename SoaListAlloc<Allocator, Types...>::LocalConstReverseIterator<Type> SoaListAlloc<Allocator, Types...>::ConstReverseEnd() const ETNoexceptHint {
	return eastl::make_reverse_iterator(this->ConstBegin<Type>());
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
ETInlineHint ETForceInlineHint typename SoaListAlloc<Allocator, Types...>::ConstReverseIterator SoaListAlloc<Allocator, Types...>::ConstReverseEnd() const ETNoexceptHint {
	return _container.crend();
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
template <size_t Index>
ETInlineHint ETForceInlineHint typename SoaListAlloc<Allocator, Types...>::LocalConstReverseIterator<typename SoaListAlloc<Allocator, Types...>::ValueType<Index>> SoaListAlloc<Allocator, Types...>::ReverseBegin() const ETNoexceptHint {
	return eastl::make_reverse_iterator(this->End<Index>());
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
template <typename Type>
ETInlineHint ETForceInlineHint typename SoaListAlloc<Allocator, Types...>::LocalConstReverseIterator<Type> SoaListAlloc<Allocator, Types...>::ReverseBegin() const ETNoexceptHint {
	return eastl::make_reverse_iterator(this->End<Type>());
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
ETInlineHint ETForceInlineHint typename SoaListAlloc<Allocator, Types...>::ConstReverseIterator SoaListAlloc<Allocator, Types...>::ReverseBegin() const ETNoexceptHint {
	return _container.rbegin();
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
template <size_t Index>
ETInlineHint ETForceInlineHint typename SoaListAlloc<Allocator, Types...>::LocalReverseIterator<typename SoaListAlloc<Allocator, Types...>::ValueType<Index>> SoaListAlloc<Allocator, Types...>::ReverseBegin() ETNoexceptHint {
	return eastl::make_reverse_iterator(this->End<Index>());
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
template <typename Type>
ETInlineHint ETForceInlineHint typename SoaListAlloc<Allocator, Types...>::LocalReverseIterator<Type> SoaListAlloc<Allocator, Types...>::ReverseBegin() ETNoexceptHint {
	return eastl::make_reverse_iterator(this->End<Type>());
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
ETInlineHint ETForceInlineHint typename SoaListAlloc<Allocator, Types...>::ReverseIterator SoaListAlloc<Allocator, Types...>::ReverseBegin() ETNoexceptHint {
	return _container.rbegin();
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
template <size_t Index>
ETInlineHint ETForceInlineHint typename SoaListAlloc<Allocator, Types...>::LocalConstReverseIterator<typename SoaListAlloc<Allocator, Types...>::ValueType<Index>> SoaListAlloc<Allocator, Types...>::ReverseEnd() const ETNoexceptHint {
	return eastl::make_reverse_iterator(this->Begin<Index>());
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
template <typename Type>
ETInlineHint ETForceInlineHint typename SoaListAlloc<Allocator, Types...>::LocalConstReverseIterator<Type> SoaListAlloc<Allocator, Types...>::ReverseEnd() const ETNoexceptHint {
	return eastl::make_reverse_iterator(this->Begin<Type>());
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
ETInlineHint ETForceInlineHint typename SoaListAlloc<Allocator, Types...>::ConstReverseIterator SoaListAlloc<Allocator, Types...>::ReverseEnd() const ETNoexceptHint {
	return _container.rend();
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
template <size_t Index>
ETInlineHint ETForceInlineHint typename SoaListAlloc<Allocator, Types...>::LocalReverseIterator<typename SoaListAlloc<Allocator, Types...>::ValueType<Index>> SoaListAlloc<Allocator, Types...>::ReverseEnd() ETNoexceptHint {
	return eastl::make_reverse_iterator(this->Begin<Index>());
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
template <typename Type>
ETInlineHint ETForceInlineHint typename SoaListAlloc<Allocator, Types...>::LocalReverseIterator<Type> SoaListAlloc<Allocator, Types...>::ReverseEnd() ETNoexceptHint {
	return eastl::make_reverse_iterator(this->Begin<Type>());
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
ETInlineHint ETForceInlineHint typename SoaListAlloc<Allocator, Types...>::ReverseIterator SoaListAlloc<Allocator, Types...>::ReverseEnd() ETNoexceptHint {
	return _container.rend();
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
template <size_t Index>
ETInlineHint ETForceInlineHint typename SoaListAlloc<Allocator, Types...>::LocalConstIterator<typename SoaListAlloc<Allocator, Types...>::ValueType<Index>> SoaListAlloc<Allocator, Types...>::ConstBegin() const ETNoexceptHint {
	return _container.get<Index>();
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
template <typename Type>
ETInlineHint ETForceInlineHint typename SoaListAlloc<Allocator, Types...>::LocalConstIterator<Type> SoaListAlloc<Allocator, Types...>::ConstBegin() const ETNoexceptHint {
	return _container.get<Type>();
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
ETInlineHint ETForceInlineHint typename SoaListAlloc<Allocator, Types...>::ConstIterator SoaListAlloc<Allocator, Types...>::ConstBegin() const ETNoexceptHint {
	return _container.cbegin();
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
template <size_t Index>
ETInlineHint ETForceInlineHint typename SoaListAlloc<Allocator, Types...>::LocalConstIterator<typename SoaListAlloc<Allocator, Types...>::ValueType<Index>> SoaListAlloc<Allocator, Types...>::ConstEnd() const ETNoexceptHint {
	return _container.get<Index>() + _container.size();
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
template <typename Type>
ETInlineHint ETForceInlineHint typename SoaListAlloc<Allocator, Types...>::LocalConstIterator<Type> SoaListAlloc<Allocator, Types...>::ConstEnd() const ETNoexceptHint {
	return _container.get<Type>() + _container.size();
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
ETInlineHint ETForceInlineHint typename SoaListAlloc<Allocator, Types...>::ConstIterator SoaListAlloc<Allocator, Types...>::ConstEnd() const ETNoexceptHint {
	return _container.cend();
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
template <size_t Index>
ETInlineHint ETForceInlineHint typename SoaListAlloc<Allocator, Types...>::LocalConstIterator<typename SoaListAlloc<Allocator, Types...>::ValueType<Index>> SoaListAlloc<Allocator, Types...>::Begin() const ETNoexceptHint {
	return _container.get<Index>();
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
template <typename Type>
ETInlineHint ETForceInlineHint typename SoaListAlloc<Allocator, Types...>::LocalConstIterator<Type> SoaListAlloc<Allocator, Types...>::Begin() const ETNoexceptHint {
	return _container.get<Type>();
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
ETInlineHint ETForceInlineHint typename SoaListAlloc<Allocator, Types...>::ConstIterator SoaListAlloc<Allocator, Types...>::Begin() const ETNoexceptHint {
	return _container.begin();
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
template <size_t Index>
ETInlineHint ETForceInlineHint typename SoaListAlloc<Allocator, Types...>::LocalIterator<typename SoaListAlloc<Allocator, Types...>::ValueType<Index>> SoaListAlloc<Allocator, Types...>::Begin() ETNoexceptHint {
	return _container.get<Index>();
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
template <typename Type>
ETInlineHint ETForceInlineHint typename SoaListAlloc<Allocator, Types...>::LocalIterator<Type> SoaListAlloc<Allocator, Types...>::Begin() ETNoexceptHint {
	return _container.get<Type>();
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
ETInlineHint ETForceInlineHint typename SoaListAlloc<Allocator, Types...>::Iterator SoaListAlloc<Allocator, Types...>::Begin() ETNoexceptHint {
	return _container.begin();
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
template <size_t Index>
ETInlineHint ETForceInlineHint typename SoaListAlloc<Allocator, Types...>::LocalConstIterator<typename SoaListAlloc<Allocator, Types...>::ValueType<Index>> SoaListAlloc<Allocator, Types...>::End() const ETNoexceptHint {
	return _container.get<Index>() + _container.size();
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
template <typename Type>
ETInlineHint ETForceInlineHint typename SoaListAlloc<Allocator, Types...>::LocalConstIterator<Type> SoaListAlloc<Allocator, Types...>::End() const ETNoexceptHint {
	return _container.get<Type>() + _container.size();
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
ETInlineHint ETForceInlineHint typename SoaListAlloc<Allocator, Types...>::ConstIterator SoaListAlloc<Allocator, Types...>::End() const ETNoexceptHint {
	return _container.end();
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
template <size_t Index>
ETInlineHint ETForceInlineHint typename SoaListAlloc<Allocator, Types...>::LocalIterator<typename SoaListAlloc<Allocator, Types...>::ValueType<Index>> SoaListAlloc<Allocator, Types...>::End() ETNoexceptHint {
	return _container.get<Index>() + _container.size();
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
template <typename Type>
ETInlineHint ETForceInlineHint typename SoaListAlloc<Allocator, Types...>::LocalIterator<Type> SoaListAlloc<Allocator, Types...>::End() ETNoexceptHint {
	return _container.get<Type>() + _container.size();
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
ETInlineHint ETForceInlineHint typename SoaListAlloc<Allocator, Types...>::Iterator SoaListAlloc<Allocator, Types...>::End() ETNoexceptHint {
	return _container.end();
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
ETInlineHint ETForceInlineHint typename SoaListAlloc<Allocator, Types...>::ConstSliceType SoaListAlloc<Allocator, Types...>::Slice(SizeType offset, SizeType length) const ETNoexceptHint {
	return { _container.cbegin() + offset, _container.cbegin() + Eldritch2::Minimum(_container.size() - offset, length) };
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
ETInlineHint ETForceInlineHint typename SoaListAlloc<Allocator, Types...>::SliceType SoaListAlloc<Allocator, Types...>::Slice(SizeType offset, SizeType length) ETNoexceptHint {
	return { _container.begin() + offset, _container.begin() + Eldritch2::Minimum(_container.size() - offset, length) };
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
ETInlineHint ETForceInlineHint SoaListAlloc<Allocator, Types...>::operator ConstSliceType() const ETNoexceptHint {
	return { ConstBegin(), ConstEnd() };
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
ETInlineHint ETForceInlineHint SoaListAlloc<Allocator, Types...>::operator SliceType() ETNoexceptHint {
	return { Begin(), ConstEnd() };
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
ETInlineHint ETForceInlineHint typename SoaListAlloc<Allocator, Types...>::ConstReference SoaListAlloc<Allocator, Types...>::operator[](SizeType offset) const ETNoexceptHint {
	return _container[offset];
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
ETInlineHint ETForceInlineHint typename SoaListAlloc<Allocator, Types...>::Reference SoaListAlloc<Allocator, Types...>::operator[](SizeType offset) ETNoexceptHint {
	return _container[offset];
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
ETInlineHint ETForceInlineHint typename SoaListAlloc<Allocator, Types...>::ConstReference SoaListAlloc<Allocator, Types...>::Front() const ETNoexceptHint {
	return _container.front();
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
ETInlineHint ETForceInlineHint typename SoaListAlloc<Allocator, Types...>::Reference SoaListAlloc<Allocator, Types...>::Front() ETNoexceptHint {
	return _container.front();
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
ETInlineHint ETForceInlineHint typename SoaListAlloc<Allocator, Types...>::ConstReference SoaListAlloc<Allocator, Types...>::Back() const ETNoexceptHint {
	return _container.back();
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
ETInlineHint ETForceInlineHint typename SoaListAlloc<Allocator, Types...>::Reference SoaListAlloc<Allocator, Types...>::Back() ETNoexceptHint {
	return _container.back();
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
template <size_t Index>
ETInlineHint ETForceInlineHint typename const SoaListAlloc<Allocator, Types...>::ValueType<Index>* SoaListAlloc<Allocator, Types...>::GetData() const ETNoexceptHint {
	return _container.get<Index>();
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
template <typename Type>
ETInlineHint ETForceInlineHint const Type* SoaListAlloc<Allocator, Types...>::GetData() const ETNoexceptHint {
	return _container.get<Type>();
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
ETInlineHint ETForceInlineHint typename SoaListAlloc<Allocator, Types...>::ConstPointer SoaListAlloc<Allocator, Types...>::GetData() const ETNoexceptHint {
	return _container.data();
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
template <size_t Index>
ETInlineHint ETForceInlineHint typename SoaListAlloc<Allocator, Types...>::ValueType<Index>* SoaListAlloc<Allocator, Types...>::GetData() ETNoexceptHint {
	return _container.get<Index>();
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
template <typename Type>
ETInlineHint ETForceInlineHint Type* SoaListAlloc<Allocator, Types...>::GetData() ETNoexceptHint {
	return _container.get<Type>();
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
ETInlineHint ETForceInlineHint typename SoaListAlloc<Allocator, Types...>::Pointer SoaListAlloc<Allocator, Types...>::GetData() ETNoexceptHint {
	return _container.data();
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
ETInlineHint ETForceInlineHint void SoaListAlloc<Allocator, Types...>::Append(const Types&... values) {
	_container.push_back(values...);
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
ETInlineHint ETForceInlineHint typename SoaListAlloc<Allocator, Types...>::Reference SoaListAlloc<Allocator, Types...>::EmplaceBack(Types&&... values) {
	return _container.emplace_back(Forward<Types>(values)...);
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
ETInlineHint ETForceInlineHint typename SoaListAlloc<Allocator, Types...>::Iterator SoaListAlloc<Allocator, Types...>::Emplace(ConstIterator where, Types&&... arguments) {
	return _container.emplace(where, Move(arguments)...);
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
ETInlineHint ETForceInlineHint typename SoaListAlloc<Allocator, Types...>::Iterator SoaListAlloc<Allocator, Types...>::Insert(ConstIterator where, const Types&... arguments) {
	return _container.insert(where, 1u, arguments...);
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
ETInlineHint ETForceInlineHint typename SoaListAlloc<Allocator, Types...>::Iterator SoaListAlloc<Allocator, Types...>::Erase(ConstIterator begin, ConstIterator end) {
	return _container.erase(begin, end);
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
ETInlineHint ETForceInlineHint typename SoaListAlloc<Allocator, Types...>::Iterator SoaListAlloc<Allocator, Types...>::Erase(ConstIterator where) {
	return _container.erase(where);
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
ETInlineHint ETForceInlineHint typename SoaListAlloc<Allocator, Types...>::Iterator SoaListAlloc<Allocator, Types...>::EraseUnordered(ConstIterator where) {
	return _container.erase_unsorted(where);
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
ETInlineHint ETForceInlineHint typename SoaListAlloc<Allocator, Types...>::SizeType SoaListAlloc<Allocator, Types...>::GetCapacity() const ETNoexceptHint {
	return _container.capacity();
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
ETInlineHint ETForceInlineHint typename SoaListAlloc<Allocator, Types...>::SizeType SoaListAlloc<Allocator, Types...>::GetSize() const ETNoexceptHint {
	return _container.size();
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
ETInlineHint ETForceInlineHint void SoaListAlloc<Allocator, Types...>::Reserve(SizeType capacityHint) {
	_container.reserve(capacityHint);
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
template <typename NaryPredicate>
ETInlineHint ETForceInlineHint void SoaListAlloc<Allocator, Types...>::ClearAndDispose(NaryPredicate disposer) ETNoexceptHintIf(noexcept(disposer(Declval<Types&>()...))) {
	while (!_container.empty()) {
		eastl::apply(disposer, _container.back());
		_container.pop_back();
	}
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
ETInlineHint ETForceInlineHint void SoaListAlloc<Allocator, Types...>::Clear() ETNoexceptHint {
	_container.clear();
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
ETInlineHint ETForceInlineHint SoaListAlloc<Allocator, Types...>::operator bool() const ETNoexceptHint {
	return !_container.empty();
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
ETInlineHint ETForceInlineHint bool SoaListAlloc<Allocator, Types...>::IsEmpty() const ETNoexceptHint {
	return _container.empty();
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
ETInlineHint ETForceInlineHint typename const SoaListAlloc<Allocator, Types...>::AllocatorType& SoaListAlloc<Allocator, Types...>::GetAllocator() const ETNoexceptHint {
	return _container.get_allocator();
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
ETInlineHint ETForceInlineHint void Swap(SoaListAlloc<Allocator, Types...>& lhs, SoaListAlloc<Allocator, Types...>& rhs) ETNoexceptHint {
	lhs._container.swap(rhs._container);
}

} // namespace Eldritch2
