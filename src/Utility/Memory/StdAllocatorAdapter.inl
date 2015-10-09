/*==================================================================*\
  StdAllocatorAdapter.inl
  ------------------------------------------------------------------
  Purpose:
  Definition of a basic allocator object that can be used to manage
  memory. Note that this class uses the slightly different (and
  vastly superior) per-instance Lakos allocator model (see "Towards
  a Better Allocator Model" for details) as opposed to the standard
  per-type C++ model. If compatibility with the old style allocator
  is of concern, see the accompanying wrapper class defined in
  ETStdAllocator.hpp.

  ------------------------------------------------------------------
  (c)2010 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2
{

	template<typename T, typename Allocator>
	ETForceInlineHint typename StdAllocatorAdapter<T, Allocator>::pointer StdAllocatorAdapter<T, Allocator>::address( reference r ) const {
		return &r;
	}

// ---------------------------------------------------

	template<typename T, typename Allocator>
	ETForceInlineHint typename StdAllocatorAdapter<T, Allocator>::const_pointer StdAllocatorAdapter<T, Allocator>::address( const_reference s ) const {
		return &s;
	}

// ---------------------------------------------------

	template<typename T, typename Allocator>
	ETForceInlineHint typename ::Eldritch2::StdAllocatorAdapter<T, Allocator>::size_type StdAllocatorAdapter<T, Allocator>::max_size() const {
		// The following has been carefully written to be independent of
		// the definition of size_t and to avoid signed/unsigned warnings.
		return static_cast<size_type>(this->GetAllocationLimitInBytes()) / sizeof(T);
	}

// ---------------------------------------------------

	template<typename T, typename Allocator>
	ETForceInlineHint bool StdAllocatorAdapter<T, Allocator>::operator!=( const StdAllocatorAdapter<T, Allocator>& other ) const {
	    return !(*this == other);
	}

// ---------------------------------------------------

	// Returns true if and only if storage allocated from *this
	// can be deallocated from other, and vice versa.
	// Always returns true for stateless allocators.
	template<typename T, typename Allocator>
	ETForceInlineHint bool StdAllocatorAdapter<T, Allocator>::operator==( const StdAllocatorAdapter<T, Allocator>& other ) const {
		return( static_cast<const UnderlyingAllocator&>(*this) == static_cast<const UnderlyingAllocator&>(other) );
	}

// ---------------------------------------------------

	template<typename T, typename Allocator>
	ETForceInlineHint void StdAllocatorAdapter<T, Allocator>::construct( T* const p, const_reference t ) const {
		new(static_cast<void*>(p)) T( t );
	}

// ---------------------------------------------------

	template <typename T, typename Allocator>
	void StdAllocatorAdapter<T, Allocator>::destroy( T* const p ) const {
		// small MSVC bug workaround. The compiler thinks p is unreferenced
		// despite the destructor call; this shuts it up.
		ETUnreferencedParameter( p );
	    p->~T();
	}

// ---------------------------------------------------

	template<typename T, typename Allocator>
	StdAllocatorAdapter<T, Allocator>::StdAllocatorAdapter( const UnderlyingAllocator& allocatorTemplate ) : UnderlyingAllocator( allocatorTemplate ) {}

// ---------------------------------------------------

	template<typename T, typename Allocator>
	template<typename U, typename AlternateAllocator>
	StdAllocatorAdapter<T, Allocator>::StdAllocatorAdapter( const ::Eldritch2::StdAllocatorAdapter<U, AlternateAllocator>& other ) : UnderlyingAllocator( other ) {}

// ---------------------------------------------------

	template<typename T, typename Allocator>
	StdAllocatorAdapter<T, Allocator>::~StdAllocatorAdapter() {}

// ---------------------------------------------------

	template<typename T, typename Allocator>
	ETRestrictHint T* StdAllocatorAdapter<T, Allocator>::allocate( const size_type n ) {
		return n ? static_cast<T*>(this->Allocate( n * sizeof( T ) )) : nullptr;
	}

// ---------------------------------------------------

	template<typename T, typename Allocator>
	ETRestrictHint T* StdAllocatorAdapter<T, Allocator>::allocate( const size_type n, const void* ) {
		return this->allocate( n );
	}

// ---------------------------------------------------

	template<typename T, typename Allocator>
	template<typename U>
	ETRestrictHint T* StdAllocatorAdapter<T, Allocator>::allocate( const size_type n, const U* ) {
		return this->allocate( n );
	}

// ---------------------------------------------------

	template<typename T, typename Allocator>
	void StdAllocatorAdapter<T, Allocator>::deallocate( T* const p, const size_type n ) {
		ETUnreferencedParameter( n );

		this->Deallocate( static_cast<void*>(p) );
	}

// ---------------------------------------------------

	template<typename T, size_t alignment, typename Allocator>
	AlignedStdAllocatorAdapter<T, alignment, Allocator>::AlignedStdAllocatorAdapter( const UnderlyingAllocator& allocatorTemplate ) : StdAllocatorAdapter<T, Allocator>( allocatorTemplate ) {}

// ---------------------------------------------------

	template<typename T, size_t alignment, typename Allocator>
	template<typename U, size_t alternateAlignment, typename AlternateAllocator>
	AlignedStdAllocatorAdapter<T, alignment, Allocator>::AlignedStdAllocatorAdapter( const ::Eldritch2::AlignedStdAllocatorAdapter<U, alternateAlignment, AlternateAllocator>& other ) : StdAllocatorAdapter<T, Allocator>( other ) {}

// ---------------------------------------------------

	template<typename T, size_t alignment, typename Allocator>
	AlignedStdAllocatorAdapter<T, alignment, Allocator>::~AlignedStdAllocatorAdapter() {}

// ---------------------------------------------------

	template<typename T, size_t alignment, typename Allocator>
	ETRestrictHint T* AlignedStdAllocatorAdapter<T, alignment, Allocator>::allocate( const size_type n ) {
		return n ? static_cast<T*>(this->Allocate( n * sizeof(T), alignment )) : nullptr;
	}

// ---------------------------------------------------

	template<typename T, size_t alignment, typename Allocator>
	ETRestrictHint T* AlignedStdAllocatorAdapter<T, alignment, Allocator>::allocate( const size_type n, const void* ) {
		return this->allocate( n );
	}

// ---------------------------------------------------

	template<typename T, size_t alignment, typename Allocator>
	template <typename U>
	ETRestrictHint T* AlignedStdAllocatorAdapter<T, alignment, Allocator>::allocate( const size_type n, const U* ) {
		return this->allocate( n );
	}

// ---------------------------------------------------

	template<typename T, size_t alignment, typename Allocator>
	void AlignedStdAllocatorAdapter<T, alignment, Allocator>::deallocate( T* const p, const size_type n ) {
		ETUnreferencedParameter( n );

		this->DeallocateAligned( static_cast<void*>(p) );
	}

}	// namespace Eldritch2