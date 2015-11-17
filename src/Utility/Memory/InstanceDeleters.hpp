/*==================================================================*\
  IntrusiveDisposers.hpp
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
#include <memory>
//------------------------------------------------------------------//

namespace Eldritch2 {
	class	Allocator;
}

namespace Eldritch2 {
namespace Utility {

	class InstanceDeleter {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref InstanceDeleter instance.
		ETInlineHint InstanceDeleter( ::Eldritch2::Allocator& allocator );
		//! Constructs this @ref InstanceDeleter instance.
		ETInlineHint InstanceDeleter( const Utility::InstanceDeleter& source ) = default;

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

		ETInlineHint ~AlignedInstanceDeleter() = default;

	// ---------------------------------------------------

		template <typename T>
		ETInlineHint void	operator()( T* const pointer ) const;
		template <typename T>
		ETInlineHint void	operator()( T& object ) const;
	};

}	// namespace Utility

	template <typename Object>
	using InstancePointer			= ::std::unique_ptr<Object, Utility::InstanceDeleter>;

	template <typename Object>
	using AlignedInstancePointer	= ::std::unique_ptr<Object, Utility::AlignedInstanceDeleter>;

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/Memory/InstanceDeleters.inl>
//------------------------------------------------------------------//
