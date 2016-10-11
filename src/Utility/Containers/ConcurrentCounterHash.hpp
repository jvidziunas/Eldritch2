/*==================================================================*\
  CounterHash.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Memory/ChildAllocator.hpp>
#include <Utility/EqualTo.hpp>
#include <Utility/Pair.hpp>
#include <Utility/Hash.hpp>
//------------------------------------------------------------------//
#include <atomic>
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename Key, typename Counter, class HashPredicate = Eldritch2::Hash<Key>, class KeyEqualityPredicate = Eldritch2::EqualTo<Key>, class Allocator = Eldritch2::ChildAllocator>
	class ConcurrentCounterHash {
	// - TYPE PUBLISHING ---------------------------------

	public:
		using KeyType					= Key;
		using MappedType				= std::atomic<Counter>;
		using ValueType					= Eldritch2::Pair<KeyType, MappedType>;
		using HashType					= HashPredicate;
		using KeyEqualityComparatorType = KeyEqualityPredicate;
		using AllocatorType				= Allocator;

	public:
		class Iterator {
		// - TYPE PUBLISHING ---------------------------------

		public:
			using iterator_category		= eastl::forward_iterator_tag;
			using HostContainerType		= Eldritch2::ConcurrentCounterHash<Key, Counter, HashPredicate, KeyEqualityPredicate, Allocator>;
			using NodeType				= HostContainerType::ValueType;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		//!	Constructs this @ref Iterator instance.
			explicit Iterator( NodeType* node, const HostContainerType* container );
		//!	Constructs this @ref Iterator instance.
			Iterator( const Iterator& ) = default;

			~Iterator() = default;

		// ---------------------------------------------------

		public:
			NodeType&	operator*() const;

			NodeType*	operator->() const;

		// ---------------------------------------------------

		public:
			const HostContainerType*	GetContainer() const;

			NodeType*					GetNode() const;

		// ---------------------------------------------------
			
		public:
			Iterator&	operator++();
			Iterator	operator++(int);

		// ---------------------------------------------------

		public:
			bool	operator==( const Iterator& rhs );

			bool	operator!=( const Iterator& rhs ) const;

		// ---------------------------------------------------

		private:
			void	MoveToNextOccupiedNode();

		// - DATA MEMBERS ------------------------------------

			NodeType*					_node;
			const HostContainerType*	_hostContainer;

		// - FRIEND CLASS DECLARATION ------------------------

			friend class Eldritch2::ConcurrentCounterHash<Key, Counter, HashPredicate, KeyEqualityPredicate, Allocator>;
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref ConcurrentCounterHash instance.
		template <typename InputIterator>
		ConcurrentCounterHash( InputIterator firstElement, InputIterator endElement, HashType&& hasher, KeyEqualityComparatorType&& keyEqualityComparator, AllocatorType&& allocator );
	//!	Constructs this @ref ConcurrentCounterHash instance.
		template <typename InputIterator>
		ConcurrentCounterHash( InputIterator firstElement, InputIterator endElement, AllocatorType&& allocator );

		~ConcurrentCounterHash();

	// ---------------------------------------------------

	public:
		Iterator	Begin();

		Iterator	End();

	// ---------------------------------------------------

	public:
		MappedType&	operator[]( KeyType key );

	// - DATA MEMBERS ------------------------------------

	private:
		AllocatorType				_allocator;
		HashType					_hasher;
		KeyEqualityComparatorType	_keyEqualityComparator;
		ValueType*					_nodes;
		ValueType*					_nodesEnd;
	};

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/Containers/ConcurrentCounterHash.inl>
//------------------------------------------------------------------//