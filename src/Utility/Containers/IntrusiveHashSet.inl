/*==================================================================*\
  IntrusiveHashSet.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  �2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename Value, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	template <typename... AllocatorConstructorArguments>
	ETInlineHint IntrusiveHashSet<Value, HashPredicate, EqualityPredicate, BucketAllocator>::IntrusiveHashSet( const SizeType initialBucketCount, AllocatorConstructorArguments&&... allocatorConstructorArguments ) : _buckets( initialBucketCount, std::forward<AllocatorConstructorArguments>(allocatorConstructorArguments)... ),
																																																														   _underlyingContainer( UnderlyingContainer::bucket_traits( _buckets.Data(), _buckets.GetSize() ) ) {}

// ---------------------------------------------------

	template <typename Value, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	template <typename... AllocatorConstructorArguments>
	ETInlineHint IntrusiveHashSet<Value, HashPredicate, EqualityPredicate, BucketAllocator>::IntrusiveHashSet( const SizeType initialBucketCount, const HashPredicate& hasher, const EqualityPredicate& equalityPredicate, AllocatorConstructorArguments&&... allocatorConstructorArguments ) : _buckets( initialBucketCount, std::forward<AllocatorContructorArguments>( allocatorConstructorArguments )... ),
																																																																																	_underlyingContainer( UnderlyingContainer::bucket_traits( _buckets.Data(), _buckets.GetSize() ), hasher, equalityPredicate ) {}
	
// ---------------------------------------------------

	template <typename Value, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	ETInlineHint IntrusiveHashSet<Value, HashPredicate, EqualityPredicate, BucketAllocator>::~IntrusiveHashSet() {}

// ---------------------------------------------------

	template <typename Value, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	template <typename Key, typename KeyHashPredicate, typename KeyEqualityPredicate>
	ETForceInlineHint typename IntrusiveHashSet<Value, HashPredicate, EqualityPredicate, BucketAllocator>::Iterator IntrusiveHashSet<Value, HashPredicate, EqualityPredicate, BucketAllocator>::Find( const Key& key, KeyHashPredicate keyHashPredicate, KeyEqualityPredicate keyEqualityPredicate ) {
		return _underlyingContainer.find( key, keyHashPredicate, keyEqualityPredicate );
	}

// ---------------------------------------------------

	template <typename Value, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	template <typename Key, typename KeyHashPredicate, typename KeyEqualityPredicate>
	ETForceInlineHint typename IntrusiveHashSet<Value, HashPredicate, EqualityPredicate, BucketAllocator>::ConstIterator IntrusiveHashSet<Value, HashPredicate, EqualityPredicate, BucketAllocator>::Find( const Key& key, KeyHashPredicate keyHashPredicate, KeyEqualityPredicate keyEqualityPredicate ) const {
		return _underlyingContainer.find( key, keyHashPredicate, keyEqualityPredicate );
	}

// ---------------------------------------------------

	template <typename Value, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	template <typename Key, typename KeyHashPredicate, typename KeyEqualityPredicate>
	ETForceInlineHint typename IntrusiveHashSet<Value, HashPredicate, EqualityPredicate, BucketAllocator>::SizeType IntrusiveHashSet<Value, HashPredicate, EqualityPredicate, BucketAllocator>::RemoveIf( const Key& key, KeyHashPredicate keyHashPredicate, KeyEqualityPredicate keyEqualityPredicate ) {
		return _underlyingContainer.erase( key, keyHashPredicate, keyEqualityPredicate );
	}

// ---------------------------------------------------

	template <typename Value, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	template<typename Key, typename KeyHashPredicate, typename KeyEqualityPredicate, typename Disposer>
	ETForceInlineHint typename IntrusiveHashSet<Value, HashPredicate, EqualityPredicate, BucketAllocator>::SizeType IntrusiveHashSet<Value, HashPredicate, EqualityPredicate, BucketAllocator>::RemoveIfAndDispose( const Key& key, KeyHashPredicate keyHashPredicate, KeyEqualityPredicate keyEqualityPredicate, Disposer disposer ) {
		return _underlyingContainer.erase_and_dispose( key, keyHashPredicate, keyEqualityPredicate, [disposer] ( ValueType* const value ) mutable {
			disposer( *value );
		} );
	}

// ---------------------------------------------------

	template <typename Value, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	ETForceInlineHint typename IntrusiveHashSet<Value, HashPredicate, EqualityPredicate, BucketAllocator>::Iterator IntrusiveHashSet<Value, HashPredicate, EqualityPredicate, BucketAllocator>::Begin() {
		return _underlyingContainer.begin();
	}

// ---------------------------------------------------

	template <typename Value, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	ETForceInlineHint typename IntrusiveHashSet<Value, HashPredicate, EqualityPredicate, BucketAllocator>::ConstIterator IntrusiveHashSet<Value, HashPredicate, EqualityPredicate, BucketAllocator>::Begin() const {
		return _underlyingContainer.begin();
	}

// ---------------------------------------------------

	template <typename Value, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	ETForceInlineHint typename IntrusiveHashSet<Value, HashPredicate, EqualityPredicate, BucketAllocator>::Iterator IntrusiveHashSet<Value, HashPredicate, EqualityPredicate, BucketAllocator>::End() {
		return _underlyingContainer.end();
	}

// ---------------------------------------------------

	template <typename Value, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	ETForceInlineHint typename IntrusiveHashSet<Value, HashPredicate, EqualityPredicate, BucketAllocator>::ConstIterator IntrusiveHashSet<Value, HashPredicate, EqualityPredicate, BucketAllocator>::End() const {
		return _underlyingContainer.end();
	}

// ---------------------------------------------------

	template <typename Value, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	ETForceInlineHint typename IntrusiveHashSet<Value, HashPredicate, EqualityPredicate, BucketAllocator>::ConstIterator IntrusiveHashSet<Value, HashPredicate, EqualityPredicate, BucketAllocator>::ConstBegin() const {
		return _underlyingContainer.cbegin();
	}

// ---------------------------------------------------

	template <typename Value, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	ETForceInlineHint typename IntrusiveHashSet<Value, HashPredicate, EqualityPredicate, BucketAllocator>::ConstIterator IntrusiveHashSet<Value, HashPredicate, EqualityPredicate, BucketAllocator>::ConstEnd() const {
		return _underlyingContainer.cend();
	}

// ---------------------------------------------------

	template <typename Value, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	ETForceInlineHint void IntrusiveHashSet<Value, HashPredicate, EqualityPredicate, BucketAllocator>::Swap( const Eldritch2::IntrusiveHashSet<Value, HashPredicate, EqualityPredicate, BucketAllocator>& containerTemplate ) {
		_buckets.swap( containerTemplate._buckets );
		_underlyingContainer.swap( containerTemplate );
	}

// ---------------------------------------------------

	template <typename Value, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	template <typename Cloner, typename Disposer>
	ETForceInlineHint void IntrusiveHashSet<Value, HashPredicate, EqualityPredicate, BucketAllocator>::CloneFrom( const Eldritch2::IntrusiveHashSet<Value, HashPredicate, EqualityPredicate, BucketAllocator>& containerTemplate, Cloner cloner, Disposer disposer ) {
		_underlyingContainer.clone_from( containerTemplate, cloner, [disposer] ( ValueType* const value ) mutable {
			disposer( *value );
		} );
	}

// ---------------------------------------------------

	template <typename Value, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	ETForceInlineHint std::pair<typename IntrusiveHashSet<Value, HashPredicate, EqualityPredicate, BucketAllocator>::Iterator, bool> IntrusiveHashSet<Value, HashPredicate, EqualityPredicate, BucketAllocator>::Insert( Reference value ) {
		return _underlyingContainer.insert( value );
	}

// ---------------------------------------------------

	template <typename Value, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	template <typename SourceIterator>
	ETForceInlineHint void IntrusiveHashSet<Value, HashPredicate, EqualityPredicate, BucketAllocator>::Insert( SourceIterator rangeBegin, SourceIterator rangeEnd ) {
		_underlyingContainer.insert( rangeBegin, rangeEnd );
	}

// ---------------------------------------------------

	template <typename Value, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	template <typename Key>
	ETForceInlineHint std::pair<typename IntrusiveHashSet<Value, HashPredicate, EqualityPredicate, BucketAllocator>::Iterator, bool> IntrusiveHashSet<Value, HashPredicate, EqualityPredicate, BucketAllocator>::InsertCheck( const Key& key, InsertCommitData& commitData ) {
		return this->InsertCheck( key, this->GetHashPredicate(), this->GetEqualityPredicate(), commitData );
	}

// ---------------------------------------------------

	template <typename Value, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	template <typename Key, typename KeyHashPredicate, typename KeyEqualityPredicate>
	ETForceInlineHint std::pair<typename IntrusiveHashSet<Value, HashPredicate, EqualityPredicate, BucketAllocator>::Iterator, bool> IntrusiveHashSet<Value, HashPredicate, EqualityPredicate, BucketAllocator>::InsertCheck( const Key& key, KeyHashPredicate keyHashPredicate, KeyEqualityPredicate keyEqualityPredicate, InsertCommitData& commitData ) {
		return _underlyingContainer.insert_check( key, keyHashPredicate, keyEqualityPredicate, commitData );
	}

// ---------------------------------------------------

	template <typename Value, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	ETForceInlineHint typename IntrusiveHashSet<Value, HashPredicate, EqualityPredicate, BucketAllocator>::Iterator IntrusiveHashSet<Value, HashPredicate, EqualityPredicate, BucketAllocator>::InsertCommit( Reference value, InsertCommitData& commitData ) {
		return _underlyingContainer.insert_commit( value, commitData );
	}

// ---------------------------------------------------

	template <typename Value, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	ETForceInlineHint void IntrusiveHashSet<Value, HashPredicate, EqualityPredicate, BucketAllocator>::Erase( ConstIterator position ) {
		_underlyingContainer.erase( position );
	}

// ---------------------------------------------------

	template <typename Value, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	ETForceInlineHint void IntrusiveHashSet<Value, HashPredicate, EqualityPredicate, BucketAllocator>::Erase( ConstIterator rangeBegin, ConstIterator rangeEnd ) {
		_underlyingContainer.erase( rangeBegin, rangeEnd );
	}

// ---------------------------------------------------

	template <typename Value, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	ETForceInlineHint void IntrusiveHashSet<Value, HashPredicate, EqualityPredicate, BucketAllocator>::Erase( ConstReference element ) {
		_underlyingContainer.erase( _underlyingContainer.iterator_to( element ) );
	}

// ---------------------------------------------------

	template <typename Value, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	template <typename Disposer>
	ETForceInlineHint void IntrusiveHashSet<Value, HashPredicate, EqualityPredicate, BucketAllocator>::EraseAndDispose( ConstIterator position, Disposer disposer ) {
		_underlyingContainer.erase_and_dispose( position, [disposer] ( ValueType* const value ) mutable {
			disposer( *value );
		} );
	}

// ---------------------------------------------------

	template <typename Value, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	template <typename Disposer>
	ETForceInlineHint void IntrusiveHashSet<Value, HashPredicate, EqualityPredicate, BucketAllocator>::EraseAndDispose( ConstIterator rangeBegin, ConstIterator rangeEnd, Disposer disposer ) {
		_underlyingContainer.erase_and_dispose( rangeBegin, rangeEnd, [disposer] ( ValueType* const value ) mutable {
			disposer( *value );
		} );
	}

// ---------------------------------------------------

	template <typename Value, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	template <typename Disposer>
	ETForceInlineHint void IntrusiveHashSet<Value, HashPredicate, EqualityPredicate, BucketAllocator>::EraseAndDispose( ConstReference element, Disposer disposer ) {
		_underlyingContainer.erase_and_dispose( _underlyingContainer.iterator_to( element ), [disposer] ( ValueType* const value ) mutable {
			disposer( *value );
		} );
	}

// ---------------------------------------------------

	template <typename Value, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	ETForceInlineHint void IntrusiveHashSet<Value, HashPredicate, EqualityPredicate, BucketAllocator>::Clear() {
		_underlyingContainer.clear();
	}

// ---------------------------------------------------

	template <typename Value, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	template <typename Disposer>
	ETForceInlineHint void IntrusiveHashSet<Value, HashPredicate, EqualityPredicate, BucketAllocator>::ClearAndDispose( Disposer disposer ) {
		_underlyingContainer.clear_and_dispose( [disposer] ( ValueType* const value ) mutable {
			disposer( *value );
		} );
	}

// ---------------------------------------------------

	template <typename Value, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	ETForceInlineHint bool IntrusiveHashSet<Value, HashPredicate, EqualityPredicate, BucketAllocator>::Empty() const {
		return _underlyingContainer.empty();
	}

// ---------------------------------------------------

	template <typename Value, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	ETForceInlineHint typename IntrusiveHashSet<Value, HashPredicate, EqualityPredicate, BucketAllocator>::SizeType IntrusiveHashSet<Value, HashPredicate, EqualityPredicate, BucketAllocator>::Size() const {
		return _underlyingContainer.size();
	}

// ---------------------------------------------------

	template <typename Value, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	ETForceInlineHint typename IntrusiveHashSet<Value, HashPredicate, EqualityPredicate, BucketAllocator>::SizeType IntrusiveHashSet<Value, HashPredicate, EqualityPredicate, BucketAllocator>::Count( ConstReference value ) const {
		return _underlyingContainer.count( value );
	}

// ---------------------------------------------------

	template <typename Value, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	template <typename Key, typename KeyHashPredicate, typename KeyEqualityPredicate>
	ETForceInlineHint typename IntrusiveHashSet<Value, HashPredicate, EqualityPredicate, BucketAllocator>::SizeType IntrusiveHashSet<Value, HashPredicate, EqualityPredicate, BucketAllocator>::Count( const Key& key, KeyHashPredicate keyHashPredicate, KeyEqualityPredicate keyEqualityPredicate ) const {
		return _underlyingContainer.count( key, keyHashPredicate, keyEqualityPredicate );
	}

// ---------------------------------------------------

	template <typename Value, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	ETForceInlineHint typename IntrusiveHashSet<Value, HashPredicate, EqualityPredicate, BucketAllocator>::SizeType IntrusiveHashSet<Value, HashPredicate, EqualityPredicate, BucketAllocator>::BucketCount() const {
		return _underlyingContainer.bucket_count();
	}

// ---------------------------------------------------

	template <typename Value, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	ETForceInlineHint typename IntrusiveHashSet<Value, HashPredicate, EqualityPredicate, BucketAllocator>::SizeType IntrusiveHashSet<Value, HashPredicate, EqualityPredicate, BucketAllocator>::BucketSize( const SizeType bucketIndex ) const {
		return _underlyingContainer.bucket_size( bucketIndex );
	}

// ---------------------------------------------------

	template <typename Value, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	ETForceInlineHint void IntrusiveHashSet<Value, HashPredicate, EqualityPredicate, BucketAllocator>::Rehash( const SizeType newBucketCount ) {
		_underlyingContainer.rehash( newBucketCount );
	}

// ---------------------------------------------------

	template <typename Value, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	ETForceInlineHint typename IntrusiveHashSet<Value, HashPredicate, EqualityPredicate, BucketAllocator>::HashPredicate IntrusiveHashSet<Value, HashPredicate, EqualityPredicate, BucketAllocator>::GetHashPredicate() const {
		return _underlyingContainer.hash_function();
	}

// ---------------------------------------------------

	template <typename Value, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	ETForceInlineHint typename IntrusiveHashSet<Value, HashPredicate, EqualityPredicate, BucketAllocator>::EqualityPredicate IntrusiveHashSet<Value, HashPredicate, EqualityPredicate, BucketAllocator>::GetEqualityPredicate() const {
		return _underlyingContainer.key_eq();
	}

}	// namespace Eldritch2

namespace std {

	template <typename Value, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	ETInlineHint ETPureFunctionHint auto begin( Eldritch2::IntrusiveHashSet<Value, HashPredicate, EqualityPredicate, BucketAllocator>& collection ) -> decltype(collection.Begin()) {
		return collection.Begin();
	}

// ---------------------------------------------------

	template <typename Value, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	ETInlineHint ETPureFunctionHint auto begin( const Eldritch2::IntrusiveHashSet<Value, HashPredicate, EqualityPredicate, BucketAllocator>& collection ) -> decltype(collection.ConstBegin()) {
		return collection.ConstBegin();
	}

// ---------------------------------------------------

	template <typename Value, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	ETInlineHint ETPureFunctionHint auto end( Eldritch2::IntrusiveHashSet<Value, HashPredicate, EqualityPredicate, BucketAllocator>& collection ) -> decltype(collection.End()) {
		return collection.End();
	}

// ---------------------------------------------------

	template <typename Value, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	ETInlineHint ETPureFunctionHint auto end( const Eldritch2::IntrusiveHashSet<Value, HashPredicate, EqualityPredicate, BucketAllocator>& collection ) -> decltype(collection.ConstEnd()) {
		return collection.ConstEnd();
	}

}	// namespace std