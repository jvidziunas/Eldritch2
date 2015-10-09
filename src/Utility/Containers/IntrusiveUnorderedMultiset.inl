/*==================================================================*\
  IntrusiveUnorderedMultiset.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename StoredObject, typename CollectionOptions, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	template <typename... AllocatorConstructorArguments>
	ETInlineHint IntrusiveUnorderedMultiset<StoredObject, CollectionOptions, HashPredicate, EqualityPredicate, BucketAllocator>::IntrusiveUnorderedMultiset( const SizeType initialBucketCount, AllocatorConstructorArguments&&... allocatorConstructorArguments ) : _buckets( initialBucketCount, ::std::forward<AllocatorConstructorArguments>(allocatorConstructorArguments)... ),
																																																																	 _underlyingContainer( UnderlyingContainer::bucket_traits( _buckets.Data(), _buckets.Size() ) ) {}

// ---------------------------------------------------

	template <typename StoredObject, typename CollectionOptions, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	template <typename... AllocatorConstructorArguments>
	ETInlineHint IntrusiveUnorderedMultiset<StoredObject, CollectionOptions, HashPredicate, EqualityPredicate, BucketAllocator>::IntrusiveUnorderedMultiset( const SizeType initialBucketCount, const HashPredicate& hasher, const EqualityPredicate& equalityPredicate, AllocatorConstructorArguments&&... allocatorConstructorArguments ) : _buckets( initialBucketCount, ::std::forward<AllocatorContructorArguments>( allocatorConstructorArguments )... ),
																																																																																			  _underlyingContainer( UnderlyingContainer::bucket_traits( _buckets.Data(), _buckets.Size() ), hasher, equalityPredicate ) {}
	
// ---------------------------------------------------

	template <typename StoredObject, typename CollectionOptions, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	ETInlineHint IntrusiveUnorderedMultiset<StoredObject, CollectionOptions, HashPredicate, EqualityPredicate, BucketAllocator>::~IntrusiveUnorderedMultiset() {}

// ---------------------------------------------------

	template <typename StoredObject, typename CollectionOptions, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	::Eldritch2::Range<typename IntrusiveUnorderedMultiset<StoredObject, CollectionOptions, HashPredicate, EqualityPredicate, BucketAllocator>::Iterator> IntrusiveUnorderedMultiset<StoredObject, CollectionOptions, HashPredicate, EqualityPredicate, BucketAllocator>::EqualRange( ConstReference value ) {
		const auto	_( _underlyingContainer.equal_range( value ) );

		return { _.first, _.second };
	}

// ---------------------------------------------------

	template <typename StoredObject, typename CollectionOptions, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	::Eldritch2::Range<typename IntrusiveUnorderedMultiset<StoredObject, CollectionOptions, HashPredicate, EqualityPredicate, BucketAllocator>::ConstIterator> IntrusiveUnorderedMultiset<StoredObject, CollectionOptions, HashPredicate, EqualityPredicate, BucketAllocator>::EqualRange( ConstReference value ) const {
		const auto	_( _underlyingContainer.equal_range( value ) );

		return { _.first, _.second };
	}

// ---------------------------------------------------

	template <typename StoredObject, typename CollectionOptions, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	template<typename Key, typename KeyHashPredicate, typename KeyEqualityPredicate>
	::Eldritch2::Range<typename IntrusiveUnorderedMultiset<StoredObject, CollectionOptions, HashPredicate, EqualityPredicate, BucketAllocator>::Iterator> IntrusiveUnorderedMultiset<StoredObject, CollectionOptions, HashPredicate, EqualityPredicate, BucketAllocator>::EqualRange( const Key& key, KeyHashPredicate keyHashPredicate, KeyEqualityPredicate keyEqualityPredicate ) {
		const auto _( _underlyingContainer.equal_range( key, keyHashPredicate, keyEqualityPredicate ) );

		return { _.first, _.second };
	}

// ---------------------------------------------------

	template <typename StoredObject, typename CollectionOptions, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	template <typename Key, typename KeyHashPredicate, typename KeyEqualityPredicate>
	::Eldritch2::Range<typename IntrusiveUnorderedMultiset<StoredObject, CollectionOptions, HashPredicate, EqualityPredicate, BucketAllocator>::ConstIterator> IntrusiveUnorderedMultiset<StoredObject, CollectionOptions, HashPredicate, EqualityPredicate, BucketAllocator>::EqualRange( const Key& key, KeyHashPredicate keyHashPredicate, KeyEqualityPredicate keyEqualityPredicate ) const {
		const auto _( _underlyingContainer.equal_range( key, keyHashPredicate, keyEqualityPredicate ) );

		return { _.first, _.second };
	}

// ---------------------------------------------------

	template <typename StoredObject, typename CollectionOptions, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	template <typename Key>
	::Eldritch2::Range<typename IntrusiveUnorderedMultiset<StoredObject, CollectionOptions, HashPredicate, EqualityPredicate, BucketAllocator>::Iterator> IntrusiveUnorderedMultiset<StoredObject, CollectionOptions, HashPredicate, EqualityPredicate, BucketAllocator>::EqualRange( const Key& key ) {
		return this->EqualRange( key, this->GetHashPredicate(), this->GetEqualityPredicate() );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename CollectionOptions, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	template <typename Key>
	::Eldritch2::Range<typename IntrusiveUnorderedMultiset<StoredObject, CollectionOptions, HashPredicate, EqualityPredicate, BucketAllocator>::ConstIterator> IntrusiveUnorderedMultiset<StoredObject, CollectionOptions, HashPredicate, EqualityPredicate, BucketAllocator>::EqualRange( const Key& key ) const {
		return this->EqualRange( key, this->GetHashPredicate(), this->GetEqualityPredicate() );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename CollectionOptions, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	template <typename Key, typename KeyHashPredicate, typename KeyEqualityPredicate>
	ETForceInlineHint typename IntrusiveUnorderedMultiset<StoredObject, CollectionOptions, HashPredicate, EqualityPredicate, BucketAllocator>::SizeType IntrusiveUnorderedMultiset<StoredObject, CollectionOptions, HashPredicate, EqualityPredicate, BucketAllocator>::RemoveIf( const Key& key, KeyHashPredicate keyHashPredicate, KeyEqualityPredicate keyEqualityPredicate ) {
		return _underlyingContainer.erase( key, keyHashPredicate, keyEqualityPredicate );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename CollectionOptions, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	template<typename Key, typename KeyHashPredicate, typename KeyEqualityPredicate, typename Disposer>
	ETForceInlineHint typename IntrusiveUnorderedMultiset<StoredObject, CollectionOptions, HashPredicate, EqualityPredicate, BucketAllocator>::SizeType IntrusiveUnorderedMultiset<StoredObject, CollectionOptions, HashPredicate, EqualityPredicate, BucketAllocator>::RemoveIfAndDispose( const Key& key, KeyHashPredicate keyHashPredicate, KeyEqualityPredicate keyEqualityPredicate, Disposer disposer ) {
		return _underlyingContainer.erase_and_dispose( key, keyHashPredicate, keyEqualityPredicate, [disposer] ( ValueType* const value ) mutable {
			disposer( *value );
		} );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename CollectionOptions, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	ETForceInlineHint typename IntrusiveUnorderedMultiset<StoredObject, CollectionOptions, HashPredicate, EqualityPredicate, BucketAllocator>::Iterator IntrusiveUnorderedMultiset<StoredObject, CollectionOptions, HashPredicate, EqualityPredicate, BucketAllocator>::Begin() {
		return _underlyingContainer.begin();
	}

// ---------------------------------------------------

	template <typename StoredObject, typename CollectionOptions, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	ETForceInlineHint typename IntrusiveUnorderedMultiset<StoredObject, CollectionOptions, HashPredicate, EqualityPredicate, BucketAllocator>::ConstIterator IntrusiveUnorderedMultiset<StoredObject, CollectionOptions, HashPredicate, EqualityPredicate, BucketAllocator>::Begin() const {
		return _underlyingContainer.begin();
	}

// ---------------------------------------------------

	template <typename StoredObject, typename CollectionOptions, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	ETForceInlineHint typename IntrusiveUnorderedMultiset<StoredObject, CollectionOptions, HashPredicate, EqualityPredicate, BucketAllocator>::Iterator IntrusiveUnorderedMultiset<StoredObject, CollectionOptions, HashPredicate, EqualityPredicate, BucketAllocator>::End() {
		return _underlyingContainer.end();
	}

// ---------------------------------------------------

	template <typename StoredObject, typename CollectionOptions, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	ETForceInlineHint typename IntrusiveUnorderedMultiset<StoredObject, CollectionOptions, HashPredicate, EqualityPredicate, BucketAllocator>::ConstIterator IntrusiveUnorderedMultiset<StoredObject, CollectionOptions, HashPredicate, EqualityPredicate, BucketAllocator>::End() const {
		return _underlyingContainer.end();
	}

// ---------------------------------------------------

	template <typename StoredObject, typename CollectionOptions, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	ETForceInlineHint typename IntrusiveUnorderedMultiset<StoredObject, CollectionOptions, HashPredicate, EqualityPredicate, BucketAllocator>::ConstIterator IntrusiveUnorderedMultiset<StoredObject, CollectionOptions, HashPredicate, EqualityPredicate, BucketAllocator>::ConstBegin() const {
		return _underlyingContainer.cbegin();
	}

// ---------------------------------------------------

	template <typename StoredObject, typename CollectionOptions, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	ETForceInlineHint typename IntrusiveUnorderedMultiset<StoredObject, CollectionOptions, HashPredicate, EqualityPredicate, BucketAllocator>::ConstIterator IntrusiveUnorderedMultiset<StoredObject, CollectionOptions, HashPredicate, EqualityPredicate, BucketAllocator>::ConstEnd() const {
		return _underlyingContainer.cend();
	}

// ---------------------------------------------------

	template <typename StoredObject, typename CollectionOptions, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	template <class AlternateAllocator, typename Cloner, typename Disposer>
	ETForceInlineHint void IntrusiveUnorderedMultiset<StoredObject, CollectionOptions, HashPredicate, EqualityPredicate, BucketAllocator>::CloneFrom( const ::Eldritch2::IntrusiveUnorderedMultiset<StoredObject, CollectionOptions, HashPredicate, EqualityPredicate, AlternateAllocator>& containerTemplate, Cloner cloner, Disposer disposer ) {
		_underlyingContainer.clone_from( containerTemplate, cloner, [disposer] ( ValueType* const value ) mutable {
			disposer( *value );
		} );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename CollectionOptions, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	ETForceInlineHint typename IntrusiveUnorderedMultiset<StoredObject, CollectionOptions, HashPredicate, EqualityPredicate, BucketAllocator>::Iterator IntrusiveUnorderedMultiset<StoredObject, CollectionOptions, HashPredicate, EqualityPredicate, BucketAllocator>::Insert( Reference value ) {
		return _underlyingContainer.insert( value );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename CollectionOptions, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	template <typename SourceIterator>
	ETForceInlineHint void IntrusiveUnorderedMultiset<StoredObject, CollectionOptions, HashPredicate, EqualityPredicate, BucketAllocator>::Insert( SourceIterator rangeBegin, SourceIterator rangeEnd ) {
		_underlyingContainer.insert( rangeBegin, rangeEnd );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename CollectionOptions, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	ETForceInlineHint void IntrusiveUnorderedMultiset<StoredObject, CollectionOptions, HashPredicate, EqualityPredicate, BucketAllocator>::Erase( ConstIterator position ) {
		_underlyingContainer.erase( position );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename CollectionOptions, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	ETForceInlineHint void IntrusiveUnorderedMultiset<StoredObject, CollectionOptions, HashPredicate, EqualityPredicate, BucketAllocator>::Erase( ConstIterator rangeBegin, ConstIterator rangeEnd ) {
		_underlyingContainer.erase( rangeBegin, rangeEnd );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename CollectionOptions, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	ETForceInlineHint void IntrusiveUnorderedMultiset<StoredObject, CollectionOptions, HashPredicate, EqualityPredicate, BucketAllocator>::Erase( ConstReference element ) {
		_underlyingContainer.erase( _underlyingContainer.iterator_to( element ) );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename CollectionOptions, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	template <typename Disposer>
	ETForceInlineHint void IntrusiveUnorderedMultiset<StoredObject, CollectionOptions, HashPredicate, EqualityPredicate, BucketAllocator>::EraseAndDispose( ConstIterator position, Disposer disposer ) {
		_underlyingContainer.erase_and_dispose( position, [disposer] ( ValueType* const value ) mutable {
			disposer( *value );
		} );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename CollectionOptions, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	template <typename Disposer>
	ETForceInlineHint void IntrusiveUnorderedMultiset<StoredObject, CollectionOptions, HashPredicate, EqualityPredicate, BucketAllocator>::EraseAndDispose( ConstIterator rangeBegin, ConstIterator rangeEnd, Disposer disposer ) {
		_underlyingContainer.erase_and_dispose( rangeBegin, rangeEnd, [disposer] ( ValueType* const value ) mutable {
			disposer( *value );
		} );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename CollectionOptions, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	template <typename Disposer>
	ETForceInlineHint void IntrusiveUnorderedMultiset<StoredObject, CollectionOptions, HashPredicate, EqualityPredicate, BucketAllocator>::EraseAndDispose( ConstReference element, Disposer disposer ) {
		_underlyingContainer.erase_and_dispose( _underlyingContainer.iterator_to( element ), [disposer] ( ValueType* const value ) mutable {
			disposer( *value );
		} );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename CollectionOptions, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	ETForceInlineHint void IntrusiveUnorderedMultiset<StoredObject, CollectionOptions, HashPredicate, EqualityPredicate, BucketAllocator>::Clear() {
		_underlyingContainer.clear();
	}

// ---------------------------------------------------

	template <typename StoredObject, typename CollectionOptions, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	template <typename Disposer>
	ETForceInlineHint void IntrusiveUnorderedMultiset<StoredObject, CollectionOptions, HashPredicate, EqualityPredicate, BucketAllocator>::ClearAndDispose( Disposer disposer ) {
		_underlyingContainer.clear_and_dispose( [disposer] ( ValueType* const value ) mutable {
			disposer( *value );
		} );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename CollectionOptions, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	ETForceInlineHint bool IntrusiveUnorderedMultiset<StoredObject, CollectionOptions, HashPredicate, EqualityPredicate, BucketAllocator>::Empty() const {
		return _underlyingContainer.empty();
	}

// ---------------------------------------------------

	template <typename StoredObject, typename CollectionOptions, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	ETForceInlineHint typename IntrusiveUnorderedMultiset<StoredObject, CollectionOptions, HashPredicate, EqualityPredicate, BucketAllocator>::SizeType IntrusiveUnorderedMultiset<StoredObject, CollectionOptions, HashPredicate, EqualityPredicate, BucketAllocator>::Size() const {
		return _underlyingContainer.size();
	}

// ---------------------------------------------------

	template <typename StoredObject, typename CollectionOptions, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	ETForceInlineHint typename IntrusiveUnorderedMultiset<StoredObject, CollectionOptions, HashPredicate, EqualityPredicate, BucketAllocator>::SizeType IntrusiveUnorderedMultiset<StoredObject, CollectionOptions, HashPredicate, EqualityPredicate, BucketAllocator>::Count( ConstReference value ) const {
		return _underlyingContainer.count( value );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename CollectionOptions, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	template <typename Key, typename KeyHashPredicate, typename KeyEqualityPredicate>
	ETForceInlineHint typename IntrusiveUnorderedMultiset<StoredObject, CollectionOptions, HashPredicate, EqualityPredicate, BucketAllocator>::SizeType IntrusiveUnorderedMultiset<StoredObject, CollectionOptions, HashPredicate, EqualityPredicate, BucketAllocator>::Count( const Key& key, KeyHashPredicate keyHashPredicate, KeyEqualityPredicate keyEqualityPredicate ) const {
		return _underlyingContainer.count( key, keyHashPredicate, keyEqualityPredicate );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename CollectionOptions, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	ETForceInlineHint typename IntrusiveUnorderedMultiset<StoredObject, CollectionOptions, HashPredicate, EqualityPredicate, BucketAllocator>::SizeType IntrusiveUnorderedMultiset<StoredObject, CollectionOptions, HashPredicate, EqualityPredicate, BucketAllocator>::BucketCount() const {
		return _underlyingContainer.bucket_count();
	}

// ---------------------------------------------------

	template <typename StoredObject, typename CollectionOptions, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	ETForceInlineHint typename IntrusiveUnorderedMultiset<StoredObject, CollectionOptions, HashPredicate, EqualityPredicate, BucketAllocator>::SizeType IntrusiveUnorderedMultiset<StoredObject, CollectionOptions, HashPredicate, EqualityPredicate, BucketAllocator>::BucketSize( const SizeType bucketIndex ) const {
		return _underlyingContainer.bucket_size( bucketIndex );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename CollectionOptions, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	ETForceInlineHint void IntrusiveUnorderedMultiset<StoredObject, CollectionOptions, HashPredicate, EqualityPredicate, BucketAllocator>::Rehash( const SizeType newBucketCount ) {
		_underlyingContainer.rehash( newBucketCount );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename CollectionOptions, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	ETForceInlineHint typename IntrusiveUnorderedMultiset<StoredObject, CollectionOptions, HashPredicate, EqualityPredicate, BucketAllocator>::HashPredicate IntrusiveUnorderedMultiset<StoredObject, CollectionOptions, HashPredicate, EqualityPredicate, BucketAllocator>::GetHashPredicate() const {
		return _underlyingContainer.hash_function();
	}

// ---------------------------------------------------

	template <typename StoredObject, typename CollectionOptions, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	ETForceInlineHint typename IntrusiveUnorderedMultiset<StoredObject, CollectionOptions, HashPredicate, EqualityPredicate, BucketAllocator>::EqualityPredicate IntrusiveUnorderedMultiset<StoredObject, CollectionOptions, HashPredicate, EqualityPredicate, BucketAllocator>::GetEqualityPredicate() const {
		return _underlyingContainer.key_eq();
	}

}	// namespace Eldritch2

namespace std {

	template <typename StoredObject, typename CollectionOptions, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	ETInlineHint ETNoAliasHint auto begin( ::Eldritch2::IntrusiveUnorderedMultiset<StoredObject, CollectionOptions, HashPredicate, EqualityPredicate, BucketAllocator>& collection ) -> decltype(collection.Begin()) {
		return collection.Begin();
	}

// ---------------------------------------------------

	template <typename StoredObject, typename CollectionOptions, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	ETInlineHint ETNoAliasHint auto begin( const ::Eldritch2::IntrusiveUnorderedMultiset<StoredObject, CollectionOptions, HashPredicate, EqualityPredicate, BucketAllocator>& collection ) -> decltype(collection.ConstBegin()) {
		return collection.ConstBegin();
	}

// ---------------------------------------------------

	template <typename StoredObject, typename CollectionOptions, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	ETInlineHint ETNoAliasHint auto end( ::Eldritch2::IntrusiveUnorderedMultiset<StoredObject, CollectionOptions, HashPredicate, EqualityPredicate, BucketAllocator>& collection ) -> decltype(collection.End()) {
		return collection.End();
	}

// ---------------------------------------------------

	template <typename StoredObject, typename CollectionOptions, typename HashPredicate, typename EqualityPredicate, typename BucketAllocator>
	ETInlineHint ETNoAliasHint auto end( const ::Eldritch2::IntrusiveUnorderedMultiset<StoredObject, CollectionOptions, HashPredicate, EqualityPredicate, BucketAllocator>& collection ) -> decltype(collection.ConstEnd()) {
		return collection.ConstEnd();
	}

}	// namespace std