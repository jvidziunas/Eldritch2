/*==================================================================*\
  ObjectPool.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2012 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <boost/math/common_factor_rt.hpp>
#include <Utility/Math/MathUtils.hpp>
#include <functional>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Utility {

	namespace Utility	= ::Eldritch2::Utility;

// ---------------------------------------------------

	template <typename Allocator>
	ETInlineHint bool ObjectPool<Allocator>::IsFrom( void* const chunk, char* const i, SizeType sizeof_i ) {
		return( ::std:less_equal<void*>()( i, chunk ) && ::std::less<void*>()( chunk, i + sizeof_i ) );
	}

// ---------------------------------------------------

	template <typename Allocator>
	template <typename... AllocatorConstructorArguments>
	ObjectPool<Allocator>::ObjectPool( SizeType elementSizeInBytes, SizeType objectsPerPage, AllocatorConstructorArguments... allocatorConstructorArguments ) : _allocator( allocatorConstructorArguments... ),
																																								_list( 0, 0 ),
																																								_elementSizeInBytes( elementSizeInBytes ),
																																								_objectsPerPage( objectsPerPage ),
																																								_startSize( objectsPerPage ) {}

// ---------------------------------------------------

	template <typename Allocator>
	ObjectPool<Allocator>::~ObjectPool() {
		PurgeMemory();
	}

// ---------------------------------------------------

	template <typename Allocator>
	ETRestrictHint void* ObjectPool<Allocator>::Allocate() {
		// Look for a non-empty storage
		if( ETBranchLikelyHint( !Store().empty() ) ) {
			return Store().malloc();
		}

		return AllocNeedResize();
    }

// ---------------------------------------------------

	template <typename Allocator>
	ETRestrictHint void* ObjectPool<Allocator>::Allocate( const ObjectPoolOrderedHint& orderedHint ) {
		ETUnreferencedParameter( orderedHint );

		if( !Store().empty() ) {
			return Store().malloc();
		}

		return OrderedAllocNeedResize();
    }

// ---------------------------------------------------

	template <typename Allocator>
	ETRestrictHint void* ObjectPool<Allocator>::Allocate( SizeType arraySize ) {
		if( void* const ret = Store().malloc_n( arraySize, GetObjectSizeInBytes() ) ) {
			return ret;
		}

		// Not enough memory in our storage; allocate more

		_objectsPerPage	= ::Eldritch2::Max( GetObjectsPerPage(), arraySize );
		
		char* const	ptr( static_cast<char*>( _allocator.Allocate( GetObjectsPerPage() * GetObjectSizeInBytes() + MIN_ALLOC_SIZE + sizeof( SizeType ) ) ) );

		if( !ptr ) {
			return nullptr;
		}
		
		const ::boost::details::PODptr<SizeType>	node( ptr, _allocator.GetAllocationSizeInBytes( ptr ) );

		// Split up block so we can use what wasn't requested
		//  (we can use "add_block" here because we know that
		//  the free list is empty, so we don't have to use
		//  the slower ordered version)
		if( _objectsPerPage > arraySize ) {
			Store().add_block( node.begin() + arraySize * GetObjectSizeInBytes(), node.element_size() - arraySize * GetObjectSizeInBytes(), GetObjectSizeInBytes() );
		}

		_objectsPerPage <<= 1;

		// insert it into the list,
		// handle border case
		if( !_list.valid() || greater<void*>()( _list.begin(), node.begin() ) ) {
			node.next( _list );
			_list = node;
		} else {
			::boost::details::PODptr<SizeType>	prev( _list );

			for( ; ; prev = prev.next() ) {
				// if we're about to hit the end or
				//  if we've found where "node" goes
				if( nullptr == prev.next_ptr() || greater<void*>()( prev.next_ptr(), node.begin() ) )
					break;
			}

			node.next( prev.next() );
			prev.next( node );
		}

		//  and return it.
		return node.begin();
	}

// ---------------------------------------------------

	template <typename Allocator>
	void ObjectPool<Allocator>::Deallocate( void* const chunk ) {
		Store().free( chunk );
	}

// ---------------------------------------------------

	template <typename Allocator>
	void ObjectPool<Allocator>::Deallocate( void* const chunk, const ObjectPoolOrderedHint& orderedHint ) {
		ETUnreferencedParameter( orderedHint );

		Store().ordered_free( chunk );
	}

// ---------------------------------------------------

	template <typename Allocator>
	void ObjectPool<Allocator>::Deallocate( void* const chunks, SizeType arraySizeInObjects ) {
    	Store().ordered_free_n( chunks, arraySizeInObjects, GetObjectSizeInBytes() );
    }

// ---------------------------------------------------

	template <typename Allocator>
	bool ObjectPool<Allocator>::ReleaseMemory() {
		// This is the return value: it will be set to true when we actually call
		//  UserAllocator::free(..)
		bool				ret( false );

		// This is a current & previous iterator pair over the memory block list
		::boost::details::PODptr<SizeType>	ptr( _list );
		::boost::details::PODptr<SizeType>	prev;

		// This is a current & previous iterator pair over the free memory chunk list
		//  Note that "prev_free" in this case does NOT point to the previous memory
		//  chunk in the free list, but rather the last free memory chunk before the
		//  current block.
		const SizeType	partitionSize( GetObjectSizeInBytes() );
		void*			previousFreePtr( nullptr );
		void*			freePtr( first );

		// Search through all the all the allocated memory blocks
		while( ptr.valid() ) {
			// At this point:
			//  ptr points to a valid memory block
			//  freePtr points to either:
			//    0 if there are no more free chunks
			//    the first free chunk in this or some next memory block
			//  previousFreePtr points to either:
			//    the last free chunk in some previous memory block
			//    0 if there is no such free chunk
			//  prev is either:
			//    the ::boost::details::PODptr whose next() is ptr
			//    !valid() if there is no such ::boost::details::PODptr

			// If there are no more free memory chunks, then every remaining
			//  block is allocated out to its fullest capacity, and we can't
			//  release any more memory
			if( !freePtr ) {
				break;
			}

			// We have to check all the chunks.  If they are *all* free (i.e., present
			//  in the free list), then we can free the block.
			bool	allChunksFree( true );

			// Iterate 'i' through all chunks in the memory block
			// if free starts in the memory block, be careful to keep it there
			void*	savedFree( freePtr );

			for( char* i( ptr.begin() ); i != ptr.end(); i += partitionSize ) {
				// If this chunk is not free
				if( i != freePtr ) {
					// We won't be able to free this block
					allChunksFree	= false;

					// freePtr might have travelled outside ptr
					freePtr			= savedFree;
					// Abort searching the chunks; we won't be able to free this
					//  block because a chunk is not free.
					break;
				}

				// We do not increment previousFreePtr because we are in the same block
				freePtr = NextOf( freePtr );
			}

			// post: if the memory block has any chunks, freePtr points to one of them
			// otherwise, our assertions above are still valid

			const ::boost::details::PODptr<SizeType>	next( ptr.next() );

			if( !allChunksFree ) {
				if( IsFrom( freePtr, ptr.begin(), ptr.element_size() ) ) {
					void* const	end( ptr.end() );

					do {
						previousFreePtr	= freePtr;
						freePtr			= NextOf( freePtr );
					} while( freePtr && ::std::less<void*>()( freePtr, end ) );
				}
				// This invariant is now restored:
				//     freePtr points to the first free chunk in some next memory block, or
				//       0 if there is no such chunk.
				//     previousFreePtr points to the last free chunk in this memory block.
				  
				// We are just about to advance ptr.  Maintain the invariant:
				// prev is the ::boost::details::PODptr whose next() is ptr, or !valid()
				// if there is no such ::boost::details::PODptr
				prev = ptr;
			} else {
				// All chunks from this block are free

				// Remove block from list
				if( prev.valid() ) {
					prev.next( next );
				} else {
					_list = next;
				}

				// Remove all entries in the free list from this block
				if( previousFreePtr ) {
					NextOf( previousFreePtr ) = freePtr;
				} else {
					first = freePtr;
				}

				// And release memory
				_allocator.Deallocate( ptr.begin() );
				ret = true;
			}

			// Increment ptr
			ptr = next;
		}

		_objectsPerPage = _startSize;
		return ret;
	}

// ---------------------------------------------------

	template <typename Allocator>
	bool ObjectPool<Allocator>::PurgeMemory() {
		::boost::details::PODptr<SizeType>	iter( _list );

		if( !iter.valid() ) {
			return false;
		}

		do {
			// hold "next" pointer
			const ::boost::details::PODptr<SizeType>	next( iter.next() );

			// delete the storage
			_allocator.Deallocate( iter.begin() );

			// increment iter
			iter = next;
		} while( iter.valid() );

		_list.invalidate();

		first			= nullptr;
		_objectsPerPage	= _startSize;

		return true;
	}

// ---------------------------------------------------

	template <typename Allocator>
	ETRestrictHint void* ObjectPool<Allocator>::AllocNeedResize() {
		// No memory in any of our storages; make a new storage,
		const SizeType	allocationSize( GetObjectsPerPage() * GetObjectSizeInBytes() + MIN_ALLOC_SIZE + sizeof( SizeType ) );
		char* const		ptr( static_cast<char*>( _allocator.Allocate( allocationSize ) ) );

		if( ETBranchUnlikelyHint( !ptr ) ) {
			return nullptr;
		}

		const ::boost::details::PODptr<SizeType>	node( ptr, allocationSize );
		_objectsPerPage <<= 1;

		//  initialize it,
		Store().add_block( node.begin(), node.element_size(), GetObjectSizeInBytes() );

		//  insert it into the list,
		node.next( _list );
		_list = node;

		//  and return a chunk from it.
		return Store().malloc();
	}

// ---------------------------------------------------

	template <typename Allocator>
	ETRestrictHint void* ObjectPool<Allocator>::OrderedAllocNeedResize() {
		// No memory in any of our storages; make a new storage,
		char* const	ptr( static_cast<char*>( _allocator.Allocate( GetObjectsPerPage() * GetObjectSizeInBytes() + MIN_ALLOC_SIZE + sizeof( SizeType ) ) ) );

		if( ETBranchUnlikelyHint( nullptr == ptr ) )
			return nullptr;

		const ::boost::details::PODptr<SizeType>	node( ptr, _allocator.GetAllocationSizeInBytes( ptr ) );
		_objectsPerPage <<= 1;

		//  initialize it,
		//  (we can use "add_block" here because we know that
		//  the free list is empty, so we don't have to use
		//  the slower ordered version)
		Store().add_block( node.begin(), node.element_size(), GetObjectSizeInBytes() );

		//  insert it into the list,
		//   handle border case
		if( !_list.valid() || greater<void*>()( _list.begin(), node.begin() ) ) {
			node.next( _list );
			_list = node;
		} else {
			::boost::details::PODptr<SizeType>	prev( _list );

			for( ; ; prev = prev.next() ) {
				// if we're about to hit the end or
				//  if we've found where "node" goes
				if( nullptr == prev.next_ptr() || greater<void*>()( prev.next_ptr(), node.begin() ) )
					break;
			}

			node.next( prev.next() );
			prev.next( node );
		}

		//  and return a chunk from it.
		return Store().malloc();
	}

// ---------------------------------------------------

	template <typename Allocator>
	::boost::details::PODptr<typename Utility::ObjectPool<Allocator>::SizeType> ObjectPool<Allocator>::FindPOD( void* const chunk ) const {
		// We have to find which storage this chunk is from.
		::boost::details::PODptr<SizeType>	iter( _list );

		for( ; iter.valid(); iter = iter.next() ) {
			if( IsFrom( chunk, iter.begin(), iter.element_size() ) ) {
				return iter;
			}
		}

		return iter;
	}

// ---------------------------------------------------

	template <typename Allocator>
	ETInlineHint typename ObjectPool<Allocator>::SegregatedStorageType& ObjectPool<Allocator>::Store() {
		return( *this );
	}

// ---------------------------------------------------

	template <typename Allocator>
	ETInlineHint typename const ObjectPool<Allocator>::SegregatedStorageType& ObjectPool<Allocator>::Store() const {
		return( *this );
	}

// ---------------------------------------------------

	template <typename Allocator>
	ETInlineHint void*& ObjectPool<Allocator>::NextOf( void* const ptr ) {
		return *( static_cast<void**>( ptr ) );
	}

// ---------------------------------------------------

	template <typename Allocator>
	ETInlineHint typename ObjectPool<Allocator>::SizeType ObjectPool<Allocator>::GetObjectsPerPage() const {
		return _objectsPerPage;
	}

// ---------------------------------------------------

	template <typename Allocator>
	ETInlineHint void ObjectPool<Allocator>::SetObjectsPerPage( SizeType pageSizeInObjects ) {
		_objectsPerPage = _startSize = pageSizeInObjects;
	}

// ---------------------------------------------------

	template <typename Allocator>
	ETInlineHint typename ObjectPool<Allocator>::SizeType ObjectPool<Allocator>::GetObjectSizeInBytes() const {
		return ::boost::math::lcm<SizeType>( _elementSizeInBytes, MIN_ALLOC_SIZE );
	}

// ---------------------------------------------------

	template <typename Allocator>
	ETInlineHint typename ObjectPool<Allocator>::SizeType ObjectPool<Allocator>::GetObjectCountForAllocationSizeInBytes( SizeType sizeInBytes ) const {
		const SizeType	objectSizeInBytes( GetObjectSizeInBytes() );

		return ( sizeInBytes + ( objectSizeInBytes - 1u ) ) / objectSizeInBytes;
	}

// ---------------------------------------------------

	template <typename Allocator>
	ETInlineHint void ObjectPool<Allocator>::SetObjectSizeInBytes( SizeType newSizeInBytes ) {
		_elementSizeInBytes = newSizeInBytes;
	}

// ---------------------------------------------------

	template <typename Allocator>
	ETInlineHint bool ObjectPool<Allocator>::OwnsChunk( void* const chunk ) const {
		return( FindPOD( chunk ).valid() );
    }

}	// namespace Utility
}	// namespace Eldritch2