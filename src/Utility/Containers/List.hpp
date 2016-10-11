/*==================================================================*\
  List.hpp
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
#include <eastl/list.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename Value, typename Allocator = Eldritch2::ChildAllocator>
	class List {
	// - TYPE PUBLISHING ---------------------------------

	protected:
		using UnderlyingContainer	= eastl::list<Value, Detail::EaStlAllocatorAdapterMixin<Allocator>>;

	public:
		using ValueType				= typename UnderlyingContainer::value_type;
		using AllocatorType			= typename UnderlyingContainer::allocator_type::PublicType;
		using Reference				= ValueType&;
		using ConstReference		= const ValueType&;
		using Iterator				= typename UnderlyingContainer::iterator;
		using ConstIterator			= typename UnderlyingContainer::const_iterator;
		using SizeType				= typename UnderlyingContainer::size_type;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------
		
	public:
	//! Constructs this @ref List instance.
		explicit List( const AllocatorType& allocator = AllocatorType() );
	//! Constructs this @ref List instance.
		template <typename InputIterator>
		List( InputIterator first, InputIterator last, const AllocatorType& allocator = AllocatorType() );
	//! Constructs this @ref List instance.
		template <class = eastl::enable_if<eastl::is_copy_constructible<ValueType>::value>::type>
		List( const List&, const AllocatorType& allocator = AllocatorType() );
	//! Constructs this @ref List instance.
		List( List&& );

		~List() = default;

	// - ALGORITHMS --------------------------------------

	public:
		ConstIterator	Find( ConstReference itemTemplate, ConstIterator searchHint ) const;
		Iterator		Find( ConstReference itemTemplate, Iterator searchHint );

		template <typename ItemPredicate>
		Iterator		RemoveIf( ItemPredicate itemPredicate );

	// - ELEMENT ITERATION -------------------------------

	public:
	//! Retrieves a @ref ConstIterator pointing to the first element stored in this @ref List.
		ConstIterator	ConstBegin() const;

	//!	Retrieves a @ref ConstIterator pointing one position after the last element stored in this @ref List.
		ConstIterator	ConstEnd() const;

	//!	Retrieves a @ref ConstIterator pointing to the first element stored in this @ref List.
		ConstIterator	Begin() const;
	//!	Retrieves an @ref Iterator pointing to the first element stored in this @ref List.
		Iterator		Begin();

	//!	Retrieves a @ref ConstIterator pointing one position after the last element stored in this @ref List.
		ConstIterator	End() const;
	//!	Retrieves an @ref Iterator pointing one position after the last element stored in this @ref List.
		Iterator		End();

	// - END POINT MANIPULATION --------------------------

	public:
	//!	Retrieves a reference to the first element stored in this @ref List.
		ConstReference	Front() const;
	//!	Retrieves a reference to the first element stored in this @ref List.
		Reference		Front();

	//!	Retrieves a reference to the last element stored in this @ref List.
		ConstReference	Back() const;
	//!	Retrieves a reference to the last element stored in this @ref List.
		Reference		Back();

	//!	Adds a duplicate of the passed-in item to the head of this @ref List.
		template <class = eastl::enable_if<eastl::is_copy_constructible<ValueType>::value>::type>
		void			PushFront( ConstReference itemTemplate );

	//!	Adds a duplicate of the passed-in item to the tail of this @ref List.
		template <class = eastl::enable_if<eastl::is_copy_constructible<ValueType>::value>::type>
		void			PushBack( ConstReference itemTemplate );

		template <typename... ElementConstructorArguments>
		void			EmplaceFront( ElementConstructorArguments&&... elementConstructorArguments );

		template <typename... ElementConstructorArguments>
		void			EmplaceBack( ElementConstructorArguments&&... elementConstructorArguments );

	//!	Removes the head element of this @ref List, reducing its size by one element.
		void			PopFront();

	//!	Removes the tail element of this @ref List, reducing its size by one element.
		void			PopBack();

	// - CONTAINER MANIPULATION --------------------------

	public:
	//!	Inserts an element at the position specified, shifting all antecedent elements down one position.
		template <class = eastl::enable_if<eastl::is_copy_constructible<ValueType>::value>::type>
		Iterator	Insert( Iterator location, ConstReference itemTemplate );

		template <typename... ElementConstructorArguments>
		Iterator	Emplace( Iterator location, ElementConstructorArguments&&... elementConstructorArguments );

	//!	Removes a range of elements in the range [first, last), reducing the size of the @ref List by <<distance( first, last )>>
		Iterator	Erase( Iterator first, Iterator last );
	//!	Removes an element at the position specified, reducing the size of the @ref List by one element.
		Iterator	Erase( Iterator location );

	//!	Removes all elements from the @ref List.
		void		Clear();

	// - CONTAINER DUPLICATION ---------------------------

	public:
		template <class = eastl::enable_if<eastl::is_copy_constructible<ValueType>::value>::type>
		List&	operator=( const List& );
		List&	operator=( List&& );

		void	Swap( List& container );

	// - CONTENT QUERY -----------------------------------

	public:
	//!	Counts the number of elements contained within this @ref List.
		SizeType	GetSize() const;

	//!	Returns a bool indicating whether or not there are any elements contained in this @ref List.
		bool		IsEmpty() const;

		explicit	operator bool() const;

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
#include <Utility/Containers/List.inl>
//------------------------------------------------------------------//