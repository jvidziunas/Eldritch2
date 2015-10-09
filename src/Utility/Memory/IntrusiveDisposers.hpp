/*==================================================================*\
  IntrusiveDisposers.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/Compiler.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Utility {
		template <typename Allocator>
		class	ObjectPool;
	}

	class	Allocator;
}

namespace Eldritch2 {
namespace Utility {

	namespace Utility	= ::Eldritch2::Utility;

// ---------------------------------------------------

	template <class T>
	class DeleteDisposer {
	public:
		ETInlineHint void operator()( T* const ptr ) const;
		ETInlineHint void operator()( T& object ) const;

	// ---------------------------------------------------

	private:
		// Disallow assignment.
		Utility::DeleteDisposer<T>&	operator=( const Utility::DeleteDisposer<T>& other );		
	};

// ---------------------------------------------------

	template <class T>
	class AllocatorDisposer {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		// Constructs this AllocatorDisposer instance.
		ETInlineHint	AllocatorDisposer( ::Eldritch2::Allocator& allocator );

	// ---------------------------------------------------

		ETInlineHint void	operator()( T* const ptr ) const;
		ETInlineHint void	operator()( T& object ) const;

	// ---------------------------------------------------

	private:
		// Disallow assignment.
		Utility::AllocatorDisposer<T>&	operator=( const Utility::AllocatorDisposer<T>& other );

	// - DATA MEMBERS ------------------------------------

		::Eldritch2::Allocator&	_allocator;
	};

// ---------------------------------------------------

	template <class T>
	class AlignedAllocatorDisposer {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		// Constructs this AlignedAllocatorDisposer instance.
		ETInlineHint	AlignedAllocatorDisposer( ::Eldritch2::Allocator& allocator );

	// ---------------------------------------------------

		ETInlineHint void	operator()( T* const ptr ) const;
		ETInlineHint void	operator()( T& object ) const;

	// ---------------------------------------------------

	private:
		// Disallow assignment.
		Utility::AlignedAllocatorDisposer<T>&	operator=( const Utility::AlignedAllocatorDisposer<T>& other );

	// - DATA MEMBERS ------------------------------------

		::Eldritch2::Allocator&	_allocator;
	};

// ---------------------------------------------------

	template <class T, class ObjectPoolAllocator>
	class PoolDisposer {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		// Constructs this PoolDisposer instance.
		ETInlineHint	PoolDisposer( Utility::ObjectPool<ObjectPoolAllocator>& pool );

	// ---------------------------------------------------

		ETInlineHint void	operator()( T* const ptr ) const;
		ETInlineHint void	operator()( T& object ) const;

	// ---------------------------------------------------

	private:
		// Disallow assignment.
		Utility::PoolDisposer<T, ObjectPoolAllocator>&	operator=( const Utility::PoolDisposer<T, ObjectPoolAllocator>& other );

	// - DATA MEMBERS ------------------------------------

		Utility::ObjectPool<ObjectPoolAllocator>&	_pool;
	};

}	// namespace Utility
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/Memory/IntrusiveDisposers.inl>
//------------------------------------------------------------------//
