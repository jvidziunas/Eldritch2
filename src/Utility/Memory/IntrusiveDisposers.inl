/*==================================================================*\
  IntrusiveDisposers.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Memory/Allocator.hpp>
#include <Utility/Memory/ObjectPool.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Utility {

	template<class T>
	ETInlineHint void DeleteDisposer<T>::operator()( T* const ptr ) const {
		delete ptr;
	}

// ---------------------------------------------------

	template<class T>
	ETInlineHint void DeleteDisposer<T>::operator()( T& object ) const {
		delete &object;
	}

// ---------------------------------------------------

	template<class T>
	ETInlineHint AllocatorDisposer<T>::AllocatorDisposer( ::Eldritch2::Allocator& allocator ) : _allocator( allocator ) {}

// ---------------------------------------------------

	template<class T>
	ETInlineHint void AllocatorDisposer<T>::operator()( T* const ptr ) const {
		_allocator.Delete( *ptr );
	}

// ---------------------------------------------------

	template<class T>
	ETInlineHint void AllocatorDisposer<T>::operator()( T& object ) const {
		_allocator.Delete( object );
	}

// ---------------------------------------------------

	template<class T>
	ETInlineHint AlignedAllocatorDisposer<T>::AlignedAllocatorDisposer( ::Eldritch2::Allocator& allocator ) : _allocator( allocator ) {}

// ---------------------------------------------------

	template<class T>
	ETInlineHint void AlignedAllocatorDisposer<T>::operator()( T* const ptr ) const {
		_allocator.Delete( *ptr, ::Eldritch2::alignedDeallocationSemantics );
	}

// ---------------------------------------------------

	template<class T>
	ETInlineHint void AlignedAllocatorDisposer<T>::operator()( T& object ) const {
		_allocator.Delete( object, ::Eldritch2::alignedDeallocationSemantics );
	}

// ---------------------------------------------------

	template <class T, class ObjectPoolAllocator>
	ETInlineHint PoolDisposer<T, ObjectPoolAllocator>::PoolDisposer( Utility::ObjectPool<ObjectPoolAllocator>& pool ) : _pool( pool ) {}

// ---------------------------------------------------

	template <class T, class ObjectPoolAllocator>
	ETInlineHint void PoolDisposer<T, ObjectPoolAllocator>::operator()( T* const ptr ) const {
		_pool.Deallocate( ( ptr->~T(), ptr ), Utility::orderedPoolBehavior );
	}

// ---------------------------------------------------

	template <class T, class ObjectPoolAllocator>
	ETInlineHint void PoolDisposer<T, ObjectPoolAllocator>::operator()( T& object ) const {
		_pool.Deallocate( ( object.~T(), &object ), Utility::orderedPoolBehavior );
	}

}	// namespace Utility
}	// namespace Eldritch2