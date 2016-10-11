/*==================================================================*\
  Map.hpp
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
#include <Utility/LessThan.hpp>
#include <Utility/Pair.hpp>
//------------------------------------------------------------------//
#if ET_COMPILER_IS_MSVC && !defined( EA_COMPILER_HAS_C99_FORMAT_MACROS )
//	MSVC complains about *lots* of macro redefinitions in eabase/inttypes.h.
#	define EA_COMPILER_HAS_C99_FORMAT_MACROS
#endif
#include <eastl/map.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename Key, typename Value, typename OrderingPredicate = Eldritch2::LessThan<Key>, class Allocator = Eldritch2::ChildAllocator>
	class Map {
	// - TYPE PUBLISHING ---------------------------------

	protected:
		using UnderlyingContainer	= eastl::map<Key, Value, OrderingPredicate, Detail::EaStlAllocatorAdapterMixin<Allocator>>;

	public:
		using ValueType				= typename UnderlyingContainer::value_type;
		using KeyType				= typename UnderlyingContainer::key_type;
		using MappedType			= typename UnderlyingContainer::data_type;
		using AllocatorType			= typename UnderlyingContainer::allocator_type::PublicType;
		using Iterator				= typename UnderlyingContainer::iterator;
		using ConstIterator			= typename UnderlyingContainer::const_iterator;
		using SizeType				= typename UnderlyingContainer::size_type;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref Map instance.
		explicit Map( const AllocatorType& allocator = AllocatorType() );
	//!	Constructs this @ref Map instance.
		template <typename InputIterator>
		Map( InputIterator begin, InputIterator end, const AllocatorType& allocator = AllocatorType() );
	//!	Constructs this @ref Map instance.
		template <class = eastl::enable_if<eastl::is_copy_constructible<ValueType>::value>::type>
		Map( const Map&, const AllocatorType& allocator = AllocatorType() );
	//!	Constructs this @ref Map instance.
		Map( Map&& );

		~Map() = default;

	// - ALGORITHMS --------------------------------------

	public:
		ConstIterator	Find( const KeyType& key ) const;
		Iterator		Find( const KeyType& key );

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

	// - ELEMENT ACCESS ----------------------------------

	public:
		template <class = eastl::is_default_constructible<ValueType>::value>
		MappedType&	operator[]( const KeyType& key );

	// - CONTAINER DUPLICATION ---------------------------

	public:
		template <class = eastl::enable_if<eastl::is_copy_constructible<ValueType>::value>::type>
		Map&	operator=( const Map& );
		Map&	operator=( Map&& );

		void	Swap( Map& other );

	// - CONTAINER MANIPULATION --------------------------

	public:
		template <typename... ConstructorArguments>
		Eldritch2::Pair<Iterator, bool>	Emplace( ConstructorArguments&&... constructorArguments );

		template <class = eastl::enable_if<eastl::is_copy_constructible<ValueType>::value>::type>
		Eldritch2::Pair<Iterator, bool>	Insert( const ValueType& value );
		Eldritch2::Pair<Iterator, bool>	Insert( ValueType&& value );

		Iterator						Erase( Iterator begin, Iterator end );
		Iterator						Erase( Iterator position );
		SizeType						Erase( const KeyType& key );

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
#include <Utility/Containers/Map.inl>
//------------------------------------------------------------------//