/*==================================================================*\
  LinkedList.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Memory/EaStlAllocatorMixin.hpp>
#include <Common/Containers/RangeAdapters.hpp>
#include <Common/Memory/MallocAllocator.hpp>
//------------------------------------------------------------------//
#include <eastl/list.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename Value, typename Allocator = MallocAllocator>
	class LinkedList {
	// - TYPE PUBLISHING ---------------------------------

	protected:
		using UnderlyingContainer	= eastl::list<Value, EaStlAllocatorMixin<Allocator>>;

	public:
		using AllocatorType			= typename UnderlyingContainer::allocator_type::PublicType;
		using ValueType				= typename UnderlyingContainer::value_type;
		using ConstReference		= const ValueType&;
		using Reference				= ValueType&;
		using ConstIterator			= typename UnderlyingContainer::const_iterator;
		using Iterator				= typename UnderlyingContainer::iterator;
		using SizeType				= typename UnderlyingContainer::size_type;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------
		
	public:
	//! Constructs this @ref LinkedList instance.
		template <typename InputIterator>
		LinkedList( InputIterator first, InputIterator last, const AllocatorType& allocator = AllocatorType() );
	//! Constructs this @ref LinkedList instance.
		template <class = eastl::enable_if<eastl::is_copy_constructible<ValueType>::value>::type>
		LinkedList( const LinkedList&, const AllocatorType& allocator = AllocatorType() );
	//! Constructs this @ref LinkedList instance.
		explicit LinkedList( const AllocatorType& allocator = AllocatorType() );
	//! Constructs this @ref LinkedList instance.
		LinkedList( LinkedList&& );

		~LinkedList() = default;

	// - ALGORITHMS --------------------------------------

	public:
		ConstIterator	Find( ConstReference itemTemplate, ConstIterator searchHint ) const;
		Iterator		Find( ConstReference itemTemplate, Iterator searchHint );

		template <typename ItemPredicate>
		Iterator		EraseIf( ItemPredicate itemPredicate );

	// - ELEMENT ITERATION -------------------------------

	public:
	//! Retrieves a @ref ConstIterator pointing to the first element stored in this @ref LinkedList.
		ConstIterator	ConstBegin() const;

	//!	Retrieves a @ref ConstIterator pointing one position after the last element stored in this @ref LinkedList.
		ConstIterator	ConstEnd() const;

	//!	Retrieves a @ref ConstIterator pointing to the first element stored in this @ref LinkedList.
		ConstIterator	Begin() const;
	//!	Retrieves an @ref Iterator pointing to the first element stored in this @ref LinkedList.
		Iterator		Begin();

	//!	Retrieves a @ref ConstIterator pointing one position after the last element stored in this @ref LinkedList.
		ConstIterator	End() const;
	//!	Retrieves an @ref Iterator pointing one position after the last element stored in this @ref LinkedList.
		Iterator		End();

	// - END POINT MANIPULATION --------------------------

	public:
	//!	Retrieves a reference to the first element stored in this @ref LinkedList.
		ConstReference	Front() const;
	//!	Retrieves a reference to the first element stored in this @ref LinkedList.
		Reference		Front();

	//!	Retrieves a reference to the last element stored in this @ref LinkedList.
		ConstReference	Back() const;
	//!	Retrieves a reference to the last element stored in this @ref LinkedList.
		Reference		Back();

	//!	Adds a duplicate of the passed-in item to the head of this @ref LinkedList.
		template <class = eastl::enable_if<eastl::is_copy_constructible<ValueType>::value>::type>
		void			Prepend( ConstReference itemTemplate );

	//!	Adds a duplicate of the passed-in item to the tail of this @ref LinkedList.
		template <class = eastl::enable_if<eastl::is_copy_constructible<ValueType>::value>::type>
		void			Append( ConstReference itemTemplate );

		template <typename... ElementConstructorArguments>
		void			EmplaceFront( ElementConstructorArguments&&... elementConstructorArguments );

		template <typename... ElementConstructorArguments>
		void			EmplaceBack( ElementConstructorArguments&&... elementConstructorArguments );

	//!	Removes the head element of this @ref LinkedList, reducing its size by one element.
		void			PopFront();

	//!	Removes the tail element of this @ref LinkedList, reducing its size by one element.
		void			Pop();

	// - CONTAINER MANIPULATION --------------------------

	public:
	//!	Inserts an element at the position specified, shifting all antecedent elements down one position.
		template <class = eastl::enable_if<eastl::is_copy_constructible<ValueType>::value>::type>
		Iterator	Insert( Iterator location, ConstReference itemTemplate );

		template <typename... ElementConstructorArguments>
		Iterator	Emplace( Iterator location, ElementConstructorArguments&&... elementConstructorArguments );

	//!	Removes a range of elements in the range [first, last), reducing the size of the @ref LinkedList by <<distance( first, last )>>
		Iterator	Erase( Iterator first, Iterator last );
	//!	Removes an element at the position specified, reducing the size of the @ref LinkedList by one element.
		Iterator	Erase( Iterator location );

	//!	Removes all elements from the @ref LinkedList.
		void		Clear();

	// - CONTAINER DUPLICATION ---------------------------

	public:
		template <class = eastl::enable_if<eastl::is_copy_constructible<ValueType>::value>::type>
		LinkedList&	operator=( const LinkedList& );
		LinkedList&	operator=( LinkedList&& );

	// - CONTENT QUERY -----------------------------------

	public:
	//!	Counts the number of elements contained within this @ref LinkedList.
		SizeType	GetSize() const;

	//!	Returns a bool indicating whether or not there are any elements contained in this @ref LinkedList.
		bool		IsEmpty() const;

		explicit	operator bool() const;

	// - ALLOCATOR ACCESS --------------------------------

	public:
		const AllocatorType&	GetAllocator() const;

	// - DATA MEMBERS ------------------------------------

	private:
		UnderlyingContainer	_container;

	// ---------------------------------------------------

		template <typename Value, typename Allocator>
		friend void	Swap( LinkedList<Value, Allocator>&, LinkedList<Value, Allocator>& );
	};

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Containers/LinkedList.inl>
//------------------------------------------------------------------//