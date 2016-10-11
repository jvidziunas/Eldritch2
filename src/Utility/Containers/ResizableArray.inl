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

//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename Value, typename Allocator>
	template <class /*SFINAE*/>
	ETInlineHint ResizableArray<Value, Allocator>::ResizableArray( const ResizableArray<Value, Allocator>& containerTemplate, const AllocatorType& allocator ) : _underlyingContainer( containerTemplate._underlyingContainer, allocator ) {}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	ETInlineHint ResizableArray<Value, Allocator>::ResizableArray( ConstPointer first, ConstPointer last, const AllocatorType& allocator ) : _underlyingContainer( first, last, allocator ) {}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	ETInlineHint ResizableArray<Value, Allocator>::ResizableArray( SizeType initialSize, const AllocatorType& allocator ) : _underlyingContainer( initialSize, allocator ) {}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	ETInlineHint ResizableArray<Value, Allocator>::ResizableArray( const AllocatorType& allocator ) : _underlyingContainer( allocator ) {}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	ETInlineHint ResizableArray<Value, Allocator>::ResizableArray( ResizableArray<Value, Allocator>&& moveSource ) : _underlyingContainer( eastl::move( moveSource._underlyingContainer ) ) {}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	ETInlineHint typename Eldritch2::ResizableArray<Value, Allocator>::ConstIterator ResizableArray<Value, Allocator>::Find( ConstReference itemTemplate ) const {
		return _underlyingContainer.find( itemTemplate );
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	ETInlineHint typename Eldritch2::ResizableArray<Value, Allocator>::Iterator ResizableArray<Value, Allocator>::Find( ConstReference itemTemplate ) {
		return _underlyingContainer.find( itemTemplate );
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	ETInlineHint typename Eldritch2::ResizableArray<Value, Allocator>::ConstIterator ResizableArray<Value, Allocator>::Find( ConstReference itemTemplate, ConstIterator searchHint ) const {
		return _underlyingContainer.find( itemTemplate, searchHint );
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	ETInlineHint typename Eldritch2::ResizableArray<Value, Allocator>::Iterator ResizableArray<Value, Allocator>::Find( ConstReference itemTemplate, Iterator searchHint ) {
		return _underlyingContainer.find( itemTemplate, searchHint );
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	ETInlineHint bool ResizableArray<Value, Allocator>::Contains( ConstReference itemTemplate ) const {
		return _underlyingContainer.contains( itemTemplate );
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	template <typename Predicate>
	ETInlineHint typename Eldritch2::ResizableArray<Value, Allocator>::Iterator ResizableArray<Value, Allocator>::RemoveIf( Predicate predicate ) {
	//	Reimplementing for now, as the eastl remove_if implementation does not use move semantics.
		const auto	last( _underlyingContainer.end() );
		auto		first( eastl::find_if( _underlyingContainer.begin(), last, predicate ) );

		if( first == last ) {
			return first;
		}

		auto	next( first++ );
		for( ; first != last; ++first ) {
			if( predicate( *first ) ) {
				continue;
			}

			*next++ = eastl::move( *first );
		}
			
		return next;
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	template <typename Predicate>
	ETInlineHint void ResizableArray<Value, Allocator>::Sort( Predicate predicate ) {
		eastl::sort( _underlyingContainer.begin(), _underlyingContainer.end(), predicate );
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	template <class /*SFINAE*/>
	ETInlineHint typename Eldritch2::ResizableArray<Value, Allocator>& ResizableArray<Value, Allocator>::operator=( const ResizableArray<Value, Allocator>& containerTemplate ) {
		_underlyingContainer = containerTemplate._underlyingContainer;

		return *this;
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	ETInlineHint typename Eldritch2::ResizableArray<Value, Allocator>& ResizableArray<Value, Allocator>::operator=( ResizableArray<Value, Allocator>&& containerTemplate ) {
		_underlyingContainer = eastl::move( containerTemplate._underlyingContainer );

		return *this;
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	ETInlineHint void ResizableArray<Value, Allocator>::Swap( Eldritch2::ResizableArray<Value, Allocator>& container ) {
		_underlyingContainer.swap( container._underlyingContainer );
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	ETInlineHint typename Eldritch2::ResizableArray<Value, Allocator>::ConstIterator ResizableArray<Value, Allocator>::ConstBegin() const {
		return _underlyingContainer.cbegin();
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	ETInlineHint typename Eldritch2::ResizableArray<Value, Allocator>::ConstIterator ResizableArray<Value, Allocator>::ConstEnd() const {
		return _underlyingContainer.cend();
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	ETInlineHint typename Eldritch2::ResizableArray<Value, Allocator>::ConstReverseIterator Eldritch2::ResizableArray<Value, Allocator>::ConstReverseBegin() const {
		return _underlyingContainer.crbegin();
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	ETInlineHint typename Eldritch2::ResizableArray<Value, Allocator>::ConstReverseIterator Eldritch2::ResizableArray<Value, Allocator>::ConstReverseEnd() const {
		return _underlyingContainer.crend();
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	ETInlineHint typename Eldritch2::ResizableArray<Value, Allocator>::ConstReverseIterator ResizableArray<Value, Allocator>::ReverseBegin() const {
		return _underlyingContainer.rbegin();
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	ETInlineHint typename Eldritch2::ResizableArray<Value, Allocator>::ReverseIterator ResizableArray<Value, Allocator>::ReverseBegin() {
		return _underlyingContainer.rbegin();
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	ETInlineHint typename Eldritch2::ResizableArray<Value, Allocator>::ConstReverseIterator ResizableArray<Value, Allocator>::ReverseEnd() const {
		return _underlyingContainer.rend();
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	ETInlineHint typename Eldritch2::ResizableArray<Value, Allocator>::ReverseIterator ResizableArray<Value, Allocator>::ReverseEnd() {
		return _underlyingContainer.rend();
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	ETInlineHint typename Eldritch2::ResizableArray<Value, Allocator>::ConstIterator ResizableArray<Value, Allocator>::Begin() const {
		return _underlyingContainer.begin();
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	ETInlineHint typename Eldritch2::ResizableArray<Value, Allocator>::Iterator ResizableArray<Value, Allocator>::Begin() {
		return _underlyingContainer.begin();
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	ETInlineHint typename Eldritch2::ResizableArray<Value, Allocator>::ConstIterator ResizableArray<Value, Allocator>::End() const {
		return _underlyingContainer.end();
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	ETInlineHint typename Eldritch2::ResizableArray<Value, Allocator>::Iterator ResizableArray<Value, Allocator>::End() {
		return _underlyingContainer.end();
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	ETInlineHint typename Eldritch2::ResizableArray<Value, Allocator>::ConstReference ResizableArray<Value, Allocator>::Front() const {
		return _underlyingContainer.front();
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	ETInlineHint typename Eldritch2::ResizableArray<Value, Allocator>::Reference ResizableArray<Value, Allocator>::Front() {
		return _underlyingContainer.front();
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	ETInlineHint typename Eldritch2::ResizableArray<Value, Allocator>::ConstReference ResizableArray<Value, Allocator>::Back() const {
		return _underlyingContainer.back();
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	ETInlineHint typename Eldritch2::ResizableArray<Value, Allocator>::Reference ResizableArray<Value, Allocator>::Back() {
		return _underlyingContainer.back();
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	template <class /*SFINAE*/>
	ETInlineHint void ResizableArray<Value, Allocator>::PushBack( const ValueType& itemTemplate ) {
		_underlyingContainer.push_back( itemTemplate );
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	ETInlineHint void ResizableArray<Value, Allocator>::PushBack( ValueType&& item ) {
		_underlyingContainer.push_back( eastl::forward<ValueType>( item ) );
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	template <typename... ElementConstructorArguments>
	ETInlineHint void ResizableArray<Value, Allocator>::EmplaceBack( ElementConstructorArguments&&... elementConstructorArguments ) {
		_underlyingContainer.emplace_back( eastl::forward<ElementConstructorArguments>( elementConstructorArguments )... );
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	ETInlineHint void ResizableArray<Value, Allocator>::PopBack() {
		_underlyingContainer.pop_back();
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	ETInlineHint typename ResizableArray<Value, Allocator>::ConstReference ResizableArray<Value, Allocator>::operator[]( SizeType elementIndex ) const {
		return _underlyingContainer[elementIndex];
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	ETInlineHint typename ResizableArray<Value, Allocator>::Reference ResizableArray<Value, Allocator>::operator[]( SizeType elementIndex ) {
		return _underlyingContainer[elementIndex];
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	ETInlineHint typename ResizableArray<Value, Allocator>::ConstReference ResizableArray<Value, Allocator>::At( SizeType elementIndex ) const {
		return _underlyingContainer.at( elementIndex );
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	ETInlineHint typename ResizableArray<Value, Allocator>::Reference ResizableArray<Value, Allocator>::At( SizeType elementIndex ) {
		return _underlyingContainer.at( elementIndex );
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	ETInlineHint typename ResizableArray<Value, Allocator>::ConstPointer ResizableArray<Value, Allocator>::Data() const {
		return _underlyingContainer.data();
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	ETInlineHint typename ResizableArray<Value, Allocator>::Pointer ResizableArray<Value, Allocator>::Data() {
		return _underlyingContainer.data();
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	ETInlineHint void ResizableArray<Value, Allocator>::Assign( ConstPointer first, ConstPointer last ) {
		_underlyingContainer.assign( first, last );
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	template <class /*SFINAE*/>
	ETInlineHint void ResizableArray<Value, Allocator>::Insert( Iterator position, SizeType itemCount, const ValueType& itemTemplate ) {
		_underlyingContainer.insert( initialIndex, itemCount, itemTemplate );
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	template <class /*SFINAE*/>
	ETInlineHint typename ResizableArray<Value, Allocator>::Iterator ResizableArray<Value, Allocator>::Insert( Iterator position, const ValueType& itemTemplate ) {
		return _underlyingContainer.insert( position, itemTemplate );
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	ETInlineHint typename ResizableArray<Value, Allocator>::Iterator ResizableArray<Value, Allocator>::Insert( Iterator position, ValueType&& itemTemplate ) {
		return _underlyingContainer.insert( position, eastl::forward<ValueType>( itemTemplate ) );
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	template <typename... ElementConstructorArguments>
	ETInlineHint typename Eldritch2::ResizableArray<Value, Allocator>::Iterator ResizableArray<Value, Allocator>::Emplace( Iterator position, ElementConstructorArguments&&... elementConstructorArguments ) {
		return _underlyingContainer.emplace( position, eastl::forward<ElementConstructorArguments>( elementConstructorArguments )... );
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	ETInlineHint typename Eldritch2::ResizableArray<Value, Allocator>::SizeType Eldritch2::ResizableArray<Value, Allocator>::Erase( const ValueType& value ) {
		return _underlyingContainer.erase( value );
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	ETInlineHint typename Eldritch2::ResizableArray<Value, Allocator>::Iterator ResizableArray<Value, Allocator>::Erase( Iterator position ) {
		return _underlyingContainer.erase( position );
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	ETInlineHint void ResizableArray<Value, Allocator>::Erase( Iterator position, const UnorderedSemantics ) {
		_underlyingContainer.erase_unsorted( position );
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	ETInlineHint typename ResizableArray<Value, Allocator>::Iterator ResizableArray<Value, Allocator>::Erase( Iterator first, Iterator last ) {
		return _underlyingContainer.erase( first, last );
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	ETInlineHint void ResizableArray<Value, Allocator>::Clear() {
		_underlyingContainer.clear();
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	ETInlineHint void ResizableArray<Value, Allocator>::Clear( const ReleaseMemorySemantics ) {
		_underlyingContainer.reset_lose_memory();
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	template <class /*SFINAE*/>
	ETInlineHint void ResizableArray<Value, Allocator>::Resize( SizeType sizeInElements, const ValueType& value ) {
		_underlyingContainer.resize( sizeInElements, value );
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	ETInlineHint void ResizableArray<Value, Allocator>::Resize( SizeType sizeInElements ) {
		_underlyingContainer.resize( sizeInElements );
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	ETInlineHint typename Eldritch2::ResizableArray<Value, Allocator>::SizeType ResizableArray<Value, Allocator>::GetSize() const {
		return _underlyingContainer.size();
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	ETInlineHint bool ResizableArray<Value, Allocator>::IsEmpty() const {
		return _underlyingContainer.empty();
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	ETInlineHint ResizableArray<Value, Allocator>::operator bool() const {
		return !_underlyingContainer.empty();
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	ETInlineHint typename Eldritch2::ResizableArray<Value, Allocator>::SizeType ResizableArray<Value, Allocator>::GetCapacity() const {
		return _underlyingContainer.capacity();
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	ETInlineHint void ResizableArray<Value, Allocator>::SetCapacity( SizeType capacityInElements ) {
		_underlyingContainer.set_capacity( capacityInElements );
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	ETInlineHint void ResizableArray<Value, Allocator>::Reserve( SizeType capacityInElementsHint ) {
		_underlyingContainer.reserve( capacityInElementsHint );
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	ETInlineHint typename const Eldritch2::ResizableArray<Value, Allocator>::AllocatorType& ResizableArray<Value, Allocator>::GetAllocator() const {
		return _underlyingContainer.get_allocator();
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	ETInlineHint bool ResizableArray<Value, Allocator>::ValidateIterator( ConstIterator iterator ) const {
		return _underlyingContainer.validate_iterator( iterator );
	}

}	// namespace Eldritch2