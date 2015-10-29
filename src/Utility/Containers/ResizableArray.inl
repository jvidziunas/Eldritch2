/*==================================================================*\
  ResizableArray.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Algorithms.hpp>
#include <type_traits>
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename StoredObject, typename Allocator>
	ETInlineHint ResizableArray<StoredObject, Allocator>::ResizableArray( AllocatorType&& allocator ) : _underlyingContainer( ::std::move( allocator ) ) {}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint ResizableArray<StoredObject, Allocator>::ResizableArray( const SizeType initialSize, AllocatorType&& allocator ) : _underlyingContainer( initialSize, ::std::move( allocator ) ) {}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint ResizableArray<StoredObject, Allocator>::ResizableArray( const StoredObject* first, const StoredObject* last, AllocatorType&& allocator ) : _underlyingContainer( first, last, ::std::move( allocator ) ) {}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	template <class AlternateAllocator>
	ETInlineHint ResizableArray<StoredObject, Allocator>::ResizableArray( const ::Eldritch2::ResizableArray<StoredObject, AlternateAllocator>& containerTemplate, AllocatorType&& allocator ) : _underlyingContainer( containerTemplate.Begin(), containerTemplate.End(), ::std::move( allocator ) ) {}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint ResizableArray<StoredObject, Allocator>::ResizableArray( ::Eldritch2::ResizableArray<StoredObject, Allocator>&& moveSource ) : _underlyingContainer( ::std::move( moveSource._underlyingContainer ) ) {}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint typename ::Eldritch2::ResizableArray<StoredObject, Allocator>::SizeType ResizableArray<StoredObject, Allocator>::IndexOf( ConstReference item, const SizeType searchHint ) const {
		return _underlyingContainer.index_of( item, searchHint );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint typename ::Eldritch2::ResizableArray<StoredObject, Allocator>::Iterator ResizableArray<StoredObject, Allocator>::Find( ConstReference itemTemplate ) {
		return _underlyingContainer.find( itemTemplate );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint typename ::Eldritch2::ResizableArray<StoredObject, Allocator>::ConstIterator ResizableArray<StoredObject, Allocator>::Find( ConstReference itemTemplate ) const {
		return _underlyingContainer.find( itemTemplate );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint typename ::Eldritch2::ResizableArray<StoredObject, Allocator>::Iterator ResizableArray<StoredObject, Allocator>::Find( ConstReference itemTemplate, Iterator searchHint ) {
		return _underlyingContainer.find( itemTemplate, searchHint );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint typename ::Eldritch2::ResizableArray<StoredObject, Allocator>::ConstIterator ResizableArray<StoredObject, Allocator>::Find( ConstReference itemTemplate, ConstIterator searchHint ) const {
		return _underlyingContainer.find( itemTemplate, searchHint );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint bool ResizableArray<StoredObject, Allocator>::Contains( ConstReference itemTemplate ) const {
		return _underlyingContainer.end() != _underlyingContainer.find( itemTemplate );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	template <typename Predicate>
	ETInlineHint typename ::Eldritch2::ResizableArray<StoredObject, Allocator>::Iterator ResizableArray<StoredObject, Allocator>::RemoveIf( Predicate predicate ) {
		return ::Eldritch2::Utility::RemoveIf( _underlyingContainer.begin(), _underlyingContainer.end(), predicate );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	template <typename Predicate>
	ETInlineHint void ResizableArray<StoredObject, Allocator>::Sort( Predicate predicate ) {
		::Eldritch2::Utility::Sort( this->Begin(), this->End(), predicate );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint typename ::Eldritch2::ResizableArray<StoredObject, Allocator>& ResizableArray<StoredObject, Allocator>::operator=( const ::Eldritch2::ResizableArray<StoredObject, Allocator>& containerTemplate ) {
		_underlyingContainer.copy( containerTemplate );

		return *this;
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint void ResizableArray<StoredObject, Allocator>::CloneFrom( const ::Eldritch2::ResizableArray<StoredObject, Allocator>& containerTemplate ) {
		_underlyingContainer.copy( containerTemplate );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint void ResizableArray<StoredObject, Allocator>::Swap( ::Eldritch2::ResizableArray<StoredObject, Allocator>& container ) {
		const SizeType	otherSize( container.Size() );
		const SizeType	thisSize( this->Size() );

		if( thisSize < otherSize ) {
			this->Reserve( otherSize );

			for( auto&& element : ::Eldritch2::Range<Pointer>( container.Data() + thisSize, container.End() ) ) {
				this->PushBack( element );
			}
		} else if( thisSize > otherSize ) {
			container.Reserve( thisSize );

			for( auto&& element : ::Eldritch2::Range<Pointer>( this->Data() + otherSize, this->End() ) ) {
				container.PushBack( element );
			}
		}

		for( Pointer element( this->Data() ), end( this->Data() + Min( thisSize, otherSize ) ), other( container.Data() ); element != end; ++element, ++other ) {
			::std::swap( *element, *other );
		}
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint typename ::Eldritch2::ResizableArray<StoredObject, Allocator>::Iterator ResizableArray<StoredObject, Allocator>::Begin() {
		return _underlyingContainer.begin();
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint typename ::Eldritch2::ResizableArray<StoredObject, Allocator>::ConstIterator ResizableArray<StoredObject, Allocator>::Begin() const {
		return _underlyingContainer.begin();
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint typename ::Eldritch2::ResizableArray<StoredObject, Allocator>::ConstIterator ResizableArray<StoredObject, Allocator>::ConstBegin() const {
		return this->Begin();
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint typename ::Eldritch2::ResizableArray<StoredObject, Allocator>::ReverseIterator ResizableArray<StoredObject, Allocator>::ReverseBegin() {
		return ReverseIterator( _underlyingContainer.end() );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint typename ::Eldritch2::ResizableArray<StoredObject, Allocator>::ReverseConstIterator ResizableArray<StoredObject, Allocator>::ReverseBegin() const {
		return ReverseConstIterator( _underlyingContainer.end() );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint typename ::Eldritch2::ResizableArray<StoredObject, Allocator>::Iterator ResizableArray<StoredObject, Allocator>::End() {
		return _underlyingContainer.end();
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint typename ::Eldritch2::ResizableArray<StoredObject, Allocator>::ConstIterator ResizableArray<StoredObject, Allocator>::End() const {
		return _underlyingContainer.end();
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint typename ::Eldritch2::ResizableArray<StoredObject, Allocator>::ConstIterator ResizableArray<StoredObject, Allocator>::ConstEnd() const {
		return this->End();
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint typename ::Eldritch2::ResizableArray<StoredObject, Allocator>::ReverseIterator ResizableArray<StoredObject, Allocator>::ReverseEnd() {
		return ReverseIterator( _underlyingContainer.begin() );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint typename ::Eldritch2::ResizableArray<StoredObject, Allocator>::ReverseConstIterator ResizableArray<StoredObject, Allocator>::ReverseEnd() const {
		return ReverseConstIterator( _underlyingContainer.begin() );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint typename ::Eldritch2::ResizableArray<StoredObject, Allocator>::Reference ResizableArray<StoredObject, Allocator>::Front() {
		return _underlyingContainer.front();
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint typename ::Eldritch2::ResizableArray<StoredObject, Allocator>::ConstReference ResizableArray<StoredObject, Allocator>::Front() const {
		return _underlyingContainer.front();
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint typename ::Eldritch2::ResizableArray<StoredObject, Allocator>::Reference ResizableArray<StoredObject, Allocator>::Back() {
		return _underlyingContainer.back();
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint typename ::Eldritch2::ResizableArray<StoredObject, Allocator>::ConstReference ResizableArray<StoredObject, Allocator>::Back() const {
		return _underlyingContainer.back();
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint void ResizableArray<StoredObject, Allocator>::PushBack( ConstReference itemTemplate ) {
		_underlyingContainer.push_back( itemTemplate );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint void ResizableArray<StoredObject, Allocator>::PushBack( ValueType&& itemTemplate ) {
		_underlyingContainer.push_back( ::std::move(itemTemplate) );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint void ResizableArray<StoredObject, Allocator>::PushBack() {
		_underlyingContainer.push_back();
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	template <typename... ElementConstructorArguments>
	ETInlineHint void ResizableArray<StoredObject, Allocator>::EmplaceBack( ElementConstructorArguments&&... elementConstructorArguments ) {
		_underlyingContainer.push_back( ::std::forward<ElementConstructorArguments>( elementConstructorArguments )... );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint void ResizableArray<StoredObject, Allocator>::PopBack() {
		_underlyingContainer.pop_back();
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint typename ResizableArray<StoredObject, Allocator>::Reference ResizableArray<StoredObject, Allocator>::operator[]( const SizeType elementIndex ) {
		return _underlyingContainer[elementIndex];
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint typename ResizableArray<StoredObject, Allocator>::ConstReference ResizableArray<StoredObject, Allocator>::operator[]( const SizeType elementIndex ) const {
		return _underlyingContainer[elementIndex];
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint typename ResizableArray<StoredObject, Allocator>::Reference ResizableArray<StoredObject, Allocator>::At( const SizeType elementIndex ) {
		return _underlyingContainer.at( elementIndex );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint typename ResizableArray<StoredObject, Allocator>::ConstReference ResizableArray<StoredObject, Allocator>::At( const SizeType elementIndex ) const {
		return _underlyingContainer.at( elementIndex );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint typename ResizableArray<StoredObject, Allocator>::Pointer ResizableArray<StoredObject, Allocator>::Data() {
		return _underlyingContainer.data();
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint typename ResizableArray<StoredObject, Allocator>::ConstPointer ResizableArray<StoredObject, Allocator>::Data() const {
		return _underlyingContainer.data();
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint void ResizableArray<StoredObject, Allocator>::Assign( ConstPointer first, ConstPointer last ) {
		_underlyingContainer.assign( first, last );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint void ResizableArray<StoredObject, Allocator>::Insert( const SizeType initialIndex, const SizeType itemCount, ConstReference itemTemplate ) {
		_underlyingContainer.insert( initialIndex, itemCount, itemTemplate );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint void ResizableArray<StoredObject, Allocator>::Insert( Iterator position, const SizeType itemCount, ConstReference itemTemplate ) {
		_underlyingContainer.insert( initialIndex, itemCount, itemTemplate );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint typename ResizableArray<StoredObject, Allocator>::Iterator ResizableArray<StoredObject, Allocator>::Insert( Iterator position, ConstReference itemTemplate ) {
		return _underlyingContainer.insert( position, itemTemplate );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint typename ResizableArray<StoredObject, Allocator>::Iterator ResizableArray<StoredObject, Allocator>::Insert( Iterator position, ValueType&& itemTemplate ) {
		return _underlyingContainer.insert( position, ::std::move(itemTemplate) );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	template <typename... ElementConstructorArguments>
	ETInlineHint void ResizableArray<StoredObject, Allocator>::Emplace( const SizeType initialIndex, const SizeType itemCount, ElementConstructorArguments&&... elementConstructorArguments ) {
		_underlyingContainer.insert( initialIndex, itemCount, ::std::forward<ElementConstructorArguments>( elementConstructorArguments )... );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	template <typename... ElementConstructorArguments>
	ETInlineHint void ResizableArray<StoredObject, Allocator>::Emplace( Iterator position, const SizeType itemCount, ElementConstructorArguments&&... elementConstructorArguments ) {
		_underlyingContainer.insert( position, itemCount, ::std::forward<ElementConstructorArguments>( elementConstructorArguments )... );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	template <typename... ElementConstructorArguments>
	ETInlineHint typename ::Eldritch2::ResizableArray<StoredObject, Allocator>::Iterator ResizableArray<StoredObject, Allocator>::Emplace( Iterator position, ElementConstructorArguments&&... elementConstructorArguments ) {
		return _underlyingContainer.insert( position, ::std::forward<ElementConstructorArguments>( elementConstructorArguments )... );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint typename ::Eldritch2::ResizableArray<StoredObject, Allocator>::Iterator ResizableArray<StoredObject, Allocator>::Erase( Iterator position ) {
		return _underlyingContainer.erase( position );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint void ResizableArray<StoredObject, Allocator>::Erase( Iterator position, const UnorderedSemantics ) {
		_underlyingContainer.erase_unordered( position );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint typename ResizableArray<StoredObject, Allocator>::Iterator ResizableArray<StoredObject, Allocator>::Erase( Iterator first, Iterator last ) {
		return _underlyingContainer.erase( first, last );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	template <typename DisposerPredicate>
	ETInlineHint typename ::Eldritch2::ResizableArray<StoredObject, Allocator>::Iterator ResizableArray<StoredObject, Allocator>::EraseAndDispose( const Iterator position, DisposerPredicate disposer ) {
		return _underlyingContainer.erase( (disposer(*position), position) );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	template <typename DisposerPredicate>
	ETInlineHint void ResizableArray<StoredObject, Allocator>::EraseAndDispose( const Iterator position, DisposerPredicate disposer, const UnorderedSemantics ) {
		_underlyingContainer.erase_unordered( (disposer( *position ), position) );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	template <typename DisposerPredicate>
	ETInlineHint typename ::Eldritch2::ResizableArray<StoredObject, Allocator>::Iterator ResizableArray<StoredObject, Allocator>::EraseAndDispose( Iterator beginPosition, Iterator endPosition, DisposerPredicate disposer ) {
		for( auto& value : ::Eldritch2::Range<Iterator>( beginPosition, endPosition ) ) {
			disposer( value );
		}

		return _underlyingContainer.erase( beginPosition, endPosition );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint void ResizableArray<StoredObject, Allocator>::Clear() {
		_underlyingContainer.clear();
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint void ResizableArray<StoredObject, Allocator>::Clear( const ReleaseMemorySemantics ) {
		_underlyingContainer.reset();
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	template <typename Disposer>
	ETInlineHint void ResizableArray<StoredObject, Allocator>::ClearAndDispose( Disposer disposer ) {
		for( auto& value : ::Eldritch2::Range<Iterator>( this->Begin(), this->End() ) ) {
			disposer( value );
		}

		this->Clear();
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	template <typename Disposer>
	ETInlineHint void ResizableArray<StoredObject, Allocator>::ClearAndDispose( Disposer disposer, const ReleaseMemorySemantics ) {
		for( auto& value : ::Eldritch2::Range<Iterator>( this->Begin(), this->End() ) ) {
			disposer( value );
		}

		this->Clear( ::Eldritch2::ReleaseMemorySemantics );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint void ResizableArray<StoredObject, Allocator>::Resize( const SizeType elementCount ) {
		_underlyingContainer.resize( elementCount );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint void ResizableArray<StoredObject, Allocator>::Resize( const SizeType elementCount, ConstReference value ) {
		_underlyingContainer.resize( elementCount, value );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint typename ::Eldritch2::ResizableArray<StoredObject, Allocator>::SizeType ResizableArray<StoredObject, Allocator>::Size() const {
		return _underlyingContainer.size();
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint bool ResizableArray<StoredObject, Allocator>::IsEmpty() const {
		return _underlyingContainer.empty();
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint ResizableArray<StoredObject, Allocator>::operator bool() const {
		return !_underlyingContainer.empty();
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint typename ::Eldritch2::ResizableArray<StoredObject, Allocator>::SizeType ResizableArray<StoredObject, Allocator>::GetCapacity() const {
		return _underlyingContainer.capacity();
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint void ResizableArray<StoredObject, Allocator>::SetCapacity( const SizeType newCapacity ) {
		_underlyingContainer.set_capacity( newCapacity );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint void ResizableArray<StoredObject, Allocator>::Reserve( const SizeType elementCountHint ) {
		_underlyingContainer.reserve( elementCountHint );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint typename ::Eldritch2::ResizableArray<StoredObject, Allocator>::SizeType ResizableArray<StoredObject, Allocator>::GetHighWatermark() const {
		return _underlyingContainer.get_high_watermark();
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint typename const ::Eldritch2::ResizableArray<StoredObject, Allocator>::AllocatorType& ResizableArray<StoredObject, Allocator>::GetAllocator() const {
		return _underlyingContainer.get_allocator();
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint bool ResizableArray<StoredObject, Allocator>::ValidateIterator( ConstIterator iterator ) const {
		return _underlyingContainer.validate_iterator( iterator );
	}

}	// namespace Eldritch2

namespace std {

	template <typename StoredObject, typename Allocator>
	ETInlineHint ETNoAliasHint auto begin( ::Eldritch2::ResizableArray<StoredObject, Allocator>& collection ) -> decltype(collection.Begin()) {
		return collection.Begin();
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint ETNoAliasHint auto begin( const ::Eldritch2::ResizableArray<StoredObject, Allocator>& collection ) -> decltype(collection.ConstBegin()) {
		return collection.ConstBegin();
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint ETNoAliasHint auto end( ::Eldritch2::ResizableArray<StoredObject, Allocator>& collection ) -> decltype(collection.End()) {
		return collection.End();
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint ETNoAliasHint auto end( const ::Eldritch2::ResizableArray<StoredObject, Allocator>& collection ) -> decltype(collection.ConstEnd()) {
		return collection.ConstEnd();
	}

}	// namespace std