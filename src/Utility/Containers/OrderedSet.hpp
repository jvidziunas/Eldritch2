/*==================================================================*\
  OrderedSet.hpp
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
#include <rdestl/set.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename StoredObject, class Allocator = ::Eldritch2::ChildAllocator>
	class OrderedSet {
	// - TYPE PUBLISHING ---------------------------------

	protected:
		typedef ::Eldritch2Detail::RDESTLAllocatorAdapterMixin<Allocator>	PrivateAllocator;
		typedef ::rde::set<StoredObject, PrivateAllocator>				UnderlyingContainer;

	public:
		typedef typename UnderlyingContainer::value_type				ValueType;
		typedef Allocator												AllocatorType;
		typedef typename UnderlyingContainer::iterator					Iterator;
		typedef typename UnderlyingContainer::const_iterator			ConstIterator;
		typedef ::rde::pair<Iterator, bool>								InsertResult;
		typedef typename AllocatorType::SizeType						SizeType;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		// Constructs this OrderedSet instance.
		template <typename... AllocatorConstructorArguments>
		ETInlineHint OrderedSet( AllocatorConstructorArguments&&... allocatorConstructorArguments );
		// Constructs this OrderedSet instance.
		template <typename InputIterator, typename... AllocatorConstructorArguments>
		ETInlineHint OrderedSet( InputIterator begin, InputIterator end, AllocatorConstructorArguments&&... allocatorConstructorArguments );
		// Constructs this OrderedSet instance.
		template <class AlternateAllocator, typename... AllocatorConstructorArguments>
		ETInlineHint OrderedSet( const ::Eldritch2::OrderedSet<StoredObject, AlternateAllocator>& containerTemplate, AllocatorConstructorArguments&&... allocatorConstructorArguments );

		// Destroys this OrderedSet instance.
		ETInlineHint ~OrderedSet();

	// - ALGORITHMS --------------------------------------

		ETInlineHint Iterator		Find( const ValueType& itemTemplate );
		ETInlineHint ConstIterator	Find( const ValueType& itemTemplate ) const;

		template <typename Predicate>
		ETInlineHint void			RemoveIf( Predicate predicate );
		template <typename ExtraArgumentType, typename Predicate>
		ETInlineHint void			RemoveIf( ExtraArgumentType extraArgument, Predicate predicate );

	// - ELEMENT ITERATION -------------------------------

		ETInlineHint Iterator		Begin();
		ETInlineHint ConstIterator	Begin() const;

		ETInlineHint ConstIterator	ConstBegin() const;

		ETInlineHint Iterator		End();
		ETInlineHint ConstIterator	End() const;

		ETInlineHint ConstIterator	ConstEnd() const;

	// - CONTAINER DUPLICATION ---------------------------

		ETInlineHint void	CloneFrom( const ::Eldritch2::OrderedSet<StoredObject, Allocator>& containerTemplate );

		ETInlineHint void	Swap( ::Eldritch2::OrderedSet<StoredObject, Allocator>& other );

	// - CONTAINER MANIPULATION --------------------------

		ETInlineHint InsertResult	Insert( const ValueType& v );

		ETInlineHint void	Erase( Iterator position );
		ETInlineHint void	Erase( Iterator begin, Iterator end );

		ETInlineHint void	Clear();

	// - CONTENT QUERY -----------------------------------

		ETInlineHint SizeType	Size() const;

		ETInlineHint bool	Empty() const;

		ETInlineHint	operator bool() const;

	// - ALLOCATOR ACCESS --------------------------------

		ETInlineHint const AllocatorType&	GetAllocator() const;

	// - DATA MEMBERS ------------------------------------

	private:
		UnderlyingContainer	_underlyingContainer;
	};

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/Containers/OrderedSet.inl>
//------------------------------------------------------------------//