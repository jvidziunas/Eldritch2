/*==================================================================*\
  EaStlAllocatorAdapterMixin.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/Compiler.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Detail {

	template <typename EldritchAllocator>
	class EaStlAllocatorAdapterMixin : public EldritchAllocator {
	// - TYPE PUBLISHING ---------------------------------

	public:
		using PublicType	= typename EldritchAllocator;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//! Constructs this @ref EaStlAllocatorAdapterMixin instance.
	/*!	@param[in] allocator Movable @ref EaStlAllocatorAdapterMixin reference from which resources will be moved. */
		EaStlAllocatorAdapterMixin( EaStlAllocatorAdapterMixin&& allocator );
	//! Constructs this @ref EaStlAllocatorAdapterMixin instance.
	/*!	@param[in] allocator Movable @ref PublicAllocatorType reference from which resources will be moved. */
		EaStlAllocatorAdapterMixin( const PublicType& allocator );
	//! Constructs this @ref EaStlAllocatorAdapterMixin instance.
	/*!	@param[in] allocator Movable @ref PublicAllocatorType reference from which resources will be moved. */
		EaStlAllocatorAdapterMixin( PublicType&& allocator );
	//! Constructs this @ref EaStlAllocatorAdapterMixin instance.
		EaStlAllocatorAdapterMixin( const EaStlAllocatorAdapterMixin& ) = default;

		~EaStlAllocatorAdapterMixin() = default;

	// ---------------------------------------------------

	public:
		ETRestrictHint void*	allocate( size_t allocationSizeInBytes, size_t alignmentInBytes, size_t offsetInBytes, int flags = 0 );
		ETRestrictHint void*	allocate( size_t allocationSizeInBytes, int flags = 0 );

		void					deallocate( void* pointer, size_t allocationSizeInBytes );

	// ---------------------------------------------------

	public:
		ETInlineHint EaStlAllocatorAdapterMixin&	operator=( const EaStlAllocatorAdapterMixin& ) = default;

	// ---------------------------------------------------

	public:
		const char* get_name() const;

		void        set_name( const char* name );
	};

}	// namespace Detail
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/Memory/EaStlAllocatorAdapterMixin.inl>
//------------------------------------------------------------------//


