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
#include <Utility/Memory/RDESTLAllocatorAdapterMixin.hpp>
#include <Utility/Memory/ChildAllocator.hpp>
//------------------------------------------------------------------//
#include <rdestl/set.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename StoredObject, class Allocator = ::Eldritch2::ChildAllocator>
	class Set {
	// - TYPE PUBLISHING ---------------------------------

	protected:
		using PrivateAllocator		= Detail::RDESTLAllocatorAdapterMixin<Allocator>;
		using UnderlyingContainer	= ::rde::set<StoredObject, PrivateAllocator>;

	public:
		using ValueType				= typename UnderlyingContainer::value_type;
		using AllocatorType			= Allocator;
		using Iterator				= typename UnderlyingContainer::iterator;
		using ConstIterator			= typename UnderlyingContainer::const_iterator;
		using InsertResult			= ::Eldritch2::Pair<Iterator, bool>;
		using SizeType				= typename AllocatorType::SizeType;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		//!	Constructs this @ref Set instance.
		ETInlineHint explicit Set( AllocatorType&& allocator = AllocatorType() );
		//!	Constructs this @ref Set instance.
		template <typename InputIterator>
		ETInlineHint Set( InputIterator begin, InputIterator end, AllocatorType&& allocator = AllocatorType() );
		//!	Constructs this @ref Set instance.
		template <class AlternateAllocator>
		ETInlineHint Set( const ::Eldritch2::Set<StoredObject, AlternateAllocator>& containerTemplate, AllocatorType&& allocator = AllocatorType() );
		//!	Constructs this @ref Set instance.
		ETInlineHint Set( ::Eldritch2::Set<StoredObject, Allocator>&& moveSource );

		ETInlineHint ~Set() = default;

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

		ETInlineHint void	CloneFrom( const ::Eldritch2::Set<StoredObject, Allocator>& containerTemplate );

		ETInlineHint void	Swap( ::Eldritch2::Set<StoredObject, Allocator>& other );

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
#include <Utility/Containers/Set.inl>
//------------------------------------------------------------------//