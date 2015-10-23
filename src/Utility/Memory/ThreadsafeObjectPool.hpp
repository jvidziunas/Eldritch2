/*==================================================================*\
  ThreadsafeObjectPool.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Memory/ObjectPool.hpp>
#include <Utility/Concurrency/Lock.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Utility {

	template <typename Allocator = Utility::ObjectPool<>::AllocatorType>
	class ThreadsafeObjectPool {
	public:
		typedef typename Utility::ObjectPool<Allocator>::AllocatorType	AllocatorType;
		typedef typename AllocatorType::SizeType						SizeType;
		typedef typename AllocatorType::DifferenceType					DifferenceType;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		// Constructs this ThreadsafeObjectPool instance.
		ThreadsafeObjectPool( SizeType elementSizeInBytes, SizeType objectsPerPage, const AllocatorType& allocatorTemplate );

		// Destroys this ThreadsafeObjectPool instance.
		~ThreadsafeObjectPool();

	// ---------------------------------------------------

		bool						AttachLock( Utility::ReaderWriterLock& lock );

		Utility::ReaderWriterLock*	DetachLock();

	// ---------------------------------------------------

		// Allocates an object from the pool, requesting more memory from the backing allocator as appropriate.
		ETRestrictHint void*	Allocate();
		// Allocates an object from the pool, requesting more memory from the backing allocator as appropriate.
		// This overload of Allocate() should be preferred when the pool will also be servicing array allocation requests.
		ETRestrictHint void*	Allocate( const Utility::ObjectPoolOrderedHint orderedHint );
		// Allocates a contiguous array of objects from this pool, requesting more memory from the backing allocator as appropriate.
		ETRestrictHint void*	Allocate( SizeType arraySizeInObjects );

	// ---------------------------------------------------

		// Deallocates a previously-allocated block of memory from this pool.
		void	Deallocate( void* const chunk );
		// Deallocates a previously-allocated block of memory from this pool, and adds it back into the memory store with ordered semantics.
		// This overload of Deallocate() should be preferred when the pool will also be servicing array allocation requests.
		void	Deallocate( void* const chunk, const Utility::ObjectPoolOrderedHint orderedHint );
		// Deallocates a contiguous array of objects from this pool, adding them back into the memory store with ordered semantics.
		void	Deallocate( void* const chunks, SizeType arraySizeInObjects );

	// ---------------------------------------------------

		bool	ReleaseMemory();

		bool	PurgeMemory();

	// ---------------------------------------------------

		ETInlineHint SizeType	GetObjectsPerPage() const;

		ETInlineHint void		SetObjectsPerPage( SizeType pageSizeInObjects );

	// ---------------------------------------------------

		ETInlineHint void		SetObjectSizeInBytes( SizeType newSizeInBytes );

		ETInlineHint SizeType	GetObjectSizeInBytes() const;

		ETInlineHint SizeType	GetObjectCountForAllocationSizeInBytes( SizeType sizeInBytes ) const;

	// ---------------------------------------------------

		ETInlineHint bool	OwnsChunk( void* const chunk ) const;

	// ---------------------------------------------------

	private:
		Utility::ObjectPool<Allocator>	_pool;
		Utility::ReaderWriterLock*		_lock;
	};

}	// namespace Utility
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/Memory/ThreadsafeObjectPool.inl>
//------------------------------------------------------------------//