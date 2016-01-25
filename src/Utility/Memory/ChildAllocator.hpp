/*==================================================================*\
  ChildAllocator.hpp
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

	class ChildAllocator : public ::Eldritch2::Allocator {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref ChildAllocator instance.
		/*! @param[in] allocator @ref Allocator the @ref ChildAllocator should forward requests to.
			@param[in] name @ref Null-terminated C string containing a human-readable C string describing the purpose for which the @ref ChildAllocator is being created.
			*/
		ChildAllocator( ::Eldritch2::Allocator& allocator, const ::Eldritch2::UTF8Char* const name );
#if( ET_DEBUG_MODE_ENABLED )
		//! Constructs this @ref ChildAllocator instance.
		/*! @param[in] allocator @ref Allocator the @ref ChildAllocator should forward requests to.
			@param[in] name @ref Null-terminated C string containing a human-readable C string describing the purpose for which the @ref ChildAllocator is being created.
			@remarks This is a special debug-only overload designed to help catch instances where hierarchal containers are not constructing real allocators.
			*/
		ChildAllocator( ::Eldritch2::ChildAllocator& allocator, const ::Eldritch2::UTF8Char* const name );
#endif
		//! Constructs this @ref ChildAllocator instance.
		/*!	@param[in] allocator Allocator to be copied.
			*/
		ChildAllocator( const ::Eldritch2::ChildAllocator& allocator );
		//! Constructs this @ref ChildAllocator instance.
		/*!	@param[in] allocator Allocator to be moved.
			*/
		ChildAllocator( ::Eldritch2::ChildAllocator&& allocator );
		//! Constructs this @ref ChildAllocator instance.
		ChildAllocator();

		//! Destroys this @ref ChildAllocator instance.
		~ChildAllocator() = default;

	// - MEMORY ALLOCATION/DEALLOCATION ------------------
		
		ETRestrictHint void*	Allocate( const SizeType sizeInBytes, const AllocationOptions options ) override;
		ETRestrictHint void*	Allocate( const SizeType sizeInBytes, const SizeType alignmentInBytes, const AllocationOptions options ) override;

		ETRestrictHint void*	Reallocate( void* const address, const SizeType newSizeInBytes, const ReallocationOptions options ) override;
		ETRestrictHint void*	Reallocate( void* const address, const SizeType newSizeInBytes, const SizeType alignmentInBytes, const ReallocationOptions options ) override;

		void					Deallocate( void* const address ) override;
		void					Deallocate( void* const address, const AlignedDeallocationSemanticsTag ) override;

	// ---------------------------------------------------

#if( ET_DEBUG_MODE_ENABLED )
		//!	Helper method to catch some corner cases with hierarchal containers.
		/*!	@returns @parblock _True_ if this @ref ChildAllocator was default constructed (Necessary for some template container compatibility but is dangerous; container users should move construct a 'real' allocator over the anonymous one)
				or _false_ if the @ref ChildAllocator was correctly constructed.
			*/
		bool	IsAnonymous() const;
#endif

	// ---------------------------------------------------

		//!	Gets a reference to the @ref Allocator that owns this @ref ChildAllocator.
		/*!	@returns A reference to the @ref Allocator instance this @ref ChildAllocator forwards allocation requests to.
			*/
		ETForceInlineHint ::Eldritch2::Allocator&	GetParent() const;

	// - DATA MEMBERS ------------------------------------

	protected:
		::Eldritch2::Allocator&	_parent;
	};

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/Memory/ChildAllocator.inl>
//------------------------------------------------------------------//