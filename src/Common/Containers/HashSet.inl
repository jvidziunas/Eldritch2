/*==================================================================*\
  HashSet.inl
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Containers/Range.hpp>
#include <Common/Algorithms.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::HashSet(
	const AllocatorType&     allocator,
	SizeType                 bucketCount,
	const HashPredicate&     hash,
	const EqualityPredicate& equal) :
	_container(bucketCount, hash, equal, allocator) {
}

// ---------------------------------------------------

template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
template <typename InputIterator>
ETInlineHint HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::HashSet(
	const AllocatorType&     allocator,
	SizeType                 bucketCount,
	const HashPredicate&     hash,
	const EqualityPredicate& equal,
	InputIterator            begin,
	InputIterator            end) :
	_container(begin, end, bucketCount, hash, equal, allocator) {
}

// ---------------------------------------------------

template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::HashSet(
	const AllocatorType&             allocator,
	SizeType                         bucketCount,
	const HashPredicate&             hash,
	const EqualityPredicate&         equal,
	std::initializer_list<ValueType> set) :
	_container(set, bucketCount, hash, equal, allocator) {
}

// ---------------------------------------------------

template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::HashSet(const AllocatorType& allocator, const HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>& set) :
	_container(set.Begin(), set.End(), 0u, set.GetHash(), set.GetEqualityPredicate(), allocator) {
}

// ---------------------------------------------------

template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
template <typename Value2, typename HashPredicate2, typename EqualityPredicate2>
ETInlineHint typename HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::ConstIterator HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Find(
	const Value2&      value,
	HashPredicate2     hash,
	EqualityPredicate2 equal) const {
	return _container.find_as<Value2, HashPredicate2, EqualityPredicate2>(value, eastl::move(hash), eastl::move(equal));
}

// ---------------------------------------------------

template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
template <typename Value2, typename HashPredicate2, typename EqualityPredicate2>
ETInlineHint typename HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Iterator HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Find(
	const Value2&      value,
	HashPredicate2     hash,
	EqualityPredicate2 equal) {
	return _container.find_as<Value2, HashPredicate2, EqualityPredicate2>(value, eastl::move(hash), eastl::move(equal));
}

// ---------------------------------------------------

template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint typename HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Iterator HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Find(const ValueType& value) {
	return _container.find(value);
}

// ---------------------------------------------------

template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint typename HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::ConstIterator HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Find(const ValueType& value) const {
	return _container.find(value);
}

// ---------------------------------------------------

template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
template <typename Value2, typename HashPredicate2, typename EqualityPredicate2>
ETInlineHint bool HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Contains(const Value2& value, HashPredicate2 hash, EqualityPredicate2 equal) const {
	return _container.find_as<Value2, HashPredicate2, EqualityPredicate2>(value, eastl::move(hash), eastl::move(equal)) != _container.end();
}

// ---------------------------------------------------

template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint bool HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Contains(const ValueType& value) const {
	return _container.find(value) != _container.end();
}

// ---------------------------------------------------

template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
template <typename UnaryPredicate>
ETInlineHint void HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::EraseIf(UnaryPredicate predicate) {
	for (auto element(_container.begin()), end(_container.end()); element != end;) {
		if (predicate(*element)) {
			element = _container.erase(element);
			continue;
		}
		++element;
	}
}

// ---------------------------------------------------

template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint typename HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Iterator HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Begin() {
	return _container.begin();
}

// ---------------------------------------------------

template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint typename HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::ConstIterator HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Begin() const {
	return _container.begin();
}

// ---------------------------------------------------

template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint typename HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::ConstIterator HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::ConstBegin() const {
	return _container.begin();
}

// ---------------------------------------------------

template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint typename HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Iterator HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::End() {
	return _container.end();
}

// ---------------------------------------------------

template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint typename HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::ConstIterator HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::End() const {
	return _container.end();
}

// ---------------------------------------------------

template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint typename HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::ConstIterator HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::ConstEnd() const {
	return _container.end();
}

// ---------------------------------------------------

template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint Pair<typename HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Iterator, bool> HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Insert(const ValueType& value) {
	return _container.insert(value);
}

// ---------------------------------------------------

template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint Pair<typename HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Iterator, bool> HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Insert(ValueType&& value) {
	return _container.insert(eastl::move(value));
}

// ---------------------------------------------------

template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
template <typename... Args>
ETInlineHint Pair<typename HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Iterator, bool> HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Emplace(Args&&... args) {
	return _container.emplace(eastl::forward<Args>(args)...);
}

// ---------------------------------------------------

template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint typename HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Iterator HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Erase(Iterator begin, Iterator end) {
	return _container.erase(begin, end);
}

// ---------------------------------------------------

template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint typename HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Iterator HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Erase(Iterator where) {
	return _container.erase(where);
}

// ---------------------------------------------------

template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint typename HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::SizeType HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Erase(const ValueType& value) {
	return _container.erase(value);
}

// ---------------------------------------------------

template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint void HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Clear() {
	_container.clear();
}

// ---------------------------------------------------

template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint typename HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::SizeType HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::GetSize() const {
	return _container.size();
}

// ---------------------------------------------------

template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint bool HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::IsEmpty() const {
	return _container.empty();
}

// ---------------------------------------------------

template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::operator bool() const {
	return !_container.empty();
}

// ---------------------------------------------------

template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint void HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::Reserve(SizeType minimumSizeHint) {
	_container.reserve(minimumSizeHint);
}

// ---------------------------------------------------

template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint typename const HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::EqualityPredicateType& HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::GetEqualityPredicate() const {
	return _container.key_eq();
}

// ---------------------------------------------------

template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint typename const HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::HashPredicateType& HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::GetHash() const {
	return _container.hash_function();
}

// ---------------------------------------------------

template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint typename const HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::AllocatorType& HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>::GetAllocator() const {
	return _container.get_allocator();
}

// ---------------------------------------------------

template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool cacheHashCode>
ETInlineHint void Swap(HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>& lhs, HashSet<Value, HashPredicate, EqualityPredicate, Allocator, cacheHashCode>& rhs) {
	lhs._container.swap(rhs._container);
}

} // namespace Eldritch2
