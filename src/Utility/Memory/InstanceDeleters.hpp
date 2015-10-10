/*==================================================================*\
  IntrusiveDisposers.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  �2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/Compiler.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	class	Allocator;
}

namespace Eldritch2 {
namespace Utility {

	namespace Utility	= ::Eldritch2::Utility;

// ---------------------------------------------------

	class InstanceDeleter {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref InstanceDeleter instance.
		ETInlineHint InstanceDeleter( ::Eldritch2::Allocator& allocator );
		//! Constructs this @ref InstanceDeleter instance.
		ETInlineHint InstanceDeleter( const Utility::InstanceDeleter& source ) = default;

		//!	Destroys this @ref InstanceDeleter instance.
		ETInlineHint ~InstanceDeleter() = default;

	// ---------------------------------------------------

		template <typename T>
		ETInlineHint void	operator()( T* const pointer ) const;
		template <typename T>
		ETInlineHint void	operator()( T& object ) const;

	// - DATA MEMBERS ------------------------------------

	protected:
		::Eldritch2::Allocator&	_allocator;
	};

// ---------------------------------------------------

	class AlignedInstanceDeleter : public Utility::InstanceDeleter {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref AlignedInstanceDeleter instance.
		ETInlineHint AlignedInstanceDeleter( ::Eldritch2::Allocator& allocator );
		//! Constructs this @ref AlignedInstanceDeleter instance.
		ETInlineHint AlignedInstanceDeleter( const Utility::AlignedInstanceDeleter& source ) = default;

		//!	Destroys this @ref AlignedInstanceDeleter instance.
		ETInlineHint ~AlignedInstanceDeleter() = default;

	// ---------------------------------------------------

		template <typename T>
		ETInlineHint void	operator()( T* const pointer ) const;
		template <typename T>
		ETInlineHint void	operator()( T& object ) const;
	};

}	// namespace Utility
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/Memory/InstanceDeleters.inl>
//------------------------------------------------------------------//