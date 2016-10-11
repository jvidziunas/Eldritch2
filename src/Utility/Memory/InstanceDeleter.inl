/*==================================================================*\
  InstanceDeleters.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Memory/Allocator.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename T>
	ETInlineHint InstanceDeleter<T>::InstanceDeleter( Eldritch2::Allocator& allocator ) : _allocatorAndSize( eastl::addressof(allocator), sizeof(T) ) {}

// ---------------------------------------------------

	template <typename T>
	template <typename U, class /*SFINAE*/>
	ETInlineHint InstanceDeleter<T>::InstanceDeleter( const InstanceDeleter<U>& other ) : _allocatorAndSize( eastl::addressof(other.GetAllocator()), other._allocatorAndSize.second() ) {}

// ---------------------------------------------------

	template <typename T>
	ETInlineHint Eldritch2::Allocator& InstanceDeleter<T>::GetAllocator() const {
		return *_allocatorAndSize.first();
	}

// ---------------------------------------------------

	template <typename T>
	ETInlineHint void InstanceDeleter<T>::operator()( Pointer object ) const {
		if( nullptr == object ) {
			return;
		}

		eastl::destruct( object );
		_allocatorAndSize.first()->Deallocate( object, _allocatorAndSize.second() );
	}

// ---------------------------------------------------

	template <typename T>
	template <typename U, class /*SFINAE*/>
	ETInlineHint InstanceDeleter<T>& InstanceDeleter<T>::operator=( const InstanceDeleter<U>& other ) {
		_allocatorAndSize.first()	= eastl::addressof(other.GetAllocator());
		_allocatorAndSize.second()	= sizeof( U );
	}

// ---------------------------------------------------

	template <typename T>
	ETInlineHint InstanceDeleter<T[]>::InstanceDeleter( size_t countInObjects, Eldritch2::Allocator& allocator ) : _countInObjects( countInObjects ), _allocator( &allocator ) {}

// ---------------------------------------------------

	template <typename T>
	ETInlineHint Eldritch2::Allocator& InstanceDeleter<T[]>::GetAllocator() const {
		return *_allocator;
	}

// ---------------------------------------------------

	template <typename T>
	ETInlineHint size_t InstanceDeleter<T[]>::GetSize() const {
		return _countInObjects;
	}

// ---------------------------------------------------

	template <typename T>
	ETInlineHint void InstanceDeleter<T[]>::operator()( Pointer objects ) const {
		if( nullptr == objects ) {
			return;
		}

		eastl::destruct( objects, objects + _countInObjects );

		_allocator->Deallocate( objects, sizeof(T) * _countInObjects );
	}

}	// namespace Eldritch2