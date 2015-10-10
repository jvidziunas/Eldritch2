/*==================================================================*\
  StdAllocatorAdapter.hpp
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
#include <boost/type_traits/remove_cv.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename T, typename Allocator = ::Eldritch2::ChildAllocator>
	class StdAllocatorAdapter : public Allocator {
	public:
		typedef Allocator											UnderlyingAllocatorType;
		typedef T													value_type;
		typedef typename UnderlyingAllocatorType::SizeType			size_type;
		typedef typename UnderlyingAllocatorType::DifferenceType	difference_type;
		typedef value_type*											pointer;
		typedef const value_type*									const_pointer;
		typedef value_type&											reference;
		typedef const value_type&									const_reference;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		// Constructs this StdAllocatorAdapter instance.
		StdAllocatorAdapter( const UnderlyingAllocatorType& allocatorTemplate );
		// Constructs this StdAllocatorAdapter instance.
		template<typename U, typename AlternateAllocatorType>
		StdAllocatorAdapter( const ::Eldritch2::StdAllocatorAdapter<U, AlternateAllocatorType>& other );

		// Destroys this StdAllocatorAdapter instance.
		virtual	~StdAllocatorAdapter();

	// ---------------------------------------------------

		template <typename U, typename AlternateAllocator = UnderlyingAllocatorType>
		struct rebind {
			typedef ::Eldritch2::StdAllocatorAdapter<U, AlternateAllocator> other;
		};

	// ---------------------------------------------------
		
		ETForceInlineHint pointer		address( reference r ) const;
		ETForceInlineHint const_pointer	address( const_reference s ) const;

	// ---------------------------------------------------
		
		ETForceInlineHint size_type	max_size() const;

	// ---------------------------------------------------
		
		ETForceInlineHint bool	operator!=( const ::Eldritch2::StdAllocatorAdapter<T, AllocatorType>& other ) const;

		ETForceInlineHint bool	operator==( const ::Eldritch2::StdAllocatorAdapter<T, AllocatorType>& other ) const;

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
	class AlignedStdAllocatorAdapter : public ::Eldritch2::StdAllocatorAdapter<T, AllocatorType> {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------
		
	public:
		// Constructs this AlignedStdAllocatorAdapter instance.
		AlignedStdAllocatorAdapter( const UnderlyingAllocatorType& allocatorTemplate );
		// Constructs this AlignedStdAllocatorAdapter instance.
		template<typename U, size_t alignment, typename AlternateAllocatorType>
		AlignedStdAllocatorAdapter( const ::Eldritch2::AlignedStdAllocatorAdapter<U, alignment, AlternateAllocatorType>& other );
		
		// Destroys this AlignedStdAllocatorAdapter instance.
		~AlignedStdAllocatorAdapter();

	// ---------------------------------------------------

		template <typename U, size_t alternateAlignment = alignment, typename AlternateAllocator = UnderlyingAllocatorType>
		struct rebind {
			typedef ::Eldritch2::AlignedStdAllocatorAdapter<U, alternateAlignment, AlternateAllocator> other;
		};

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
#include <Utility/Memory/StdAllocatorAdapter.inl>
//------------------------------------------------------------------//