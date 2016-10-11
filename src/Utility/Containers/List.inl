/*==================================================================*\
  List.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Algorithms.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	
	template <typename Value, typename Allocator>
	ETInlineHint List<Value, Allocator>::List( const AllocatorType& allocator ) : _underlyingContainer( allocator ) {}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	template <typename InputIterator>
	ETInlineHint List<Value, Allocator>::List( InputIterator first, InputIterator last, const AllocatorType& allocator ) : _underlyingContainer( first, last, allocator ) {}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	template <class /*SFINAE*/>
	ETInlineHint List<Value, Allocator>::List( const List<Value, Allocator>& other, const AllocatorType& allocator ) : _underlyingContainer( other._underlyingContainer, allocator ) {}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	ETInlineHint typename List<Value, Allocator>::ConstIterator List<Value, Allocator>::Find( ConstReference itemTemplate, ConstIterator searchHint ) const {
		return Eldritch2::Utility::Find( searchHint, _underlyingContainer.end(), itemTemplate, IsEquivalent() );
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	ETInlineHint typename List<Value, Allocator>::Iterator List<Value, Allocator>::Find( ConstReference itemTemplate, Iterator searchHint ) {
		return Eldritch2::Utility::Find( searchHint, _underlyingContainer.end(), itemTemplate, IsEquivalent() );
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	template <typename ItemPredicate>
	ETInlineHint typename List<Value, Allocator>::Iterator List<Value, Allocator>::RemoveIf( ItemPredicate itemPredicate ) {
		return Eldritch2::Utility::RemoveIf( _underlyingContainer.begin(), _underlyingContainer.end(), itemPredicate );
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	ETInlineHint typename List<Value, Allocator>::Iterator List<Value, Allocator>::Begin() {
		return _underlyingContainer.begin();
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	ETInlineHint typename List<Value, Allocator>::ConstIterator List<Value, Allocator>::Begin() const {
		return _underlyingContainer.begin();
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	ETInlineHint typename List<Value, Allocator>::ConstIterator List<Value, Allocator>::ConstBegin() const {
		return _underlyingContainer.begin();
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	ETInlineHint typename List<Value, Allocator>::Iterator List<Value, Allocator>::End() {
		return _underlyingContainer.end();
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	ETInlineHint typename List<Value, Allocator>::ConstIterator List<Value, Allocator>::End() const {
		return _underlyingContainer.end();
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	ETInlineHint typename List<Value, Allocator>::ConstIterator List<Value, Allocator>::ConstEnd() const {
		return _underlyingContainer.end();
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	ETInlineHint typename List<Value, Allocator>::Reference List<Value, Allocator>::Front() {
		return _underlyingContainer.front();
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	ETInlineHint typename List<Value, Allocator>::ConstReference List<Value, Allocator>::Front() const {
		return _underlyingContainer.front();
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	template <class /*SFINAE*/>
	ETInlineHint void List<Value, Allocator>::PushFront( ConstReference value ) {
		_underlyingContainer.push_front( value );
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	template <typename... ElementConstructorArguments>
	ETInlineHint void List<Value, Allocator>::EmplaceFront( ElementConstructorArguments&&... elementConstructorArguments ) {
		_underlyingContainer.emplace_front( eastl::forward<ElementConstructorArguments>( elemenetConstructorArguments )... );
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	ETInlineHint void List<Value, Allocator>::PopFront() {
		_underlyingContainer.pop_front();
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	ETInlineHint typename List<Value, Allocator>::Reference List<Value, Allocator>::Back() {
		return _underlyingContainer.back();
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	ETInlineHint typename List<Value, Allocator>::ConstReference List<Value, Allocator>::Back() const {
		return _underlyingContainer.back();
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	template <class /*SFINAE*/>
	ETInlineHint void List<Value, Allocator>::PushBack( ConstReference value ) {
		_underlyingContainer.push_back( value );
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	template <typename... ElementConstructorArguments>
	ETInlineHint void List<Value, Allocator>::EmplaceBack( ElementConstructorArguments&&... elementConstructorArguments ) {
		_underlyingContainer.emplace_back( eastl::forward<ElementConstructorArguments>( elementConstructorArguments )... );
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	ETInlineHint void List<Value, Allocator>::PopBack() {
		_underlyingContainer.pop_back();
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	template <class /*SFINAE*/>
	ETInlineHint typename List<Value, Allocator>::Iterator List<Value, Allocator>::Insert( Iterator location, ConstReference value ) {
		return _underlyingContainer.insert( location, value );
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	template <typename... ElementConstructorArguments>
	ETInlineHint typename List<Value, Allocator>::Iterator List<Value, Allocator>::Emplace( Iterator location, ElementConstructorArguments&&... elementConstructorArguments ) {
		return _underlyingContainer.emplace( location, eastl::forward<ElementConstructorArguments>( elementConstructorArguments )... );
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	ETInlineHint typename List<Value, Allocator>::Iterator List<Value, Allocator>::Erase( Iterator first, Iterator last ) {
		return _underlyingContainer.erase( first, last );
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	ETInlineHint typename List<Value, Allocator>::Iterator List<Value, Allocator>::Erase( Iterator location ) {
		return _underlyingContainer.erase( location );
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	ETInlineHint void List<Value, Allocator>::Clear() {
		_underlyingContainer.clear();
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	template <class /*SFINAE*/>
	ETInlineHint List<Value, Allocator>& List<Value, Allocator>::operator=( const List<Value, Allocator>& other ) {
		_underlyingContainer = other._underlyingContainer;

		return *this;
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	ETInlineHint List<Value, Allocator>& List<Value, Allocator>::operator=( List<Value, Allocator>&& other ) {
		_underlyingContainer = eastl::move( other._underlyingContainer );

		return *this;
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	ETInlineHint void List<Value, Allocator>::Swap( List<Value, Allocator>& other ) {
		_underlyingContainer.swap( other._underlyingContainer );
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	ETInlineHint bool List<Value, Allocator>::IsEmpty() const {
		return _underlyingContainer.empty();
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	ETInlineHint List<Value, Allocator>::operator bool() const {
		return !_underlyingContainer.empty();
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	ETInlineHint typename List<Value, Allocator>::SizeType List<Value, Allocator>::GetSize() const {
		return _underlyingContainer.size();
	}

// ---------------------------------------------------

	template <typename Value, typename Allocator>
	ETInlineHint typename const List<Value, Allocator>::AllocatorType& List<Value, Allocator>::GetAllocator() const {
		return _underlyingContainer.get_allocator();
	}

}	// namespace Eldritch2