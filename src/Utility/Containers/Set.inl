/*==================================================================*\
  Set.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/Range.hpp>
#include <type_traits>
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename Value, class Allocator>
	ETInlineHint Set<Value, Allocator>::Set( const AllocatorType& allocator ) : _underlyingContainer( allocator ) {}

// ---------------------------------------------------

	template <typename Value, class Allocator>
	ETInlineHint Set<Value, Allocator>::Set( InputIterator begin, InputIterator end, const AllocatorType& allocator ) : _underlyingContainer( allocator ) {
		while( begin != end ) {
			_underlyingContainer.insert( *begin++ );
		}
	}

// ---------------------------------------------------

	template <typename Value, class Allocator>
	template <class /*SFINAE*/>
	ETInlineHint Set<Value, Allocator>::Set( const Set<Value, Allocator>& containerTemplate, const AllocatorType& allocator ) : _underlyingContainer( containerTemplate._underlyingContainer, allocator ) {}

// ---------------------------------------------------

	template <typename Value, class Allocator>
	ETInlineHint Set<Value, Allocator>::Set( Eldritch2::Set<Value, Allocator>&& moveSource ) : _underlyingContainer( eastl::move( moveSource._underlyingContainer ) ) {}

// ---------------------------------------------------

	template <typename Value, class Allocator>
	ETInlineHint typename Set<Value, Allocator>::Iterator Set<Value, Allocator>::Find( const ValueType& itemTemplate ) {
		return _underlyingContainer.find( itemTemplate );
	}

// ---------------------------------------------------

	template <typename Value, class Allocator>
	ETInlineHint typename Set<Value, Allocator>::ConstIterator Set<Value, Allocator>::Find( const ValueType& itemTemplate ) const {
		return _underlyingContainer.find( itemTemplate );
	}

// ---------------------------------------------------

	template <typename Value, class Allocator>
	template <typename Predicate>
	ETInlineHint void Set<Value, Allocator>::RemoveIf( Predicate predicate ) {
		for( Iterator element( _underlyingContainer.begin() ), end( _underlyingContainer.end() ); element != end; ) {
			if( predicate( *element ) ) {
				_underlyingContainer.erase( element++ );
			} else {
				++element;
			}
		}
	}

// ---------------------------------------------------

	template <typename Value, class Allocator>
	template <typename ExtraArgumentType, typename Predicate>
	ETInlineHint void Set<Value, Allocator>::RemoveIf( ExtraArgumentType extraArgument, Predicate predicate ) {
		for( Iterator element( _underlyingContainer.begin() ), end( _underlyingContainer.end() ); element != end; ) {
			if( predicate( *element, extraArgument ) ) {
				_underlyingContainer.erase( element++ );
			} else {
				++element;
			}
		}
	}

// ---------------------------------------------------

	template <typename Value, class Allocator>
	ETInlineHint typename Set<Value, Allocator>::Iterator Set<Value, Allocator>::Begin() {
		return _underlyingContainer.begin();
	}

// ---------------------------------------------------

	template <typename Value, class Allocator>
	ETInlineHint typename Set<Value, Allocator>::ConstIterator Set<Value, Allocator>::Begin() const {
		return _underlyingContainer.begin();
	}

// ---------------------------------------------------

	template <typename Value, class Allocator>
	ETInlineHint typename Set<Value, Allocator>::ConstIterator Set<Value, Allocator>::ConstBegin() const {
		return _underlyingContainer.begin();
	}

// ---------------------------------------------------

	template <typename Value, class Allocator>
	ETInlineHint typename Set<Value, Allocator>::Iterator Set<Value, Allocator>::End() {
		return _underlyingContainer.end();
	}

// ---------------------------------------------------

	template <typename Value, class Allocator>
	ETInlineHint typename Set<Value, Allocator>::ConstIterator Set<Value, Allocator>::End() const {
		return _underlyingContainer.end();
	}

// ---------------------------------------------------

	template <typename Value, class Allocator>
	ETInlineHint typename Set<Value, Allocator>::ConstIterator Set<Value, Allocator>::ConstEnd() const {
		return _underlyingContainer.end();
	}

// ---------------------------------------------------

	template <typename Value, class Allocator>
	template <class /*SFINAE*/>
	ETInlineHint Set<Value, Allocator>& Set<Value, Allocator>::operator=( const Set<Value, Allocator>& other ) {
		_underlyingContainer = other._underlyingContainer;

		return *this;
	}

// ---------------------------------------------------

	template <typename Value, class Allocator>
	ETInlineHint Set<Value, Allocator>& Set<Value, Allocator>::operator=( Set<Value, Allocator>&& other ) {
		_underlyingContainer = eastl::move( other._underlyingContainer );

		return *this;
	}

// ---------------------------------------------------

	template <typename Value, class Allocator>
	ETInlineHint void Set<Value, Allocator>::Swap( Set<Value, Allocator>& other ) {
		_underlyingContainer.swap( other );
	}

// ---------------------------------------------------

	template <typename Value, class Allocator>
	template <class /*SFINAE*/>
	ETInlineHint typename Set<Value, Allocator>::InsertResult Set<Value, Allocator>::Insert( const ValueType& valueTemplate ) {
		return _underlyingContainer.insert( valueTemplate );
	}

// ---------------------------------------------------

	template <typename Value, class Allocator>
	ETInlineHint void Set<Value, Allocator>::Erase( Iterator position ) {
		_underlyingContainer.erase( position );
	}

// ---------------------------------------------------

	template <typename Value, class Allocator>
	ETInlineHint void Set<Value, Allocator>::Erase( Iterator begin, Iterator end ) {
		while( begin != end ) {
			_underlyingContainer.erase( begin++ );
		}
	}

// ---------------------------------------------------

	template <typename Value, class Allocator>
	ETInlineHint void Set<Value, Allocator>::Clear() {
		_underlyingContainer.clear();
	}

// ---------------------------------------------------

	template <typename Value, class Allocator>
	ETInlineHint typename Set<Value, Allocator>::SizeType Set<Value, Allocator>::GetSize() const {
		return _underlyingContainer.size();
	}

// ---------------------------------------------------

	template <typename Value, class Allocator>
	ETInlineHint bool Set<Value, Allocator>::IsEmpty() const {
		return _underlyingContainer.empty();
	}

// ---------------------------------------------------

	template <typename Value, class Allocator>
	ETInlineHint Set<Value, Allocator>::operator bool() const {
		return !_underlyingContainer.empty();
	}

// ---------------------------------------------------

	template <typename Value, class Allocator>
	ETInlineHint typename const Set<Value, Allocator>::AllocatorType& Set<Value, Allocator>::GetAllocator() const {
		return _underlyingContainer.get_allocator();
	}

}	// namespace Eldritch2