/*==================================================================*\
  StdAllocatorAdapterMixin.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {

	template<typename T, typename Allocator>
	ETForceInlineHint typename StdAllocatorAdapterMixin<T, Allocator>::pointer StdAllocatorAdapterMixin<T, Allocator>::address( reference r ) const {
		return &r;
	}

// ---------------------------------------------------

	template<typename T, typename Allocator>
	ETForceInlineHint typename StdAllocatorAdapterMixin<T, Allocator>::const_pointer StdAllocatorAdapterMixin<T, Allocator>::address( const_reference s ) const {
		return &s;
	}

// ---------------------------------------------------

	template<typename T, typename Allocator>
	ETForceInlineHint typename ::Eldritch2::StdAllocatorAdapterMixin<T, Allocator>::size_type StdAllocatorAdapterMixin<T, Allocator>::max_size() const {
		// The following has been carefully written to be independent of
		// the definition of size_t and to avoid signed/unsigned warnings.
		return static_cast<size_type>(this->GetAllocationLimitInBytes()) / sizeof(T);
	}

// ---------------------------------------------------

	template<typename T, typename Allocator>
	ETForceInlineHint bool StdAllocatorAdapterMixin<T, Allocator>::operator!=( const StdAllocatorAdapterMixin<T, Allocator>& other ) const {
	    return !(*this == other);
	}

// ---------------------------------------------------

	// Returns true if and only if storage allocated from *this
	// can be deallocated from other, and vice versa.
	// Always returns true for stateless allocators.
	template<typename T, typename Allocator>
	ETForceInlineHint bool StdAllocatorAdapterMixin<T, Allocator>::operator==( const StdAllocatorAdapterMixin<T, Allocator>& other ) const {
		return( static_cast<const UnderlyingAllocator&>(*this) == static_cast<const UnderlyingAllocator&>(other) );
	}

// ---------------------------------------------------

	template<typename T, typename Allocator>
	ETForceInlineHint void StdAllocatorAdapterMixin<T, Allocator>::construct( T* const p, const_reference t ) const {
		new(static_cast<void*>(p)) T( t );
	}

// ---------------------------------------------------

	template <typename T, typename Allocator>
	void StdAllocatorAdapterMixin<T, Allocator>::destroy( T* const p ) const {
		// small MSVC bug workaround. The compiler thinks p is unreferenced despite the destructor call; this shuts it up.
		ETUnreferencedParameter( p );
	    p->~T();
	}

// ---------------------------------------------------

	template<typename T, typename Allocator>
	StdAllocatorAdapterMixin<T, Allocator>::StdAllocatorAdapterMixin( const UnderlyingAllocator& allocatorTemplate ) : UnderlyingAllocator( allocatorTemplate ) {}

// ---------------------------------------------------

	template<typename T, typename Allocator>
	template<typename U, typename AlternateAllocator>
	StdAllocatorAdapterMixin<T, Allocator>::StdAllocatorAdapterMixin( const ::Eldritch2::StdAllocatorAdapterMixin<U, AlternateAllocator>& other ) : UnderlyingAllocator( other ) {}

// ---------------------------------------------------

	template<typename T, typename Allocator>
	ETRestrictHint T* StdAllocatorAdapterMixin<T, Allocator>::allocate( const size_type n ) {
		return n ? static_cast<T*>(this->Allocate( n * sizeof( T ) )) : nullptr;
	}

// ---------------------------------------------------

	template<typename T, typename Allocator>
	ETRestrictHint T* StdAllocatorAdapterMixin<T, Allocator>::allocate( const size_type n, const void* ) {
		return this->allocate( n );
	}

// ---------------------------------------------------

	template<typename T, typename Allocator>
	template<typename U>
	ETRestrictHint T* StdAllocatorAdapterMixin<T, Allocator>::allocate( const size_type n, const U* ) {
		return this->allocate( n );
	}

// ---------------------------------------------------

	template<typename T, typename Allocator>
	void StdAllocatorAdapterMixin<T, Allocator>::deallocate( T* const p, const size_type /*n*/ ) {
		this->Deallocate( static_cast<void*>(p) );
	}

// ---------------------------------------------------

	template<typename T, size_t alignment, typename Allocator>
	AlignedStdAllocatorAdapterMixin<T, alignment, Allocator>::AlignedStdAllocatorAdapterMixin( const UnderlyingAllocator& allocatorTemplate ) : StdAllocatorAdapterMixin<T, Allocator>( allocatorTemplate ) {}

// ---------------------------------------------------

	template<typename T, size_t alignment, typename Allocator>
	template<typename U, size_t alternateAlignment, typename AlternateAllocator>
	AlignedStdAllocatorAdapterMixin<T, alignment, Allocator>::AlignedStdAllocatorAdapterMixin( const ::Eldritch2::AlignedStdAllocatorAdapterMixin<U, alternateAlignment, AlternateAllocator>& other ) : StdAllocatorAdapterMixin<T, Allocator>( other ) {}

// ---------------------------------------------------

	template<typename T, size_t alignment, typename Allocator>
	ETRestrictHint T* AlignedStdAllocatorAdapterMixin<T, alignment, Allocator>::allocate( const size_type n ) {
		return n ? static_cast<T*>(this->Allocate( n * sizeof(T), alignment )) : nullptr;
	}

// ---------------------------------------------------

	template<typename T, size_t alignment, typename Allocator>
	ETRestrictHint T* AlignedStdAllocatorAdapterMixin<T, alignment, Allocator>::allocate( const size_type n, const void* ) {
		return this->allocate( n );
	}

// ---------------------------------------------------

	template<typename T, size_t alignment, typename Allocator>
	template <typename U>
	ETRestrictHint T* AlignedStdAllocatorAdapterMixin<T, alignment, Allocator>::allocate( const size_type n, const U* ) {
		return this->allocate( n );
	}

// ---------------------------------------------------

	template<typename T, size_t alignment, typename Allocator>
	void AlignedStdAllocatorAdapterMixin<T, alignment, Allocator>::deallocate( T* const p, const size_type /*n*/ ) {
		this->DeallocateAligned( static_cast<void*>(p) );
	}

}	// namespace Eldritch2