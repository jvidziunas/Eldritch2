/*==================================================================*\
  OrderedMap.hpp
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
#include <Utility/Pair.hpp>
//------------------------------------------------------------------//
#include <rdestl/map.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename Key, typename StoredObject, class Allocator = ::Eldritch2::ChildAllocator>
	class OrderedMap {
	// - TYPE PUBLISHING ---------------------------------

	protected:
		using PrivateAllocator		= Detail::RDESTLAllocatorAdapterMixin<Allocator>;
		using UnderlyingContainer	= ::rde::map<Key, StoredObject, PrivateAllocator>;

	public:
		using ValueType				= typename UnderlyingContainer::value_type;
		using KeyType				= typename UnderlyingContainer::key_type;
		using MappedType			= typename UnderlyingContainer::data_type;
		using AllocatorType			= Allocator;
		using Iterator				= typename UnderlyingContainer::iterator;
		using ConstIterator			= typename UnderlyingContainer::const_iterator;
		using SizeType				= typename UnderlyingContainer::size_type;
		using InsertResult			= ::Eldritch2::Pair<Iterator, bool>;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		//!	Constructs this @ref OrderedMap instance.
		ETInlineHint explicit OrderedMap( AllocatorType&& allocator = AllocatorType() );
		//!	Constructs this @ref OrderedMap instance.
		template <typename InputIterator>
		ETInlineHint OrderedMap( InputIterator begin, InputIterator end, AllocatorType&& allocator = AllocatorType() );
		//!	Constructs this @ref OrderedMap instance.
		template <class AlternateAllocator>
		ETInlineHint OrderedMap( const ::Eldritch2::OrderedMap<Key, StoredObject, AlternateAllocator>& containerTemplate, AllocatorType&& allocator = AllocatorType() );
		//!	Constructs this @ref OrderedMap instance.
		ETInlineHint OrderedMap( ::Eldritch2::OrderedMap<Key, StoredObject, Allocator>&& moveSource );

		ETInlineHint ~OrderedMap() = default;

	// - ALGORITHMS --------------------------------------

		ETInlineHint Iterator		Find( const KeyType& key );
		ETInlineHint ConstIterator	Find( const KeyType& key ) const;

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

	// - ELEMENT ACCESS ----------------------------------

		ETInlineHint MappedType&	operator[]( const KeyType& key );

	// - CONTAINER DUPLICATION ---------------------------

		ETInlineHint void	CloneFrom( const ::Eldritch2::OrderedMap<Key, StoredObject, Allocator>& containerTemplate );

		ETInlineHint void	Swap( ::Eldritch2::OrderedMap<Key, StoredObject, Allocator>& other );

	// - CONTAINER MANIPULATION --------------------------

		ETInlineHint InsertResult	Insert( const ValueType& v );
		ETInlineHint InsertResult	Insert( const KeyType& key, const MappedType& value );

		ETInlineHint SizeType	Erase( const KeyType& key );
		ETInlineHint void		Erase( Iterator position );
		ETInlineHint void		Erase( Iterator begin, Iterator end );

		ETInlineHint void	Clear();

	// - CONTENT QUERY -----------------------------------

		ETInlineHint SizeType	Size() const;

		ETInlineHint bool		Empty() const;

		ETInlineHint			operator bool() const;

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
#include <Utility/Containers/OrderedMap.inl>
//------------------------------------------------------------------//