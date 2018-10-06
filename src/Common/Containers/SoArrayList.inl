/*==================================================================*\
  SoArrayList.inl
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

template <class Allocator, typename... Types>
ETInlineHint ETForceInlineHint SoArrayListAlloc<Allocator, Types...>::SoArrayListAlloc(const AllocatorType& allocator, SizeType capacityHint) :
	_container(allocator) {
	_container.reserve(capacityHint);
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
ETInlineHint ETForceInlineHint SoArrayListAlloc<Allocator, Types...>::SoArrayListAlloc(const AllocatorType& allocator, const SoArrayListAlloc<Allocator, Types...>& list) :
	_container(list._container, allocator) {
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
template <typename Type>
ETInlineHint ETForceInlineHint typename SoArrayListAlloc<Allocator, Types...>::LocalConstReverseIterator<Type> SoArrayListAlloc<Allocator, Types...>::ConstReverseBegin() const ETNoexceptHint {
	return LocalConstReverseIterator<Type>(this->ConstEnd<Type>());
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
ETInlineHint ETForceInlineHint typename SoArrayListAlloc<Allocator, Types...>::ConstReverseIterator SoArrayListAlloc<Allocator, Types...>::ConstReverseBegin() const ETNoexceptHint {
	return _container.crend();
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
template <typename Type>
ETInlineHint ETForceInlineHint typename SoArrayListAlloc<Allocator, Types...>::LocalConstReverseIterator<Type> SoArrayListAlloc<Allocator, Types...>::ConstReverseEnd() const ETNoexceptHint {
	return LocalConstReverseIterator<Type>(this->ConstBegin<Type>());
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
ETInlineHint ETForceInlineHint typename SoArrayListAlloc<Allocator, Types...>::ConstReverseIterator SoArrayListAlloc<Allocator, Types...>::ConstReverseEnd() const ETNoexceptHint {
	return _container.crend();
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
template <typename Type>
ETInlineHint ETForceInlineHint typename SoArrayListAlloc<Allocator, Types...>::LocalConstReverseIterator<Type> SoArrayListAlloc<Allocator, Types...>::ReverseBegin() const ETNoexceptHint {
	return LocalConstReverseIterator<Type>(this->End<Type>());
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
ETInlineHint ETForceInlineHint typename SoArrayListAlloc<Allocator, Types...>::ConstReverseIterator SoArrayListAlloc<Allocator, Types...>::ReverseBegin() const ETNoexceptHint {
	return _container.rbegin();
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
template <typename Type>
ETInlineHint ETForceInlineHint typename SoArrayListAlloc<Allocator, Types...>::LocalReverseIterator<Type> SoArrayListAlloc<Allocator, Types...>::ReverseBegin() ETNoexceptHint {
	return LocalReverseIterator<Type>(this->End<Type>());
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
ETInlineHint ETForceInlineHint typename SoArrayListAlloc<Allocator, Types...>::ReverseIterator SoArrayListAlloc<Allocator, Types...>::ReverseBegin() ETNoexceptHint {
	return _container.rbegin();
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
template <typename Type>
ETInlineHint ETForceInlineHint typename SoArrayListAlloc<Allocator, Types...>::LocalConstReverseIterator<Type> SoArrayListAlloc<Allocator, Types...>::ReverseEnd() const ETNoexceptHint {
	return LocalConstReverseIterator<Type>(this->Begin<Type>());
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
ETInlineHint ETForceInlineHint typename SoArrayListAlloc<Allocator, Types...>::ConstReverseIterator SoArrayListAlloc<Allocator, Types...>::ReverseEnd() const ETNoexceptHint {
	return _container.rend();
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
template <typename Type>
ETInlineHint ETForceInlineHint typename SoArrayListAlloc<Allocator, Types...>::LocalReverseIterator<Type> SoArrayListAlloc<Allocator, Types...>::ReverseEnd() ETNoexceptHint {
	return LocalReverseIterator<Type>(this->Begin<Type>());
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
ETInlineHint ETForceInlineHint typename SoArrayListAlloc<Allocator, Types...>::ReverseIterator SoArrayListAlloc<Allocator, Types...>::ReverseEnd() ETNoexceptHint {
	return _container.rend();
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
template <typename Type>
ETInlineHint ETForceInlineHint typename SoArrayListAlloc<Allocator, Types...>::LocalConstIterator<Type> SoArrayListAlloc<Allocator, Types...>::ConstBegin() const ETNoexceptHint {
	return _container.get<Type>();
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
ETInlineHint ETForceInlineHint typename SoArrayListAlloc<Allocator, Types...>::ConstIterator SoArrayListAlloc<Allocator, Types...>::ConstBegin() const ETNoexceptHint {
	return _container.cbegin();
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
template <typename Type>
ETInlineHint ETForceInlineHint typename SoArrayListAlloc<Allocator, Types...>::LocalConstIterator<Type> SoArrayListAlloc<Allocator, Types...>::ConstEnd() const ETNoexceptHint {
	return _container.get<Type>() + _container.size();
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
ETInlineHint ETForceInlineHint typename SoArrayListAlloc<Allocator, Types...>::ConstIterator SoArrayListAlloc<Allocator, Types...>::ConstEnd() const ETNoexceptHint {
	return _container.cend();
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
template <typename Type>
ETInlineHint ETForceInlineHint typename SoArrayListAlloc<Allocator, Types...>::LocalConstIterator<Type> SoArrayListAlloc<Allocator, Types...>::Begin() const ETNoexceptHint {
	return _container.get<Type>();
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
ETInlineHint ETForceInlineHint typename SoArrayListAlloc<Allocator, Types...>::ConstIterator SoArrayListAlloc<Allocator, Types...>::Begin() const ETNoexceptHint {
	return _container.begin();
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
template <typename Type>
ETInlineHint ETForceInlineHint typename SoArrayListAlloc<Allocator, Types...>::LocalIterator<Type> SoArrayListAlloc<Allocator, Types...>::Begin() ETNoexceptHint {
	return _container.get<Type>();
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
ETInlineHint ETForceInlineHint typename SoArrayListAlloc<Allocator, Types...>::Iterator SoArrayListAlloc<Allocator, Types...>::Begin() ETNoexceptHint {
	return _container.begin();
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
template <typename Type>
ETInlineHint ETForceInlineHint typename SoArrayListAlloc<Allocator, Types...>::LocalConstIterator<Type> SoArrayListAlloc<Allocator, Types...>::End() const ETNoexceptHint {
	return _container.get<Type>() + _container.size();
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
ETInlineHint ETForceInlineHint typename SoArrayListAlloc<Allocator, Types...>::ConstIterator SoArrayListAlloc<Allocator, Types...>::End() const ETNoexceptHint {
	return _container.end();
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
template <typename Type>
ETInlineHint ETForceInlineHint typename SoArrayListAlloc<Allocator, Types...>::LocalIterator<Type> SoArrayListAlloc<Allocator, Types...>::End() ETNoexceptHint {
	return _container.get<Type>() + _container.size();
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
ETInlineHint ETForceInlineHint typename SoArrayListAlloc<Allocator, Types...>::Iterator SoArrayListAlloc<Allocator, Types...>::End() ETNoexceptHint {
	return _container.end();
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
ETInlineHint ETForceInlineHint typename SoArrayListAlloc<Allocator, Types...>::ConstReference SoArrayListAlloc<Allocator, Types...>::operator[](SizeType offset) const ETNoexceptHint {
	return _container[offset];
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
ETInlineHint ETForceInlineHint typename SoArrayListAlloc<Allocator, Types...>::Reference SoArrayListAlloc<Allocator, Types...>::operator[](SizeType offset) ETNoexceptHint {
	return _container[offset];
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
ETInlineHint ETForceInlineHint typename SoArrayListAlloc<Allocator, Types...>::ConstReference SoArrayListAlloc<Allocator, Types...>::Front() const ETNoexceptHint {
	return _container.front();
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
ETInlineHint ETForceInlineHint typename SoArrayListAlloc<Allocator, Types...>::Reference SoArrayListAlloc<Allocator, Types...>::Front() ETNoexceptHint {
	return _container.front();
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
ETInlineHint ETForceInlineHint typename SoArrayListAlloc<Allocator, Types...>::ConstReference SoArrayListAlloc<Allocator, Types...>::Back() const ETNoexceptHint {
	return _container.back();
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
ETInlineHint ETForceInlineHint typename SoArrayListAlloc<Allocator, Types...>::Reference SoArrayListAlloc<Allocator, Types...>::Back() ETNoexceptHint {
	return _container.back();
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
template <typename Type>
ETInlineHint ETForceInlineHint const Type* SoArrayListAlloc<Allocator, Types...>::GetData() const ETNoexceptHint {
	return _container.get<Type>();
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
ETInlineHint ETForceInlineHint typename SoArrayListAlloc<Allocator, Types...>::ConstPointer SoArrayListAlloc<Allocator, Types...>::GetData() const ETNoexceptHint {
	return _container.data();
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
template <typename Type>
ETInlineHint ETForceInlineHint Type* SoArrayListAlloc<Allocator, Types...>::GetData() ETNoexceptHint {
	return _container.get<Type>();
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
ETInlineHint ETForceInlineHint typename SoArrayListAlloc<Allocator, Types...>::Pointer SoArrayListAlloc<Allocator, Types...>::GetData() ETNoexceptHint {
	return _container.data();
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
ETInlineHint ETForceInlineHint void SoArrayListAlloc<Allocator, Types...>::Append(const Types&... values) {
	_container.push_back(values...);
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
ETInlineHint ETForceInlineHint typename SoArrayListAlloc<Allocator, Types...>::Reference SoArrayListAlloc<Allocator, Types...>::EmplaceBack(Types&&... values) {
	return _container.emplace_back(eastl::forward<Types>(values)...);
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
ETInlineHint ETForceInlineHint typename SoArrayListAlloc<Allocator, Types...>::Iterator SoArrayListAlloc<Allocator, Types...>::Emplace(ConstIterator where, Types&&... arguments) {
	return _container.emplace(where, eastl::move(arguments)...);
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
ETInlineHint ETForceInlineHint typename SoArrayListAlloc<Allocator, Types...>::Iterator SoArrayListAlloc<Allocator, Types...>::Insert(ConstIterator where, const Types&... arguments) {
	return _container.insert(where, 1u, arguments...);
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
ETInlineHint ETForceInlineHint typename SoArrayListAlloc<Allocator, Types...>::Iterator SoArrayListAlloc<Allocator, Types...>::Erase(ConstIterator begin, ConstIterator end) {
	return _container.erase(begin, end);
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
ETInlineHint ETForceInlineHint typename SoArrayListAlloc<Allocator, Types...>::Iterator SoArrayListAlloc<Allocator, Types...>::Erase(ConstIterator where) {
	return _container.erase(where);
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
ETInlineHint ETForceInlineHint typename SoArrayListAlloc<Allocator, Types...>::Iterator SoArrayListAlloc<Allocator, Types...>::EraseUnordered(ConstIterator where) {
	return _container.erase_unsorted(where);
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
ETInlineHint ETForceInlineHint typename SoArrayListAlloc<Allocator, Types...>::SizeType SoArrayListAlloc<Allocator, Types...>::GetCapacity() const ETNoexceptHint {
	return _container.capacity();
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
ETInlineHint ETForceInlineHint typename SoArrayListAlloc<Allocator, Types...>::SizeType SoArrayListAlloc<Allocator, Types...>::GetSize() const ETNoexceptHint {
	return _container.size();
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
ETInlineHint ETForceInlineHint void SoArrayListAlloc<Allocator, Types...>::Reserve(SizeType capacityHint) {
	_container.reserve(capacityHint);
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
ETInlineHint ETForceInlineHint void SoArrayListAlloc<Allocator, Types...>::Clear() ETNoexceptHint {
	_container.clear();
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
ETInlineHint ETForceInlineHint SoArrayListAlloc<Allocator, Types...>::operator bool() const ETNoexceptHint {
	return !_container.empty();
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
ETInlineHint ETForceInlineHint bool SoArrayListAlloc<Allocator, Types...>::IsEmpty() const ETNoexceptHint {
	return _container.empty();
}

// ---------------------------------------------------

template <class Allocator, typename... Types>
ETInlineHint ETForceInlineHint typename const SoArrayListAlloc<Allocator, Types...>::AllocatorType& SoArrayListAlloc<Allocator, Types...>::GetAllocator() const ETNoexceptHint {
	return _container.get_allocator()
}

} // namespace Eldritch2
