/*==================================================================*\
  Set.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Memory/EaStlAllocatorAdapterMixin.hpp>
#include <Utility/Containers/RangeAdapters.hpp>
#include <Utility/Memory/ChildAllocator.hpp>
//------------------------------------------------------------------//
#if ET_COMPILER_IS_MSVC && !defined( EA_COMPILER_HAS_C99_FORMAT_MACROS )
//	MSVC complains about *lots* of macro redefinitions in eabase/inttypes.h.
#	define EA_COMPILER_HAS_C99_FORMAT_MACROS
#endif
#include <eastl/set.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename Value, class Allocator = Eldritch2::ChildAllocator>
	class Set {
	// - TYPE PUBLISHING ---------------------------------

	protected:
		using UnderlyingContainer	= eastl::set<Value, Detail::EaStlAllocatorAdapterMixin<Allocator>>;

	public:
		using ValueType				= typename UnderlyingContainer::value_type;
		using AllocatorType			= typename UnderlyingContainer::allocator_type::PublicType;
		using Iterator				= typename UnderlyingContainer::iterator;
		using ConstIterator			= typename UnderlyingContainer::const_iterator;
		using SizeType				= typename AllocatorType::SizeType;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref Set instance.
		template <typename InputIterator>
		Set( InputIterator begin, InputIterator end, const AllocatorType& allocator = AllocatorType() );
	//!	Constructs this @ref Set instance.
		template <class = eastl::enable_if<eastl::is_copy_constructible<ValueType>::value>::type>
		Set( const Set& containerTemplate, const AllocatorType& allocator = AllocatorType() );
	//!	Constructs this @ref Set instance.
		explicit Set( const AllocatorType& allocator = AllocatorType() );
	//!	Constructs this @ref Set instance.
		Set( Set&& );

		~Set() = default;

	// - ALGORITHMS --------------------------------------

	public:
		ConstIterator	Find( const ValueType& itemTemplate ) const;
		Iterator		Find( const ValueType& itemTemplate );

		template <typename Predicate>
		void			RemoveIf( Predicate predicate );

	// - ELEMENT ITERATION -------------------------------

	public:
		ConstIterator	ConstBegin() const;

		ConstIterator	ConstEnd() const;

		ConstIterator	Begin() const;
		Iterator		Begin();
		
		ConstIterator	End() const;
		Iterator		End();

	// - CONTAINER DUPLICATION ---------------------------

	public:
		template <class = eastl::enable_if<eastl::is_copy_constructible<ValueType>::value>::type>
		Set&	operator=( const Set& );
		Set&	operator=( Set&& );

		void	Swap( Set& other );

	// - CONTAINER MANIPULATION --------------------------

	public:
		template <class = eastl::enable_if<eastl::is_copy_constructible<ValueType>::value>::type>
		Eldritch2::Pair<Iterator, bool>	Insert( const ValueType& value );
		Eldritch2::Pair<Iterator, bool>	Insert( ValueType&& value );

		Iterator						Erase( Iterator begin, Iterator end );
		Iterator						Erase( Iterator position );

		void							Clear();

	// - CONTENT QUERY -----------------------------------

	public:
		SizeType			GetSize() const;

		bool				IsEmpty() const;

		explicit operator	bool() const;

	// - ALLOCATOR ACCESS --------------------------------

	public:
		const AllocatorType&	GetAllocator() const;

	// - DATA MEMBERS ------------------------------------

	private:
		UnderlyingContainer	_underlyingContainer;
	};

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/Containers/Set.inl>
//------------------------------------------------------------------//