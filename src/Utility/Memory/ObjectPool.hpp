/*==================================================================*\
  ObjectPool.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Memory/ChildAllocator.hpp>
//------------------------------------------------------------------//
#include <boost/pool/pool.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Utility {

	struct ObjectPoolOrderedHint {};

	extern const ObjectPoolOrderedHint	orderedPoolBehavior;

// ---------------------------------------------------

	template <typename Allocator = ::Eldritch2::ChildAllocator>
	class ObjectPool : protected ::boost::simple_segregated_storage<typename Allocator::SizeType> {
	public:
		typedef Allocator								AllocatorType;
		typedef typename AllocatorType::SizeType		SizeType;
		typedef typename AllocatorType::DifferenceType	DifferenceType;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		//!	Constructs this @ref ObjectPool instance.
		template <typename... AllocatorConstructorArguments>
		ETInlineHint ObjectPool( SizeType elementSizeInBytes, SizeType objectsPerPage, AllocatorConstructorArguments... allocatorConstructorArguments );

		//!	Destroys this @ref ObjectPool instance.
		~ObjectPool();

	// ---------------------------------------------------

		// Allocates an object from the pool, requesting more memory from the backing allocator as appropriate.
		ETRestrictHint void*	Allocate();
		// Allocates an object from the pool, requesting more memory from the backing allocator as appropriate.
		// This overload of Allocate() should be preferred when the pool will also be servicing array allocation requests.
		ETRestrictHint void*	Allocate( const Utility::ObjectPoolOrderedHint& orderedHint );
		// Allocates a contiguous array of objects from this pool, requesting more memory from the backing allocator as appropriate.
		ETRestrictHint void*	Allocate( SizeType arraySizeInObjects );

	// ---------------------------------------------------

		// Deallocates a previously-allocated block of memory from this pool.
		void	Deallocate( void* const chunk );
		// Deallocates a previously-allocated block of memory from this pool, and adds it back into the memory store with ordered semantics.
		// This overload of Deallocate() should be preferred when the pool will also be servicing array allocation requests.
		void	Deallocate( void* const chunk, const Utility::ObjectPoolOrderedHint& orderedHint );
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
		ENUM_CLASS( size_type ) {
			MIN_ALLOC_SIZE = ::boost::math::static_lcm<sizeof( void* ), sizeof( SizeType )>::value
		};

		typedef ::boost::simple_segregated_storage<SizeType>	SegregatedStorageType;

	// ---------------------------------------------------

		ETRestrictHint void*	AllocNeedResize();

		ETRestrictHint void*	OrderedAllocNeedResize();

	// ---------------------------------------------------

	protected:
		ETInlineHint SegregatedStorageType&			Store();
		ETInlineHint const SegregatedStorageType&	Store() const;

	// ---------------------------------------------------

		::boost::details::PODptr<SizeType>	FindPOD( void* const chunk ) const;

	// ---------------------------------------------------

		static bool	IsFrom( void* const chunk, char* const i, SizeType sizeof_i );

	// ---------------------------------------------------

		ETInlineHint static void*&	NextOf( void* const ptr );

	// ---------------------------------------------------

	private:
		AllocatorType						_allocator;
		::boost::details::PODptr<SizeType>	_list;

	    SizeType							_elementSizeInBytes;
	    SizeType							_objectsPerPage;
	    SizeType							_startSize;
	};

}	// namespace Utility
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/Memory/ObjectPool.inl>
//------------------------------------------------------------------//