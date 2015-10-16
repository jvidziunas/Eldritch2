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
#include <type_traits>
//------------------------------------------------------------------//

namespace Eldritch2 {
	
	template <typename StoredObject, typename Allocator>
	ETInlineHint List<StoredObject, Allocator>::List( AllocatorType&& allocator ) : UnderlyingContainer( ::std::move( allocator ) ) {}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	template <typename InputIterator>
	ETInlineHint List<StoredObject, Allocator>::List( InputIterator first, InputIterator last, AllocatorType&& allocator ) : UnderlyingContainer( first, last, ::std::move( allocator ) ) {}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	template <typename AlternateAllocator>
	ETInlineHint List<StoredObject, Allocator>::List( const ::Eldritch2::List<StoredObject, AlternateAllocator>& listTemplate, AllocatorType&& allocator ) : List<StoredObject, Allocator>( listTemplate.Begin(), listTemplate.End(), ::std::move( allocator ) ) {}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint typename List<StoredObject, Allocator>::Iterator List<StoredObject, Allocator>::Find( ConstReference itemTemplate, Iterator searchHint ) {
		return ::Eldritch2::Utility::Find( searchHint, UnderlyingContainer::end(), itemTemplate, IsEquivalent() );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint typename List<StoredObject, Allocator>::ConstIterator List<StoredObject, Allocator>::Find( ConstReference itemTemplate, ConstIterator searchHint ) const {
		return ::Eldritch2::Utility::Find( searchHint, UnderlyingContainer::end(), itemTemplate, IsEquivalent() );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	template <typename ItemPredicate>
	ETInlineHint typename List<StoredObject, Allocator>::Iterator List<StoredObject, Allocator>::RemoveIf( ItemPredicate itemPredicate ) {
		return ::Eldritch2::Utility::RemoveIf( UnderlyingContainer::begin(), UnderlyingContainer::end(), itemPredicate );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	template <typename ExtraArgumentType, typename ItemPredicate>
	ETInlineHint typename List<StoredObject, Allocator>::Iterator List<StoredObject, Allocator>::RemoveIf( ExtraArgumentType extraArgument, ItemPredicate itemPredicate ) {
		return ::Eldritch2::Utility::RemoveIf( UnderlyingContainer::begin(), UnderlyingContainer::end(), extraArgument, itemPredicate );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint typename List<StoredObject, Allocator>::Iterator List<StoredObject, Allocator>::Begin() {
		return UnderlyingContainer::begin();
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint typename List<StoredObject, Allocator>::ConstIterator List<StoredObject, Allocator>::Begin() const {
		return UnderlyingContainer::begin();
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint typename List<StoredObject, Allocator>::ConstIterator List<StoredObject, Allocator>::ConstBegin() const {
		return UnderlyingContainer::begin();
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint typename List<StoredObject, Allocator>::Iterator List<StoredObject, Allocator>::End() {
		return UnderlyingContainer::end();
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint typename List<StoredObject, Allocator>::ConstIterator List<StoredObject, Allocator>::End() const {
		return UnderlyingContainer::end();
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint typename List<StoredObject, Allocator>::ConstIterator List<StoredObject, Allocator>::ConstEnd() const {
		return UnderlyingContainer::end();
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint typename List<StoredObject, Allocator>::Reference List<StoredObject, Allocator>::Front() {
		return UnderlyingContainer::front();
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint typename List<StoredObject, Allocator>::ConstReference List<StoredObject, Allocator>::Front() const {
		return UnderlyingContainer::front();
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint void List<StoredObject, Allocator>::PushFront( ConstReference value ) {
		UnderlyingContainer::push_front( value );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint void List<StoredObject, Allocator>::PopFront() {
		UnderlyingContainer::pop_front();
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint typename List<StoredObject, Allocator>::Reference List<StoredObject, Allocator>::Back() {
		return UnderlyingContainer::back();
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint typename List<StoredObject, Allocator>::ConstReference List<StoredObject, Allocator>::Back() const {
		return UnderlyingContainer::back();
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint void List<StoredObject, Allocator>::PushBack( ConstReference value ) {
		UnderlyingContainer::push_back( value );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint void List<StoredObject, Allocator>::PopBack() {
		UnderlyingContainer::pop_back();
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint typename List<StoredObject, Allocator>::Iterator List<StoredObject, Allocator>::Insert( Iterator location, ConstReference value ) {
		return UnderlyingContainer::insert( location, value );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint typename List<StoredObject, Allocator>::Iterator List<StoredObject, Allocator>::Erase( Iterator location ) {
		return this->erase( location );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint typename List<StoredObject, Allocator>::Iterator List<StoredObject, Allocator>::Erase( Iterator first, Iterator last ) {
		return UnderlyingContainer::erase( first, last );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	template <typename InputIterator>
	ETInlineHint void List<StoredObject, Allocator>::Assign( InputIterator first, InputIterator last ) {
		UnderlyingContainer::assign( first, last );
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint void List<StoredObject, Allocator>::Clear() {
		UnderlyingContainer::();
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint bool List<StoredObject, Allocator>::Empty() const {
		return UnderlyingContainer::empty();
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint typename List<StoredObject, Allocator>::SizeType List<StoredObject, Allocator>::Size() const {
		return UnderlyingContainer::size();
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint const List<StoredObject, Allocator>::AllocatorType& List<StoredObject, Allocator>::GetAllocator() const {
		return UnderlyingContainer::get_allocator();
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint void List<StoredObject, Allocator>::SetAllocator( const AllocatorType& allocator ) {
		UnderlyingContainer::set_allocator( PrivateAllocator( allocator ) );
	}

}	// namespace Eldritch2

namespace std {

	template <typename StoredObject, typename Allocator>
	ETInlineHint ETNoAliasHint auto begin( ::Eldritch2::List<StoredObject, Allocator>& collection ) -> decltype(collection.Begin()) {
		return collection.Begin();
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint ETNoAliasHint auto begin( const ::Eldritch2::List<StoredObject, Allocator>& collection ) -> decltype(collection.ConstBegin()) {
		return collection.ConstBegin();
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint ETNoAliasHint auto end( ::Eldritch2::List<StoredObject, Allocator>& collection ) -> decltype(collection.End()) {
		return collection.End();
	}

// ---------------------------------------------------

	template <typename StoredObject, typename Allocator>
	ETInlineHint ETNoAliasHint auto end( const ::Eldritch2::List<StoredObject, Allocator>& collection ) -> decltype(collection.ConstEnd()) {
		return collection.ConstEnd();
	}

}	// namespace std