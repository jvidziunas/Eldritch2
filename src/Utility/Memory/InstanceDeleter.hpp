/*==================================================================*\
  InstanceDeleter.hpp
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
#include <EASTL/bonus/compressed_pair.h>
#include <eastl/type_traits.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
	class	Allocator;
}

namespace Eldritch2 {

	template <typename T>
	class InstanceDeleter {
	// - TYPE PUBLISHING ---------------------------------

	public:
		using Pointer	= typename eastl::remove_const<T>::type*;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//! Constructs this @ref InstanceDeleter instance.
		InstanceDeleter( Eldritch2::Allocator& allocator );
	//! Constructs this @ref InstanceDeleter instance.
		template <typename U, class = eastl::enable_if<eastl::is_base_of<T, U>::value>::type>
		InstanceDeleter( const InstanceDeleter<U>& );
	//! Constructs this @ref InstanceDeleter instance.
		InstanceDeleter( const InstanceDeleter& ) = default;
	//! Constructs this @ref InstanceDeleter instance.
		InstanceDeleter() = default;

		~InstanceDeleter() = default;

	// ---------------------------------------------------

	public:
		Eldritch2::Allocator&	GetAllocator() const;

	// ---------------------------------------------------

	public:
		void	operator()( Pointer object ) const;

	// ---------------------------------------------------

	public:
		template <typename U, class = eastl::enable_if<eastl::is_base_of<T, U>::value>::type>
		InstanceDeleter&	operator=( const InstanceDeleter<U>& );
		InstanceDeleter&	operator=( const InstanceDeleter& ) = default;

	// - DATA MEMBERS ------------------------------------

	private:
	//	TODO: Specialize this to not store a size if the type isn't polymorphic?
		eastl::compressed_pair<Eldritch2::Allocator*, size_t>	_allocatorAndSize;

	// ---------------------------------------------------

		template <typename U>
		friend class Eldritch2::InstanceDeleter;
	};

// ---------------------------------------------------

	template <typename T>
	class InstanceDeleter<T[]> {
	// - TYPE PUBLISHING ---------------------------------

	public:
		using Pointer	= T*;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//! Constructs this @ref InstanceDeleter instance.
		InstanceDeleter( size_t countInObjects, Eldritch2::Allocator& allocator );
	//! Constructs this @ref InstanceDeleter instance.
		InstanceDeleter( const InstanceDeleter& ) = default;
	//! Constructs this @ref InstanceDeleter instance.
		InstanceDeleter() = default;

		~InstanceDeleter() = default;

	// ---------------------------------------------------

	public:
		Eldritch2::Allocator&	GetAllocator() const;

		size_t					GetSize() const;

	// ---------------------------------------------------

	public:
		void	operator()( Pointer objects ) const;

	// ---------------------------------------------------

	public:
		InstanceDeleter&	operator=( const InstanceDeleter& ) = default;

	// - DATA MEMBERS ------------------------------------

	private:
		Eldritch2::Allocator*	_allocator;
		size_t					_countInObjects;
	};

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/Memory/InstanceDeleter.inl>
//------------------------------------------------------------------//
