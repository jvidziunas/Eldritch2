/*==================================================================*\
  List.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Memory/RDESTLAllocatorAdapterMixin.hpp>
#include <Utility/Memory/ChildAllocator.hpp>
//------------------------------------------------------------------//
#include <rdestl/list.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename StoredObject, typename Allocator = ::Eldritch2::ChildAllocator>
	class List : private ::rde::list<StoredObject, ::Eldritch2Detail::RDESTLAllocatorAdapterMixin<Allocator> > {
	// - TYPE PUBLISHING ---------------------------------

	protected:
		using PrivateAllocator		= ::Eldritch2Detail::RDESTLAllocatorAdapterMixin<Allocator>;
		using UnderlyingContainer	= ::rde::list<StoredObject, PrivateAllocator>;

	public:
		using ValueType				= typename UnderlyingContainer::value_type;
		using AllocatorType			= Allocator;
		using Reference				= ValueType&;
		using ConstReference		= const ValueType&;
		using Iterator				= typename UnderlyingContainer::iterator;
		using ConstIterator			= typename UnderlyingContainer::const_iterator;
		using SizeType				= typename UnderlyingContainer::size_type;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		//! Constructs this @ref List instance.
		template <typename... AllocatorConstructorArguments>
		ETInlineHint List( AllocatorConstructorArguments&&... allocatorConstructorArguments );
		//! Constructs this @ref List instance.
		template <typename InputIterator, typename... AllocatorConstructorArguments>
		ETInlineHint List( InputIterator first, InputIterator last, AllocatorConstructorArguments&&... allocatorConstructorArguments );
		//! Constructs this @ref List instance.
		template <typename AlternateAllocator, typename... AllocatorConstructorArguments>
		ETInlineHint List( const ::Eldritch2::List<StoredObject, AlternateAllocator>& listTemplate, AllocatorConstructorArguments&&... allocatorConstructorArguments );

		//! Destroys this @ref List instance.
		ETInlineHint ~List();

	// - ALGORITHMS --------------------------------------

		ETInlineHint Iterator		Find( ConstReference itemTemplate, Iterator searchHint );
		ETInlineHint ConstIterator	Find( ConstReference itemTemplate, ConstIterator searchHint ) const;

		template <typename ItemPredicate>
		ETInlineHint Iterator		RemoveIf( ItemPredicate itemPredicate );
		template <typename ExtraArgumentType, typename ItemPredicate>
		ETInlineHint Iterator		RemoveIf( ExtraArgumentType extraArgument, ItemPredicate itemPredicate );

	// - ELEMENT ITERATION -------------------------------

		// Retrieves an @ref Iterator pointing to the first element stored in this @ref List.
		Iterator		Begin();
		// Retrieves an @ref Iterator pointing to the first element stored in this @ref List.
		ConstIterator	Begin() const;

		// Retrieves an @ref Iterator pointing to the first element stored in this @ref List.
		ConstIterator	ConstBegin() const;

		// Retrieves an @ref Iterator pointing one position after the last element stored in this @ref List.
		Iterator		End();
		// Retrieves an @ref Iterator pointing one position after the last element stored in this @ref List.
		ConstIterator	End() const;

		// Retrieves an @ref Iterator pointing one position after the last element stored in this @ref List.
		ConstIterator	ConstEnd() const;

	// - END POINT MANIPULATION --------------------------

		// Retrieves a reference to the first element stored in this @ref List.
		ETInlineHint Reference		Front();
		// Retrieves a reference to the first element stored in this @ref List.
		ETInlineHint ConstReference	Front() const;

		// Adds a duplicate of the passed-in item to the head of this @ref List.
		ETInlineHint void			PushFront( ConstReference itemTemplate );

		// Removes the head element of this @ref List, reducing its size by one element.
		ETInlineHint void			PopFront();

		// Retrieves a reference to the last element stored in this @ref List.
		ETInlineHint Reference		Back();
		// Retrieves a reference to the last element stored in this @ref List.
		ETInlineHint ConstReference	Back() const;

		// Adds a duplicate of the passed-in item to the tail of this @ref List.
		ETInlineHint void			PushBack( ConstReference itemTemplate );

		// Removes the tail element of this @ref List, reducing its size by one element.
		ETInlineHint void			PopBack();

	// - CONTAINER MANIPULATION --------------------------

		// Inserts an element at the position specified, shifting all antecedent elements down one position.
		ETInlineHint Iterator	Insert( Iterator location, ConstReference itemTemplate );

		// Removes an element at the position specified, reducing the size of the @ref List by one element.
		ETInlineHint Iterator	Erase( Iterator location );
		// Removes a range of elements in the range [first, last), reducing the size of the @ref List by <<distance( first, last )>>
		ETInlineHint Iterator	Erase( Iterator first, Iterator last );

		// Removes the current contents of this @ref List and duplicates each element in the range [first, last) into the container.
		template <typename InputIterator>
		ETInlineHint void		Assign( InputIterator first, InputIterator last );

		// Removes all elements from the List.
		ETInlineHint void		Clear();

	// - CONTENT QUERY -----------------------------------

		// Returns a bool indicating whether or not there are any elements contained in this List.
		ETInlineHint bool		Empty() const;

		// Counts the number of elements contained within this List.
		ETInlineHint SizeType	Size() const;

	// - ALLOCATOR ACCESS --------------------------------

		ETInlineHint const AllocatorType&	GetAllocator() const;

		ETInlineHint void					SetAllocator( const AllocatorType& allocator );
	};

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/Containers/List.inl>
//------------------------------------------------------------------//