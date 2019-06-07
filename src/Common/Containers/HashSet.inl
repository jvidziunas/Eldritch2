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
#include <Common/Containers/Span.hpp>
#include <Common/Algorithms.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint HashSet<Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::HashSet(
	const AllocatorType&     allocator,
	SizeType                 bucketCount,
	const HashPredicate&     hash,
	const EqualityPredicate& equal) :
	_container(bucketCount, hash, equal, allocator) {}

// ---------------------------------------------------

template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
template <typename InputIterator>
ETInlineHint ETForceInlineHint HashSet<Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::HashSet(
	const AllocatorType&     allocator,
	SizeType                 bucketCount,
	const HashPredicate&     hash,
	const EqualityPredicate& equal,
	InputIterator            begin,
	InputIterator            end) :
	_container(begin, end, bucketCount, hash, equal, allocator) {}

// ---------------------------------------------------

template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint HashSet<Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::HashSet(
	const AllocatorType&       allocator,
	SizeType                   bucketCount,
	const HashPredicate&       hash,
	const EqualityPredicate&   equal,
	InitializerList<ValueType> values) :
	_container(values, bucketCount, hash, equal, allocator) {}

// ---------------------------------------------------

template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint HashSet<Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::HashSet(const AllocatorType& allocator, const HashSet<Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>& set) :
	_container(set.Begin(), set.End(), 0u, set.GetHash(), set.GetEqualityPredicate(), allocator) {
}

// ---------------------------------------------------

template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
template <typename Value2, typename HashPredicate2, typename EqualityPredicate2>
ETInlineHint ETForceInlineHint typename HashSet<Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::ConstIterator HashSet<Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::Find(
	const Value2&      value,
	HashPredicate2     hash,
	EqualityPredicate2 equal) const ETNoexceptHint {
	return _container.find_as<Value2, HashPredicate2, EqualityPredicate2>(value, Move(hash), Move(equal));
}

// ---------------------------------------------------

template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
template <typename Value2, typename HashPredicate2, typename EqualityPredicate2>
ETInlineHint ETForceInlineHint typename HashSet<Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::Iterator HashSet<Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::Find(
	const Value2&      value,
	HashPredicate2     hash,
	EqualityPredicate2 equal) ETNoexceptHint {
	return _container.find_as<Value2, HashPredicate2, EqualityPredicate2>(value, Move(hash), Move(equal));
}

// ---------------------------------------------------

template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint typename HashSet<Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::Iterator HashSet<Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::Find(ConstReference value) ETNoexceptHint {
	return _container.find(value);
}

// ---------------------------------------------------

template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint typename HashSet<Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::ConstIterator HashSet<Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::Find(ConstReference value) const ETNoexceptHint {
	return _container.find(value);
}

// ---------------------------------------------------

template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
template <typename Value2, typename HashPredicate2, typename EqualityPredicate2>
ETInlineHint ETForceInlineHint bool HashSet<Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::Contains(const Value2& value, HashPredicate2 hash, EqualityPredicate2 equal) const ETNoexceptHint {
	return _container.find_as<Value2, HashPredicate2, EqualityPredicate2>(value, Move(hash), Move(equal)) != _container.end();
}

// ---------------------------------------------------

template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint bool HashSet<Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::Contains(ConstReference value) const ETNoexceptHint {
	return _container.find(value) != _container.end();
}

// ---------------------------------------------------

template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
template <typename UnaryPredicate>
ETInlineHint ETForceInlineHint void HashSet<Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::EraseIf(UnaryPredicate predicate) {
	for (auto element(_container.begin()), end(_container.end()); element != end;) {
		if (predicate(*element)) {
			element = _container.erase(element);
			continue;
		}
		++element;
	}
}

// ---------------------------------------------------

template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint typename HashSet<Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::Iterator HashSet<Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::Begin() ETNoexceptHint {
	return _container.begin();
}

// ---------------------------------------------------

template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint typename HashSet<Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::ConstIterator HashSet<Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::Begin() const ETNoexceptHint {
	return _container.begin();
}

// ---------------------------------------------------

template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint typename HashSet<Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::ConstIterator HashSet<Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::ConstBegin() const ETNoexceptHint {
	return _container.begin();
}

// ---------------------------------------------------

template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint typename HashSet<Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::Iterator HashSet<Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::End() ETNoexceptHint {
	return _container.end();
}

// ---------------------------------------------------

template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint typename HashSet<Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::ConstIterator HashSet<Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::End() const ETNoexceptHint {
	return _container.end();
}

// ---------------------------------------------------

template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint typename HashSet<Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::ConstIterator HashSet<Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::ConstEnd() const ETNoexceptHint {
	return _container.end();
}

// ---------------------------------------------------

template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint Pair<typename HashSet<Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::Iterator, bool> HashSet<Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::Insert(ConstReference value) {
	return _container.insert(value);
}

// ---------------------------------------------------

template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint Pair<typename HashSet<Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::Iterator, bool> HashSet<Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::Insert(ValueType&& value) {
	return _container.insert(Move(value));
}

// ---------------------------------------------------

template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
template <typename... Args>
ETInlineHint ETForceInlineHint Pair<typename HashSet<Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::Iterator, bool> HashSet<Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::Emplace(Args&&... args) {
	return _container.emplace(Forward<Args>(args)...);
}

// ---------------------------------------------------

template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint typename HashSet<Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::Iterator HashSet<Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::Erase(Iterator begin, Iterator end) {
	return _container.erase(begin, end);
}

// ---------------------------------------------------

template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint typename HashSet<Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::Iterator HashSet<Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::Erase(Iterator where) {
	return _container.erase(where);
}

// ---------------------------------------------------

template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
template <typename Value2, typename HashPredicate2, typename EqualityPredicate2>
ETInlineHint ETForceInlineHint typename HashSet<Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::SizeType HashSet<Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::Erase(const Value2& value, HashPredicate2 hash, EqualityPredicate2 equal) {
	const auto where(_container.find_as<Value2, HashPredicate2, EqualityPredicate2>(value, Move(hash), Move(equal)));
	if (where == _container.end()) {
		return 0;
	}

	_container.erase(where);
	return 1;
}

// ---------------------------------------------------

template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint typename HashSet<Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::SizeType HashSet<Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::Erase(ConstReference value) {
	return _container.erase(value);
}

// ---------------------------------------------------

template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
template <typename UnaryPredicate>
ETInlineHint ETForceInlineHint void HashSet<Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::ClearAndDispose(UnaryPredicate disposer) ETNoexceptHint(noexcept(disposer(Declval<Reference>()))) {
	for (ConstReference value : _container) {
		// Use of const_cast is gross, but this is one of the suggested solutions in http://www.open-std.org/jtc1/sc22/wg21/docs/lwg-defects.html#103
		disposer(const_cast<Reference>(value));
	}

	_container.clear();
}

// ---------------------------------------------------

template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint void HashSet<Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::Clear() ETNoexceptHint {
	_container.clear();
}

// ---------------------------------------------------

template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint typename HashSet<Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::SizeType HashSet<Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::GetSize() const ETNoexceptHint {
	return _container.size();
}

// ---------------------------------------------------

template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint bool HashSet<Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::IsEmpty() const ETNoexceptHint {
	return _container.empty();
}

// ---------------------------------------------------

template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint HashSet<Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::operator bool() const ETNoexceptHint {
	return !_container.empty();
}

// ---------------------------------------------------

template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint void HashSet<Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::Reserve(SizeType minimumSizeHint) {
	_container.reserve(minimumSizeHint);
}

// ---------------------------------------------------

template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint typename HashSet<Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::EqualityPredicateType HashSet<Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::GetEqualityPredicate() const ETNoexceptHint {
	return _container.key_eq();
}

// ---------------------------------------------------

template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint typename HashSet<Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::HashPredicateType HashSet<Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::GetHash() const ETNoexceptHint {
	return _container.hash_function();
}

// ---------------------------------------------------

template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint typename const HashSet<Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::AllocatorType& HashSet<Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>::GetAllocator() const ETNoexceptHint {
	return _container.get_allocator();
}

// ---------------------------------------------------

template <typename Value, class HashPredicate, class EqualityPredicate, class Allocator, bool CacheHashCode>
ETInlineHint ETForceInlineHint void Swap(HashSet<Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>& lhs, HashSet<Value, HashPredicate, EqualityPredicate, Allocator, CacheHashCode>& rhs) ETNoexceptHint {
	lhs._container.swap(rhs._container);
}

} // namespace Eldritch2
