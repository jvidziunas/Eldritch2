/*==================================================================*\
  RDESTLAllocatorAdapterMixin.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/Compiler.hpp>
//------------------------------------------------------------------//

namespace Eldritch2Detail {

	template <typename PublicAllocatorType>
	class RDESTLAllocatorAdapterMixin : public PublicAllocatorType {
	// - TYPE PUBLISHING ---------------------------------

	public:
		typedef typename PublicAllocatorType	RealAllocatorType;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		//! Constructs this @ref RDESTLAllocatorAdapterMixin instance.
		/*!	@param[in] allocator Const @ref RDESTLAllocatorAdapterMixin reference from which resources will be copied.
			*/
		ETForceInlineHint RDESTLAllocatorAdapterMixin( const ::Eldritch2Detail::RDESTLAllocatorAdapterMixin<PublicAllocatorType>& allocator ) = default;
		//! Constructs this @ref RDESTLAllocatorAdapterMixin instance.
		/*!	@param[in] allocator Movable @ref RDESTLAllocatorAdapterMixin reference from which resources will be moved.
			*/
		ETForceInlineHint RDESTLAllocatorAdapterMixin( ::Eldritch2Detail::RDESTLAllocatorAdapterMixin<PublicAllocatorType>&& allocator );
		//! Constructs this @ref RDESTLAllocatorAdapterMixin instance.
		/*!	@param[in] allocator Movable @ref Allocator subclass instance from which resources will be moved.
			*/
		ETForceInlineHint RDESTLAllocatorAdapterMixin( PublicAllocatorType&& allocator );

		//! Destroys this @ref RDESTLAllocatorAdapterMixin instance.
		ETForceInlineHint ~RDESTLAllocatorAdapterMixin() = default;

	// ---------------------------------------------------

		ETForceInlineHint void*	allocate( unsigned int bytes, int flags = 0 );

		ETForceInlineHint void*	allocate_aligned( unsigned int bytes, unsigned int alignment, int flags = 0 );

		ETForceInlineHint void	deallocate( void* ptr, unsigned int bytes );

	// ---------------------------------------------------

		ETForceInlineHint const char*	get_name() const;
	};

}	// namespace Eldritch2Detail

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/Memory/RDESTLAllocatorAdapterMixin.inl>
//------------------------------------------------------------------//