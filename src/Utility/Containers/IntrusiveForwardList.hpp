/*==================================================================*\
  IntrusiveForwardList.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/IntrusiveForwardListHook.hpp>
#include <Utility/Containers/RangeAdapters.hpp>
#include <Utility/MPL/Compiler.hpp>
//------------------------------------------------------------------//
#if ET_COMPILER_IS_MSVC && !defined( EA_COMPILER_HAS_C99_FORMAT_MACROS )
//	MSVC complains about *lots* of macro redefinitions in eabase/inttypes.h.
#	define EA_COMPILER_HAS_C99_FORMAT_MACROS
#endif
#include <eastl/bonus/intrusive_slist.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <class Value>
	class IntrusiveForwardList {
	// - TYPE PUBLISHING ---------------------------------

	private:
		using UnderlyingContainer	= eastl::intrusive_slist<Value>;

	public:
		using ValueType				= typename UnderlyingContainer::value_type;
		using Reference				= typename UnderlyingContainer::reference;
		using ConstReference		= typename UnderlyingContainer::const_reference;
		using Iterator				= typename UnderlyingContainer::iterator;
		using ConstIterator			= typename UnderlyingContainer::const_iterator;
		using SizeType				= typename UnderlyingContainer::size_type;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//! Constructs this @ref IntrusiveForwardList instance.
		template <typename SourceIterator>
		IntrusiveForwardList( SourceIterator elementBegin, SourceIterator elementEnd );
	//! Constructs this @ref IntrusiveForwardList instance.
		IntrusiveForwardList( IntrusiveForwardList&& );
	//! Constructs this @ref IntrusiveForwardList instance.
		IntrusiveForwardList( const IntrusiveForwardList& ) = delete;
	//! Constructs this @ref IntrusiveForwardList instance.
		IntrusiveForwardList() = default;

		~IntrusiveForwardList() = default;

	// - ALGORITHMS --------------------------------------

	public:
		template <typename Predicate>
		ConstIterator	Find( Predicate predicate, ConstIterator searchHint ) const;
		template <typename Predicate>
		ConstIterator	Find( Predicate predicate ) const;
		template <typename Predicate>
		Iterator		Find( Predicate predicate, Iterator searchHint );
		template <typename Predicate>
		Iterator		Find( Predicate predicate );

		template <typename Predicate>
		void			RemoveIf( Predicate predicate );

		template <typename Predicate, typename Disposer>
		void			RemoveAndDisposeIf( Predicate predicate, Disposer disposer );

		template <typename Predicate>
		void			Sort( Predicate predicate );

	// - ELEMENT ITERATION -------------------------------
		
	public:
		ConstIterator	ConstBegin() const;
		ConstIterator	ConstEnd() const;

		ConstIterator	Begin() const;
		Iterator		Begin();

		ConstIterator	End() const;
		Iterator		End();
		
		ConstIterator	IteratorTo( ConstReference element ) const;
		Iterator		IteratorTo( Reference element );

	// - END POINT MANIPULATION --------------------------

	public:
	//! Retrieves a reference to the first element stored in this @ref IntrusiveForwardList.
		ConstReference	Front() const;
	//! Retrieves a reference to the first element stored in this @ref IntrusiveForwardList.
		Reference		Front();

	//! Adds the passed-in item to the head of this @ref IntrusiveForwardList.
		void			PushFront( Reference item );

	//! Removes the head element of this @ref IntrusiveForwardList, reducing its size by one element.
		template <typename Disposer>
		void			PopFrontAndDispose( Disposer disposer );

	// - CONTAINER DUPLICATION ---------------------------

	public:
		IntrusiveForwardList&	operator=( IntrusiveForwardList&& );
		IntrusiveForwardList&	operator=( const IntrusiveForwardList& ) = delete;

		void					Swap( IntrusiveForwardList& other );

	// - CONTAINER MANIPULATION --------------------------

	public:
	//! Inserts an element, shifting all antecedent elements down one position.
		Iterator	Insert( Iterator location, Reference item );

		Iterator	InsertAfter( Iterator location, Reference item );

		Iterator	Erase( Iterator begin, Iterator end );
		Iterator	Erase( Iterator position );

		Iterator	EraseAfter( Iterator beforeBegin, Iterator end );
		Iterator	EraseAfter( Iterator position );

		template <typename Disposer>
		void		EraseAndDispose( Iterator begin, Iterator end, Disposer disposer );

		template <typename Disposer>
		Iterator	EraseAndDispose( Iterator position, Disposer disposer );

		void		Clear();

		template <typename Disposer>
		void		ClearAndDispose( Disposer disposer );

	// - CONTENT QUERY -----------------------------------

	public:
		SizeType			GetSize() const;

		bool				IsEmpty() const;

		explicit operator	bool() const;

	// - DATA MEMBERS ------------------------------------

	private:
		UnderlyingContainer	_underlyingContainer;
	};

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/Containers/IntrusiveForwardList.inl>
//------------------------------------------------------------------//