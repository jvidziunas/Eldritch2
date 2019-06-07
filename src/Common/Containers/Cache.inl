/*==================================================================*\
  Cache.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2019 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator>
	ETInlineHint ETForceInlineHint Pair<typename Cache<Key, Value, HashPredicate, EqualityPredicate, Allocator>::Iterator, bool> Cache<Key, Value, HashPredicate, EqualityPredicate, Allocator>::Insert(const ValueType& value) {
		Pair<Iterator, bool> where(_container.Insert(value));
		if (where.second) {
			_lru.Append(where.first->second);
		}

		return where;
	}

	// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator>
	ETInlineHint ETForceInlineHint Pair<typename Cache<Key, Value, HashPredicate, EqualityPredicate, Allocator>::Iterator, bool> Cache<Key, Value, HashPredicate, EqualityPredicate, Allocator>::Insert(ValueType&& value) {
		Pair<Iterator, bool> where(_container.Insert(Move(value)));
		if (where.second) {
			_lru.Append(where.first->second);
		}

		return where;
	}

	// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator>
	template <typename... Arguments>
	ETInlineHint ETForceInlineHint Pair<typename Cache<Key, Value, HashPredicate, EqualityPredicate, Allocator>::Iterator, bool> Cache<Key, Value, HashPredicate, EqualityPredicate, Allocator>::TryEmplace(const KeyType& key, Arguments&&... arguments) {
		Pair<Iterator, bool> where(_container.TryEmplace(key, Forward<Arguments>(arguments)...));
		if (where.second) {
			_lru.Append(where.first->second);
		}

		return where;
	}

	// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator>
	template <typename... Arguments>
	ETInlineHint ETForceInlineHint Pair<typename Cache<Key, Value, HashPredicate, EqualityPredicate, Allocator>::Iterator, bool> Cache<Key, Value, HashPredicate, EqualityPredicate, Allocator>::TryEmplace(KeyType&& key, Arguments&&... arguments) {
		Pair<Iterator, bool> where(_container.TryEmplace(key, Forward<Arguments>(arguments)...));
		if (where.second) {
			_lru.Append(where.first->second);
		}

		return where;
	}

	// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator>
	template <typename... Arguments>
	ETInlineHint ETForceInlineHint Pair<typename Cache<Key, Value, HashPredicate, EqualityPredicate, Allocator>::Iterator, bool> Cache<Key, Value, HashPredicate, EqualityPredicate, Allocator>::Emplace(Arguments&&... arguments) {
		Pair<Iterator, bool> where(_container.Emplace(Forward<Arguments>(arguments)...));
		if (where.second) {
			_lru.Append(where.first->second);
		}

		return where;
	}

	// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator>
	template <typename... Arguments>
	ETInlineHint ETForceInlineHint typename Cache<Key, Value, HashPredicate, EqualityPredicate, Allocator>::Iterator Cache<Key, Value, HashPredicate, EqualityPredicate, Allocator>::ReplaceOldest(const KeyType& key, Arguments&&... arguments) {
		_lru.PopFrontAndDispose([](typename LeastRecentlyUsedList::Reference oldest) {
		});
	}

	// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator>
	template <typename... Arguments>
	ETInlineHint ETForceInlineHint typename Cache<Key, Value, HashPredicate, EqualityPredicate, Allocator>::Iterator Cache<Key, Value, HashPredicate, EqualityPredicate, Allocator>::ReplaceOldest(KeyType&& key, Arguments&&... arguments) {
		_lru.PopFrontAndDispose([](typename LeastRecentlyUsedList::Reference oldest) {
		});
	}

	// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator>
	ETInlineHint ETForceInlineHint typename Cache<Key, Value, HashPredicate, EqualityPredicate, Allocator>::Iterator Cache<Key, Value, HashPredicate, EqualityPredicate, Allocator>::Erase(Iterator where) {
		_lru.Erase(where->second);
		return _container.Erase(where);
	}

	// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator>
	ETInlineHint ETForceInlineHint typename Cache<Key, Value, HashPredicate, EqualityPredicate, Allocator>::SizeType Cache<Key, Value, HashPredicate, EqualityPredicate, Allocator>::Erase(const KeyType& key) {
		const Iterator where(_container.Find(key));
		if (where == _container.End()) {
			return SizeType(0u);
		}

		_lru.Erase(where->second);
		_container.Erase(where);
		return SizeType(1u);
	}

	// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator>
	template <typename UnaryPredicate>
	ETInlineHint ETForceInlineHint void Cache<Key, Value, HashPredicate, EqualityPredicate, Allocator>::ClearAndDispose(UnaryPredicate disposer) {
		_container.ClearAndDispose<UnaryPredicate>(Move(disposer))();
		_lru.Clear();
	}

	// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator>
	ETInlineHint ETForceInlineHint void Cache<Key, Value, HashPredicate, EqualityPredicate, Allocator>::Clear() {
		_container.Clear();
		_lru.Clear();
	}

	// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator>
	ETInlineHint ETForceInlineHint typename Cache<Key, Value, HashPredicate, EqualityPredicate, Allocator>::SizeType Cache<Key, Value, HashPredicate, EqualityPredicate, Allocator>::GetSize() const ETNoexceptHint {
		return _container.Size();
	}

	// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator>
	ETInlineHint ETForceInlineHint bool Cache<Key, Value, HashPredicate, EqualityPredicate, Allocator>::IsEmpty() const ETNoexceptHint {
		return _container.IsEmpty();
	}

	// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator>
	ETInlineHint ETForceInlineHint Cache<Key, Value, HashPredicate, EqualityPredicate, Allocator>::operator bool() const ETNoexceptHint {
		return !_container.IsEmpty();
	}

	// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator>
	ETInlineHint ETForceInlineHint typename Cache<Key, Value, HashPredicate, EqualityPredicate, Allocator>::EqualityPredicateType Cache<Key, Value, HashPredicate, EqualityPredicate, Allocator>::GetEqualityPredicate() const ETNoexceptHint {
		return _container.GetEqualityPredicate();
	}

	// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator>
	ETInlineHint ETForceInlineHint typename Cache<Key, Value, HashPredicate, EqualityPredicate, Allocator>::HashPredicateType Cache<Key, Value, HashPredicate, EqualityPredicate, Allocator>::GetHash() const ETNoexceptHint {
		return _container.GetHash();
	}

	// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator>
	ETInlineHint ETForceInlineHint typename const Cache<Key, Value, HashPredicate, EqualityPredicate, Allocator>::AllocatorType& Cache<Key, Value, HashPredicate, EqualityPredicate, Allocator>::GetAllocator() const ETNoexceptHint {
		return _container.GetAllocator();
	}

	// ---------------------------------------------------

	template <typename Key, typename Value, class HashPredicate, class EqualityPredicate, class Allocator>
	ETInlineHint ETForceInlineHint void Swap(Cache<Key, Value, HashPredicate, EqualityPredicate, Allocator>& lhs, Cache<Key, Value, HashPredicate, EqualityPredicate, Allocator>& rhs) ETNoexceptHint {
		Swap(lhs._container, rhs._container);
		Swap(lhs._lru, rhs._lru);
	}

}	// namespace Eldritch2
