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

	template <typename T, typename Allocator = Eldritch2::ChildAllocator>
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
			using	other = Eldritch2::StdAllocatorAdapterMixin<U, AlternateAllocator>;
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref StdAllocatorAdapterMixin instance.
		template <typename... ConstructorArguments>
		StdAllocatorAdapterMixin( ConstructorArguments&&... constructorArguments );

		virtual	~StdAllocatorAdapterMixin() = default;

	// ---------------------------------------------------
		
	public:
		const_pointer	address( const_reference s ) const;
		pointer			address( reference r ) const;

	// ---------------------------------------------------
		
	public:
		size_type	max_size() const;

	// ---------------------------------------------------
		
	public:
		bool	operator!=( const Eldritch2::StdAllocatorAdapterMixin<T, Allocator>& other ) const;

		bool	operator==( const Eldritch2::StdAllocatorAdapterMixin<T, Allocator>& other ) const;

	// ---------------------------------------------------
		
	public:
		void	construct( T* const p, const_reference t ) const;

		void	destroy( T* const p ) const;

	// ---------------------------------------------------

	public:
		ETRestrictHint pointer	allocate( const size_type n );
		ETRestrictHint pointer	allocate( const size_type n, const void* hint );
		template<typename U>
		ETRestrictHint pointer	allocate( const size_type n, const U* hint );

		void					deallocate( T* const p, const size_type n );
	};

// ---------------------------------------------------

	template<typename T, size_t alignment = Eldritch2::TypeTraits::AlignmentOf<T>::val, typename AllocatorType = Eldritch2::ChildAllocator>
	class AlignedStdAllocatorAdapterMixin : public Eldritch2::StdAllocatorAdapterMixin<T, AllocatorType> {
	// - TYPE PUBLISHING ---------------------------------
		
	public:
		template <typename U, size_t alternateAlignment = alignment, typename AlternateAllocator = UnderlyingAllocatorType>
		struct rebind {
			using other	= Eldritch2::AlignedStdAllocatorAdapterMixin<U, alternateAlignment, AlternateAllocator>;
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref AlignedStdAllocatorAdapterMixin instance.
		template <typename... ConstructorArguments>
		AlignedStdAllocatorAdapterMixin( ConstructorArguments&&... constructorArguments );
		
		~AlignedStdAllocatorAdapterMixin() = default;

	// ---------------------------------------------------
		
	public:
		size_type	max_size() const;

	// ---------------------------------------------------

	public:
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