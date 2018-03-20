/*==================================================================*\
  Dequeue.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename Value, class Allocator, unsigned int granularity>
	ETInlineHint Dequeue<Value, Allocator, granularity>::Dequeue( const Dequeue<Value, Allocator, granularity>& dequeue, const AllocatorType& allocator ) : _container( dequeue, allocator ) {}

// ---------------------------------------------------

	template <typename Value, class Allocator, unsigned int granularity>
	ETInlineHint Dequeue<Value, Allocator, granularity>::Dequeue( const AllocatorType& allocator ) : _container( allocator ) {}

// ---------------------------------------------------

	template <typename Value, class Allocator, unsigned int granularity>
	ETInlineHint Dequeue<Value, Allocator, granularity>::Dequeue( Dequeue<Value, Allocator, granularity>&& dequeue ) : _container( eastl::move( dequeue._container ) ) {}

// ---------------------------------------------------

	template <typename Value, class Allocator, unsigned int granularity>
	ETInlineHint typename Dequeue<Value, Allocator, granularity>::ConstReference Dequeue<Value, Allocator, granularity>::Front() const {
		return _container.front();
	}

// ---------------------------------------------------

	template <typename Value, class Allocator, unsigned int granularity>
	ETInlineHint typename Dequeue<Value, Allocator, granularity>::Reference Dequeue<Value, Allocator, granularity>::Front() {
		return _container.front();
	}

// ---------------------------------------------------

	template <typename Value, class Allocator, unsigned int granularity>
	ETInlineHint typename Dequeue<Value, Allocator, granularity>::ConstReference Dequeue<Value, Allocator, granularity>::Back() const {
		return _container.back();
	}

// ---------------------------------------------------

	template <typename Value, class Allocator, unsigned int granularity>
	ETInlineHint typename Dequeue<Value, Allocator, granularity>::Reference Dequeue<Value, Allocator, granularity>::Back() {
		return _container.back();
	}

// ---------------------------------------------------

	template <typename Value, class Allocator, unsigned int granularity>
	ETInlineHint void Dequeue<Value, Allocator, granularity>::Prepend( const ValueType& value ) {
		_container.push_front( value );
	}

// ---------------------------------------------------

	template <typename Value, class Allocator, unsigned int granularity>
	ETInlineHint void Dequeue<Value, Allocator, granularity>::Prepend( ValueType&& value ) {
		return _container.push_front( eastl::forward<ValueType>( value ) );
	}

// ---------------------------------------------------

	template <typename Value, class Allocator, unsigned int granularity>
	ETInlineHint typename Dequeue<Value, Allocator, granularity>::Reference Dequeue<Value, Allocator, granularity>::Prepend() {
		return _container.push_front();
	}

// ---------------------------------------------------

	template <typename Value, class Allocator, unsigned int granularity>
	ETInlineHint void Dequeue<Value, Allocator, granularity>::Append( const ValueType& value ) {
		_container.push_back( value );
	}

// ---------------------------------------------------

	template <typename Value, class Allocator, unsigned int granularity>
	ETInlineHint void Dequeue<Value, Allocator, granularity>::Append( ValueType&& value ) {
		_container.push_back( eastl::forward<ValueType>( value ) );
	}

// ---------------------------------------------------

	template <typename Value, class Allocator, unsigned int granularity>
	ETInlineHint typename Dequeue<Value, Allocator, granularity>::Reference Dequeue<Value, Allocator, granularity>::Append() {
		return _container.push_back();
	}

// ---------------------------------------------------

	template <typename Value, class Allocator, unsigned int granularity>
	ETInlineHint void Dequeue<Value, Allocator, granularity>::PopFront() {
		_container.pop_front();
	}

// ---------------------------------------------------

	template <typename Value, class Allocator, unsigned int granularity>
	ETInlineHint void Dequeue<Value, Allocator, granularity>::Pop() {
		_container.pop_back();
	}

// ---------------------------------------------------

	template <typename Value, class Allocator, unsigned int granularity>
	template <typename InputIterator>
	ETInlineHint void Dequeue<Value, Allocator, granularity>::Assign( InputIterator first, InputIterator last ) {
		_container.assign( first, last );
	}

// ---------------------------------------------------

	template <typename Value, class Allocator, unsigned int granularity>
	ETInlineHint void Dequeue<Value, Allocator, granularity>::Assign( std::initializer_list<ValueType> values ) {
		_container.assign( values );
	}

// ---------------------------------------------------

	template <typename Value, class Allocator, unsigned int granularity>
	ETInlineHint void Dequeue<Value, Allocator, granularity>::Assign( SizeType n, const ValueType& value ) {
		_container.assign( n, value );
	}

// ---------------------------------------------------

	template <typename Value, class Allocator, unsigned int granularity>
	ETInlineHint typename Dequeue<Value, Allocator, granularity>::ConstReverseIterator Dequeue<Value, Allocator, granularity>::ConstReverseBegin() const {
		return _container.crbegin();
	}

// ---------------------------------------------------

	template <typename Value, class Allocator, unsigned int granularity>
	ETInlineHint typename Dequeue<Value, Allocator, granularity>::ConstReverseIterator Dequeue<Value, Allocator, granularity>::ConstReverseEnd() const {
		return _container.crend();
	}

// ---------------------------------------------------

	template <typename Value, class Allocator, unsigned int granularity>
	ETInlineHint typename Dequeue<Value, Allocator, granularity>::ConstReverseIterator Dequeue<Value, Allocator, granularity>::ReverseBegin() const {
		return _container.rbegin();
	}

// ---------------------------------------------------

	template <typename Value, class Allocator, unsigned int granularity>
	ETInlineHint typename Dequeue<Value, Allocator, granularity>::ReverseIterator Dequeue<Value, Allocator, granularity>::ReverseBegin() {
		return _container.rbegin();
	}

// ---------------------------------------------------

	template <typename Value, class Allocator, unsigned int granularity>
	ETInlineHint typename Dequeue<Value, Allocator, granularity>::ConstReverseIterator Dequeue<Value, Allocator, granularity>::ReverseEnd() const {
		return _container.rend();
	}

// ---------------------------------------------------

	template <typename Value, class Allocator, unsigned int granularity>
	ETInlineHint typename Dequeue<Value, Allocator, granularity>::ReverseIterator Dequeue<Value, Allocator, granularity>::ReverseEnd() {
		return _container.rend();
	}

// ---------------------------------------------------

	template <typename Value, class Allocator, unsigned int granularity>
	ETInlineHint typename Dequeue<Value, Allocator, granularity>::ConstIterator Dequeue<Value, Allocator, granularity>::ConstBegin() const {
		return _container.cbegin();
	}

// ---------------------------------------------------

	template <typename Value, class Allocator, unsigned int granularity>
	ETInlineHint typename Dequeue<Value, Allocator, granularity>::ConstIterator Dequeue<Value, Allocator, granularity>::ConstEnd() const {
		return _container.cend();
	}

// ---------------------------------------------------

	template <typename Value, class Allocator, unsigned int granularity>
	ETInlineHint typename Dequeue<Value, Allocator, granularity>::ConstIterator Dequeue<Value, Allocator, granularity>::Begin() const {
		return _container.begin();
	}

// ---------------------------------------------------

	template <typename Value, class Allocator, unsigned int granularity>
	ETInlineHint typename Dequeue<Value, Allocator, granularity>::Iterator Dequeue<Value, Allocator, granularity>::Begin() {
		return _container.begin();
	}

// ---------------------------------------------------

	template <typename Value, class Allocator, unsigned int granularity>
	ETInlineHint typename Dequeue<Value, Allocator, granularity>::ConstIterator Dequeue<Value, Allocator, granularity>::End() const {
		return _container.end();
	}

// ---------------------------------------------------

	template <typename Value, class Allocator, unsigned int granularity>
	ETInlineHint typename Dequeue<Value, Allocator, granularity>::Iterator Dequeue<Value, Allocator, granularity>::End() {
		return _container.end();
	}

// ---------------------------------------------------

	template <typename Value, class Allocator, unsigned int granularity>
	ETInlineHint void Dequeue<Value, Allocator, granularity>::Resize( SizeType n, const ValueType& value ) {
		_container.resize( n, value );
	}

// ---------------------------------------------------

	template <typename Value, class Allocator, unsigned int granularity>
	ETInlineHint void Dequeue<Value, Allocator, granularity>::Resize( SizeType n ) {
		_container.resize( n );
	}

// ---------------------------------------------------

	template <typename Value, class Allocator, unsigned int granularity>
	ETInlineHint void Dequeue<Value, Allocator, granularity>::SetCapacity( SizeType n ) {
		_container.set_capacity( n );
	}

// ---------------------------------------------------

	template <typename Value, class Allocator, unsigned int granularity>
	ETInlineHint void Dequeue<Value, Allocator, granularity>::ShrinkToFit() {
		_container.shrink_to_fit();
	}

// ---------------------------------------------------

	template <typename Value, class Allocator, unsigned int granularity>
	ETInlineHint typename Dequeue<Value, Allocator, granularity>::ConstReference Dequeue<Value, Allocator, granularity>::operator[]( SizeType index ) const {
		return _container[index];
	}

// ---------------------------------------------------

	template <typename Value, class Allocator, unsigned int granularity>
	ETInlineHint typename Dequeue<Value, Allocator, granularity>::Reference Dequeue<Value, Allocator, granularity>::operator[]( SizeType index ) {
		return _container[index];
	}

// ---------------------------------------------------

	template <typename Value, class Allocator, unsigned int granularity>
	ETInlineHint typename Dequeue<Value, Allocator, granularity>::ConstReference Dequeue<Value, Allocator, granularity>::At( SizeType index ) const {
		return _container.at( index );
	}

// ---------------------------------------------------

	template <typename Value, class Allocator, unsigned int granularity>
	ETInlineHint typename Dequeue<Value, Allocator, granularity>::Reference Dequeue<Value, Allocator, granularity>::At( SizeType index ) {
		return _container.at( index );
	}

// ---------------------------------------------------

	template <typename Value, class Allocator, unsigned int granularity>
	template <class... Args>
	ETInlineHint typename Dequeue<Value, Allocator, granularity>::Iterator Dequeue<Value, Allocator, granularity>::Emplace( ConstIterator position, Args&&... args ) {
		return _container.emplace( position, eastl::forward<Args>( args )... );
	}

// ---------------------------------------------------

	template <typename Value, class Allocator, unsigned int granularity>
	template <class... Args>
	ETInlineHint void Dequeue<Value, Allocator, granularity>::EmplaceFront( Args&&... args ) {
		_container.emplace_front( eastl::forward<Args>( args )... );
	}

// ---------------------------------------------------

	template <typename Value, class Allocator, unsigned int granularity>
	template <class... Args>
	ETInlineHint void Dequeue<Value, Allocator, granularity>::EmplaceBack( Args&&... args ) {
		_container.emplace_back( eastl::forward<Args>( args )... );
	}

// ---------------------------------------------------

	template <typename Value, class Allocator, unsigned int granularity>
	template <typename InputIterator>
	ETInlineHint void Dequeue<Value, Allocator, granularity>::Insert( ConstIterator position, InputIterator first, InputIterator last ) {
		_container.insert( position, first, last );
	}

// ---------------------------------------------------

	template <typename Value, class Allocator, unsigned int granularity>
	ETInlineHint typename Dequeue<Value, Allocator, granularity>::Iterator Dequeue<Value, Allocator, granularity>::Insert( ConstIterator position, std::initializer_list<ValueType> values ) {
		return _container.insert( position, values );
	}

// ---------------------------------------------------

	template <typename Value, class Allocator, unsigned int granularity>
	ETInlineHint void Dequeue<Value, Allocator, granularity>::Insert( ConstIterator position, SizeType n, const ValueType& value ) {
		_container.insert( position, n, value );
	}

// ---------------------------------------------------

	template <typename Value, class Allocator, unsigned int granularity>
	ETInlineHint typename Dequeue<Value, Allocator, granularity>::Iterator Dequeue<Value, Allocator, granularity>::Insert( ConstIterator position, const ValueType& value ) {
		return _container.insert( position, value );
	}

// ---------------------------------------------------

	template <typename Value, class Allocator, unsigned int granularity>
	ETInlineHint typename Dequeue<Value, Allocator, granularity>::Iterator Dequeue<Value, Allocator, granularity>::Insert( ConstIterator position, ValueType&& value ) {
		return _container.insert( position, eastl::forward<ValueType>( value ) );
	}

// ---------------------------------------------------

	template <typename Value, class Allocator, unsigned int granularity>
	ETInlineHint typename Dequeue<Value, Allocator, granularity>::ReverseIterator Dequeue<Value, Allocator, granularity>::Erase( ReverseIterator first, ReverseIterator last ) {
		return _container.erase( first, last );
	}

// ---------------------------------------------------

	template <typename Value, class Allocator, unsigned int granularity>
	ETInlineHint typename Dequeue<Value, Allocator, granularity>::ReverseIterator Dequeue<Value, Allocator, granularity>::Erase( ReverseIterator position ) {
		return _container.erase( position );
	}

// ---------------------------------------------------

	template <typename Value, class Allocator, unsigned int granularity>
	ETInlineHint typename Dequeue<Value, Allocator, granularity>::Iterator Dequeue<Value, Allocator, granularity>::Erase( ConstIterator first, ConstIterator last ) {
		return _container.erase( first, last );
	}

// ---------------------------------------------------

	template <typename Value, class Allocator, unsigned int granularity>
	ETInlineHint typename Dequeue<Value, Allocator, granularity>::Iterator Dequeue<Value, Allocator, granularity>::Erase( ConstIterator position ) {
		return _container.erase( position );
	}

// ---------------------------------------------------

	template <typename Value, class Allocator, unsigned int granularity>
	ETInlineHint void Dequeue<Value, Allocator, granularity>::Clear() {
		_container.clear();
	}

// ---------------------------------------------------

	template <typename Value, class Allocator, unsigned int granularity>
	ETInlineHint bool Dequeue<Value, Allocator, granularity>::IsEmpty() const {
		return _container.empty();
	}

// ---------------------------------------------------

	template <typename Value, class Allocator, unsigned int granularity>
	ETInlineHint typename Dequeue<Value, Allocator, granularity>::SizeType Dequeue<Value, Allocator, granularity>::GetSize() const {
		return _container.size();
	}

// ---------------------------------------------------

	template <typename Value, class Allocator, unsigned int granularity>
	ETInlineHint typename const Dequeue<Value, Allocator, granularity>::AllocatorType& Dequeue<Value, Allocator, granularity>::GetAllocator() const {
		return _container.get_allocator();
	}

// ---------------------------------------------------

	template <typename Value, class Allocator, unsigned int granularity>
	ETInlineHint void Swap( Dequeue<Value, Allocator, granularity>& dequeue0, Dequeue<Value, Allocator, granularity>& dequeue1 ) {
		eastl::swap( dequeue0._container, dequeue1._container );
	}

}	// namespace Eldritch2

