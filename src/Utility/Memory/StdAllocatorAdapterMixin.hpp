/*==================================================================*\
  StdAllocatorAdapterMixin.hpp
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

	template <typename T, typename Allocator = ::Eldritch2::ChildAllocator>
	class StdAllocatorAdapterMixin : public Allocator {
	// - TYPE PUBLISHING ---------------------------------

	public:
		using UnderlyingAllocatorType	= Allocator;
		using value_type				= T;
		using size_type					= typename UnderlyingAllocatorType::SizeType;
		using difference_type			= typename UnderlyingAllocatorType::DifferenceType;
		using pointer					= value_type*;
		using const_pointer				= const value_type*;
		using reference					= value_type&;
		using const_reference			= const value_type&;

		template <typename U, typename AlternateAllocator = UnderlyingAllocatorType>
		struct rebind {
			typedef ::Eldritch2::StdAllocatorAdapterMixin<U, AlternateAllocator> other;
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		//!	Constructs this @ref StdAllocatorAdapterMixin instance.
		StdAllocatorAdapterMixin( const UnderlyingAllocatorType& allocatorTemplate );
		//!	Constructs this @ref StdAllocatorAdapterMixin instance.
		template<typename U, typename AlternateAllocatorType>
		StdAllocatorAdapterMixin( const ::Eldritch2::StdAllocatorAdapterMixin<U, AlternateAllocatorType>& other );

		//!	Destroys this @ref StdAllocatorAdapterMixin instance.
		virtual	~StdAllocatorAdapterMixin() = default;

	// ---------------------------------------------------
		
		ETForceInlineHint pointer		address( reference r ) const;
		ETForceInlineHint const_pointer	address( const_reference s ) const;

	// ---------------------------------------------------
		
		ETForceInlineHint size_type	max_size() const;

	// ---------------------------------------------------
		
		ETForceInlineHint bool	operator!=( const ::Eldritch2::StdAllocatorAdapterMixin<T, AllocatorType>& other ) const;

		ETForceInlineHint bool	operator==( const ::Eldritch2::StdAllocatorAdapterMixin<T, AllocatorType>& other ) const;

	// ---------------------------------------------------
		
		ETForceInlineHint void	construct( T* const p, const_reference t ) const;

		ETForceInlineHint void	destroy( T* const p ) const;

	// ---------------------------------------------------

		ETRestrictHint pointer	allocate( const size_type n );
		ETRestrictHint pointer	allocate( const size_type n, const void* hint );
		template<typename U>
		ETRestrictHint pointer	allocate( const size_type n, const U* hint );

		void					deallocate( T* const p, const size_type n );
	};

// ---------------------------------------------------

	template<typename T, size_t alignment = ::Eldritch2::TypeTraits::AlignmentOf<T>::val, typename AllocatorType = ::Eldritch2::ChildAllocator>
	class AlignedStdAllocatorAdapterMixin : public ::Eldritch2::StdAllocatorAdapterMixin<T, AllocatorType> {
	// - TYPE PUBLISHING ---------------------------------
		
	public:
		template <typename U, size_t alternateAlignment = alignment, typename AlternateAllocator = UnderlyingAllocatorType>
		struct rebind {
			typedef ::Eldritch2::AlignedStdAllocatorAdapterMixin<U, alternateAlignment, AlternateAllocator> other;
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		//!	Constructs this @ref AlignedStdAllocatorAdapterMixin instance.
		AlignedStdAllocatorAdapterMixin( const UnderlyingAllocatorType& allocatorTemplate );
		//!	Constructs this @ref AlignedStdAllocatorAdapterMixin instance.
		template<typename U, size_t alignment, typename AlternateAllocatorType>
		AlignedStdAllocatorAdapterMixin( const ::Eldritch2::AlignedStdAllocatorAdapterMixin<U, alignment, AlternateAllocatorType>& other );
		
		//!	Destroys this @ref AlignedStdAllocatorAdapterMixin instance.
		~AlignedStdAllocatorAdapterMixin() = default;

	// ---------------------------------------------------
		
		ETForceInlineHint size_type	max_size() const;

	// ---------------------------------------------------

		ETRestrictHint pointer	allocate( const size_type n );
		ETRestrictHint pointer	allocate( const size_type n, const void* hint );
		template <typename U>
		ETRestrictHint pointer	allocate( const size_type n, const U* hint );

		void					deallocate( T* const p, const size_type n );
	};

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/Memory/StdAllocatorAdapterMixin.inl>
//------------------------------------------------------------------//