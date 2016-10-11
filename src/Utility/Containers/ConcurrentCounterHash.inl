/*==================================================================*\
  SmallConcurrentHash.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Algorithms.hpp>
#include <Utility/Assert.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename Key, typename Counter, class HashPredicate, class KeyEqualityPredicate, class Allocator>
	ConcurrentCounterHash<Key, Counter, HashPredicate, KeyEqualityPredicate, Allocator>::Node::Node( KeyType key, Counter counterValue ) : ValueType( key, counterValue ) {}

// ---------------------------------------------------

	template <typename Key, typename Counter, class HashPredicate, class KeyEqualityPredicate, class Allocator>
	ConcurrentCounterHash<Key, Counter, HashPredicate, KeyEqualityPredicate, Allocator>::Iterator::Iterator( NodeType* node, const HostContainerType* hostContainer ) : _node( node ), _hostContainer( hostContainer ) {}

// ---------------------------------------------------

	template <typename Key, typename Counter, class HashPredicate, class KeyEqualityPredicate, class Allocator>
	typename ConcurrentCounterHash<Key, Counter, HashPredicate, KeyEqualityPredicate, Allocator>::ValueType& ConcurrentCounterHash<Key, Counter, HashPredicate, KeyEqualityPredicate, Allocator>::Iterator::operator*() const {
		ETRuntimeAssert( _node != nullptr );
		return _node;
	}

// ---------------------------------------------------

	template <typename Key, typename Counter, class HashPredicate, class KeyEqualityPredicate, class Allocator>
	typename ConcurrentCounterHash<Key, Counter, HashPredicate, KeyEqualityPredicate, Allocator>::ValueType* ConcurrentCounterHash<Key, Counter, HashPredicate, KeyEqualityPredicate, Allocator>::Iterator::operator->() const {
		return &_node;
	}

// ---------------------------------------------------

	template <typename Key, typename Counter, class HashPredicate, class KeyEqualityPredicate, class Allocator>
	const typename ConcurrentCounterHash<Key, Counter, HashPredicate, KeyEqualityPredicate, Allocator>::Iterator::HostContainer* ConcurrentCounterHash<Key, Counter, HashPredicate, KeyEqualityPredicate, Allocator>::Iterator::GetContainer() const {
		return _hostContainer;
	}
	
// ---------------------------------------------------

	template <typename Key, typename Counter, class HashPredicate, class KeyEqualityPredicate, class Allocator>
	ETForceInlineHint typename ConcurrentCounterHash<Key, Counter, HashPredicate, KeyEqualityPredicate, Allocator>::ValueType* ConcurrentCounterHash<Key, Counter, HashPredicate, KeyEqualityPredicate, Allocator>::Iterator::GetNode() const {
		return _node;
	}

// ---------------------------------------------------

	template <typename Key, typename Counter, class HashPredicate, class KeyEqualityPredicate, class Allocator>
	typename ConcurrentCounterHash<Key, Counter, HashPredicate, KeyEqualityPredicate, Allocator>::Iterator& ConcurrentCounterHash<Key, Counter, HashPredicate, KeyEqualityPredicate, Allocator>::Iterator::operator++() {
		ETRuntimeAssert( _node != 0 );

		++_node;
		MoveToNextOccupiedNode();

		return *this;
	}

// ---------------------------------------------------

	template <typename Key, typename Counter, class HashPredicate, class KeyEqualityPredicate, class Allocator>
	typename ConcurrentCounterHash<Key, Counter, HashPredicate, KeyEqualityPredicate, Allocator>::Iterator ConcurrentCounterHash<Key, Counter, HashPredicate, KeyEqualityPredicate, Allocator>::Iterator::operator++( int ) {
		auto	copy( *this );

		++(*this);

		return copy;
	}

// ---------------------------------------------------

	template <typename Key, typename Counter, class HashPredicate, class KeyEqualityPredicate, class Allocator>
	ETForceInlineHint bool ConcurrentCounterHash<Key, Counter, HashPredicate, KeyEqualityPredicate, Allocator>::Iterator::operator==( const Iterator& rhs ) const {
		return rhs.m_node == _node;
	}

// ---------------------------------------------------

	template <typename Key, typename Counter, class HashPredicate, class KeyEqualityPredicate, class Allocator>
	bool ConcurrentCounterHash<Key, Counter, HashPredicate, KeyEqualityPredicate, Allocator>::Iterator::operator!=( const Iterator& rhs ) const {
		return !(rhs == *this);
	}

// ---------------------------------------------------

	template <typename Key, typename Counter, class HashPredicate, class KeyEqualityPredicate, class Allocator>
	void ConcurrentCounterHash<Key, Counter, HashPredicate, KeyEqualityPredicate, Allocator>::Iterator::MoveToNextOccupiedNode() {
		_node = Eldritch2::Find( _node, _hostContainer->_nodesEnd, [] ( const NodeType& node ) {
			return node.first != HostContainerType::KeyType();
		} );
	}

// ---------------------------------------------------

	template <typename Key, typename Counter, class HashPredicate, class KeyEqualityPredicate, class Allocator>
	template <typename InputIterator>
	ConcurrentCounterHash<Key, Counter, HashPredicate, KeyEqualityPredicate, Allocator>::ConcurrentCounterHash( InputIterator firstElement, InputIterator endElement, AllocatorType&& allocator ) : ConcurrentCounterHash( firstElement, endElement, HashPredicate(), KeyEqualityComparatorType(), eastl::move( allocator ) ) {}

// ---------------------------------------------------

	template <typename Key, typename Counter, class HashPredicate, class KeyEqualityPredicate, class Allocator>
	template <typename InputIterator>
	ConcurrentCounterHash<Key, Counter, HashPredicate, KeyEqualityPredicate, Allocator>::ConcurrentCounterHash( InputIterator firstElement,
																										  InputIterator endElement,
																										  HashType&& hasher,
																										  KeyEqualityComparatorType&& keyEqualityComparator,
																										  AllocatorType&& allocator ) : _allocator( eastl::move( allocator ) ),
																										  								_hasher( eastl::move( hasher ) ),
																										  								_keyEqualityComparator( keyEqualityComparator ) {
		const auto	capacity( std::distance( firstElement, endElement ) );

		_nodes		= _allocator.Allocate( sizeof(Node) * capacity );
		_nodesEnd	= _nodes + capacity;

		for( auto* node( _nodes ); firstElement != endElement; ++node, ++firstElement ) {
			new(node) ValueType( firstElement.first, MappedType() );
		}
	}

// ---------------------------------------------------

	template <typename Key, typename Counter, class HashPredicate, class KeyEqualityPredicate, class Allocator>
	ConcurrentCounterHash<Key, Counter, HashPredicate, KeyEqualityPredicate, Allocator>::ConcurrentCounterHash<Key, Counter, HashPredicate, KeyEqualityPredicate, Allocator>::~ConcurrentCounterHash() {
		Eldritch2::DestroyRange( _nodes, _nodesEnd );

		_allocator.Deallocate( _nodes );
	}

}	// namespace Eldritch2