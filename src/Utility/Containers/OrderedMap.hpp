/*==================================================================*\
  OrderedMap.hpp
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
#include <rdestl/map.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename Key, typename StoredObject, class Allocator = ::Eldritch2::ChildAllocator>
	class OrderedMap {
	// - TYPE PUBLISHING ---------------------------------

	protected:
		typedef ::Eldritch2Detail::RDESTLAllocatorAdapterMixin<Allocator>	PrivateAllocator;
		typedef ::rde::map<Key, StoredObject, PrivateAllocator>			UnderlyingContainer;

	public:
		typedef typename UnderlyingContainer::value_type				ValueType;
		typedef typename UnderlyingContainer::key_type					KeyType;
		typedef typename UnderlyingContainer::data_type					MappedType;
		typedef Allocator												AllocatorType;
		typedef typename UnderlyingContainer::iterator					Iterator;
		typedef typename UnderlyingContainer::const_iterator			ConstIterator;
		typedef typename UnderlyingContainer::size_type					SizeType;
		typedef ::rde::pair<Iterator, bool>								InsertResult;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		// Constructs this OrderedMap instance.
		template <typename... AllocatorConstructorArguments>
		ETInlineHint OrderedMap( AllocatorConstructorArguments&&... allocatorConstructorArguments );
		// Constructs this OrderedMap instance.
		template <typename InputIterator, typename... AllocatorConstructorArguments>
		ETInlineHint OrderedMap( InputIterator begin, InputIterator end, AllocatorConstructorArguments&&... allocatorConstructorArguments );
		// Constructs this OrderedMap instance.
		template <class AlternateAllocator, typename... AllocatorConstructorArguments>
		ETInlineHint OrderedMap( const ::Eldritch2::OrderedMap<Key, StoredObject, AlternateAllocator>& containerTemplate, AllocatorConstructorArguments&&... allocatorConstructorArguments );

		// Destroys this OrderedMap instance.
		ETInlineHint ~OrderedMap();

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