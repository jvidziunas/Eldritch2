/*==================================================================*\
  ThreadsafeObjectPool.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Utility {

	template <typename Allocator>
	ThreadsafeObjectPool<Allocator>::ThreadsafeObjectPool( SizeType elementSizeInBytes, SizeType objectsPerPage, const AllocatorType& allocatorTemplate ) : _pool( elementSizeInBytes, objectsPerPage, allocatorTemplate ), _lock( nullptr ) {}

// ---------------------------------------------------

	template <typename Allocator>
	ThreadsafeObjectPool<Allocator>::~ThreadsafeObjectPool() {
		ETRuntimeAssert( !_lock );
	}

// ---------------------------------------------------

	template <typename Allocator>
	bool ThreadsafeObjectPool<Allocator>::AttachLock( Utility::ReaderWriterLock& lock ) {
		if( _lock ) {
			return false;
		}

		_lock = &lock;

		return true;
	}

// ---------------------------------------------------

	template <typename Allocator>
	Utility::ReaderWriterLock* ThreadsafeObjectPool<Allocator>::DetachLock() {
		Utility::ReaderWriterLock*	result( _lock );

		_lock = nullptr;

		return result;
	}

// ---------------------------------------------------

	template <typename Allocator>
	ETRestrictHint void* ThreadsafeObjectPool<Allocator>::Allocate() {
		const ScopedLock	_( _lock );

		return _pool.Allocate();
	}

// ---------------------------------------------------
	
	template <typename Allocator>
	ETRestrictHint void* ThreadsafeObjectPool<Allocator>::Allocate( const Utility::ObjectPoolOrderedHint orderedHint ) {
		const ScopedLock	_( _lock );

		return _pool.Allocate( orderedHint );
	}

// ---------------------------------------------------

	template <typename Allocator>
	ETRestrictHint void* ThreadsafeObjectPool<Allocator>::Allocate( SizeType arraySizeInObjects ) {
		const ScopedLock	_( _lock );

		return _pool.Allocate( arraySizeInObjects );
	}

// ---------------------------------------------------

	template <typename Allocator>
	void ThreadsafeObjectPool<Allocator>::Deallocate( void* const chunk ) {
		const ScopedLock	_( _lock );

		_pool.Deallocate( chunk );
	}

// ---------------------------------------------------

	template <typename Allocator>
	void ThreadsafeObjectPool<Allocator>::Deallocate( void* const chunk, const Utility::ObjectPoolOrderedHint orderedHint ) {
		const ScopedLock	_( _lock );

		_pool.Deallocate( chunk, orderedHint );
	}

// ---------------------------------------------------

	template <typename Allocator>
	void ThreadsafeObjectPool<Allocator>::Deallocate( void* const chunks, SizeType arraySizeInObjects ) {
		const ScopedLock	_( _lock );

		_pool.Deallocate( chunks, arraySizeInObjects );
	}

// ---------------------------------------------------

	template <typename Allocator>
	bool ThreadsafeObjectPool<Allocator>::ReleaseMemory() {
		const ScopedLock	_( _lock );

		return _pool.ReleaseMemory();
	}

// ---------------------------------------------------

	template <typename Allocator>
	bool ThreadsafeObjectPool<Allocator>::PurgeMemory() {
		const ScopedLock	_( _lock );

		return _pool.PurgeMemory();
	}

// ---------------------------------------------------

	template <typename Allocator>
	ETInlineHint typename ThreadsafeObjectPool<Allocator>::SizeType ThreadsafeObjectPool<Allocator>::GetObjectsPerPage() const {
		const ScopedReaderLock	_( _lock );

		return _pool.GetObjectsPerPage();
	}

// ---------------------------------------------------

	template <typename Allocator>
	ETInlineHint void ThreadsafeObjectPool<Allocator>::SetObjectsPerPage( SizeType pageSizeInObjects ) {
		const ScopedLock	_( _lock );

		return _pool.PurgeMemory();
	}

// ---------------------------------------------------

	template <typename Allocator>
	ETInlineHint void ThreadsafeObjectPool<Allocator>::SetObjectSizeInBytes( SizeType newSizeInBytes ) {
		const ScopedLock	_( _lock );

		_pool.SetObjectSizeInBytes( newSizeInBytes );
	}

// ---------------------------------------------------

	template <typename Allocator>
	ETInlineHint typename ThreadsafeObjectPool<Allocator>::SizeType ThreadsafeObjectPool<Allocator>::GetObjectSizeInBytes() const {
		const ScopedReaderLock	_( _lock );

		return _pool.GetObjectSizeInBytes();
	}

// ---------------------------------------------------

	template <typename Allocator>
	ETInlineHint typename ThreadsafeObjectPool<Allocator>::SizeType ThreadsafeObjectPool<Allocator>::GetObjectCountForAllocationSizeInBytes( SizeType sizeInBytes ) const {
		const ScopedReaderLock	_( _lock );

		return _pool.GetObjectCountForAllocationSizeInBytes( sizeInBytes );
	}

// ---------------------------------------------------

	template <typename Allocator>
	ETInlineHint bool ThreadsafeObjectPool<Allocator>::OwnsChunk( void* const chunk ) const {
		const ScopedReaderLock	_( _lock );

		return _pool.OwnsChunk( chunk );
	}

}	// namespace Utility
}	// namespace Eldritch2