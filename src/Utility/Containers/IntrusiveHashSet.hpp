/*==================================================================*\
  IntrusiveHashSet.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/ResizableArray.hpp>
//------------------------------------------------------------------//
#include <eastl/intrusive_hash_set.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename Value, typename HashPredicate, typename EqualityPredicate = std::equal_to<Value>, typename BucketAllocator = Eldritch2::ChildAllocator>
	class IntrusiveHashSet {
	// - TYPE PUBLISHING ---------------------------------

	protected:
		using UnderlyingContainer	= ::boost::intrusive::unordered_set<Value, ::boost::intrusive::hash<HashPredicate>, ::boost::intrusive::equal<EqualityPredicate>, ::boost::intrusive::compare_hash<true>>;

	public:
		using ValueType				= typename UnderlyingContainer::value_type;
		using Reference				= typename UnderlyingContainer::reference;
		using ConstReference		= typename UnderlyingContainer::const_reference;
		using SizeType				= typename UnderlyingContainer::size_type;
		using EqualityPredicate		= typename UnderlyingContainer::key_equal;
		using HashPredicate			= typename UnderlyingContainer::hasher;
		using Iterator				= typename UnderlyingContainer::iterator;
		using ConstIterator			= typename UnderlyingContainer::const_iterator;
		using InsertCommitData		= typename UnderlyingContainer::insert_commit_data;
		using BucketAllocatorType	= BucketAllocator;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	//! Constructs this @ref IntrusiveHashSet instance.
		ETInlineHint explicit IntrusiveHashSet( const SizeType initialBucketCount, AllocatorConstructorArguments&&... allocatorConstructorArguments );
	//! Constructs this @ref IntrusiveHashSet instance.
		ETInlineHint IntrusiveHashSet( const SizeType initialBucketCount, const HashPredicate& hasher, const EqualityPredicate& equalityPredicate, AllocatorConstructorArguments&&... allocatorConstructorArguments );

	//! Destroys this @ref IntrusiveHashSet instance.
		ETInlineHint ~IntrusiveHashSet();

	// - ALGORITHMS --------------------------------------

		template <typename Key, typename KeyHashPredicate, typename KeyEqualityPredicate>
		ETForceInlineHint Iterator		Find( const Key& key, KeyHashPredicate keyHashPredicate, KeyEqualityPredicate keyEqualityPredicate );
		template <typename Key, typename KeyHashPredicate, typename KeyEqualityPredicate>
		ETForceInlineHint ConstIterator	Find( const Key& key, KeyHashPredicate keyHashPredicate, KeyEqualityPredicate keyEqualityPredicate ) const;

		template <typename Key, typename KeyHashPredicate, typename KeyEqualityPredicate>
		ETForceInlineHint SizeType		RemoveIf( const Key& key, KeyHashPredicate keyHashPredicate, KeyEqualityPredicate keyEqualityPredicate );

		template<typename Key, typename KeyHashPredicate, typename KeyEqualityPredicate, typename Disposer>
		ETForceInlineHint SizeType		RemoveIfAndDispose( const Key& key, KeyHashPredicate keyHashPredicate, KeyEqualityPredicate keyEqualityPredicate, Disposer disposer );

	// - ELEMENT ITERATION -------------------------------

		ETForceInlineHint Iterator		Begin();
		ETForceInlineHint ConstIterator	Begin() const;

		ETForceInlineHint Iterator		End();
		ETForceInlineHint ConstIterator End() const;

		ETForceInlineHint ConstIterator ConstBegin() const;

		ETForceInlineHint ConstIterator ConstEnd() const;

	// - CONTAINER DUPLICATION ---------------------------

		ETForceInlineHint void	Swap( const Eldritch2::IntrusiveHashSet<Value, HashPredicate, EqualityPredicate, BucketAllocator>& containerTemplate );

		template <typename Cloner, typename Disposer>
		ETForceInlineHint void	CloneFrom( const Eldritch2::IntrusiveHashSet<Value, HashPredicate, EqualityPredicate, BucketAllocator>& containerTemplate, Cloner cloner, Disposer disposer );

	// - CONTAINER MANIPULATION --------------------------

		template <typename Key>
		ETForceInlineHintstd::pair<Iterator, bool>	InsertCheck( const Key& key, InsertCommitData& commitData );
		template <typename Key, typename KeyHashPredicate, typename KeyEqualityPredicate>
		ETForceInlineHintstd::pair<Iterator, bool>	InsertCheck( const Key& key, KeyHashPredicate keyHashPredicate, KeyEqualityPredicate keyEqualityPredicate, InsertCommitData& commitData );

		ETForceInlineHint std::pair<Iterator, bool>	Insert( Reference value );
		template <typename SourceIterator>
		ETForceInlineHint void							Insert( SourceIterator rangeBegin, SourceIterator rangeEnd );

		ETForceInlineHint Iterator	InsertCommit( Reference value, InsertCommitData& commitData );

		ETForceInlineHint void	Erase( ConstIterator position );
		ETForceInlineHint void	Erase( ConstIterator rangeBegin, ConstIterator rangeEnd );
		ETForceInlineHint void	Erase( ConstReference element );

		template <typename Disposer>
		ETForceInlineHint void	EraseAndDispose( ConstIterator position, Disposer disposer );
		template <typename Disposer>
		ETForceInlineHint void	EraseAndDispose( ConstIterator rangeBegin, ConstIterator rangeEnd, Disposer disposer );
		template <typename Disposer>
		ETForceInlineHint void	EraseAndDispose( ConstReference element, Disposer disposer );

		ETForceInlineHint void	Clear();

		template <typename Disposer>
		ETForceInlineHint void	ClearAndDispose( Disposer disposer );

	// - CONTENT QUERY -----------------------------------

		ETForceInlineHint bool	Empty() const;

		ETForceInlineHint SizeType	Size() const;

		ETForceInlineHint SizeType	Count( ConstReference value ) const;
		template <typename Key, typename KeyHashPredicate, typename KeyEqualityPredicate>
		ETForceInlineHint SizeType	Count( const Key& key, KeyHashPredicate keyHashPredicate, KeyEqualityPredicate keyEqualityPredicate ) const;

	// ---------------------------------------------------

		ETForceInlineHint SizeType	BucketCount() const;

		ETForceInlineHint SizeType	BucketSize( const SizeType bucketIndex ) const;

	// ---------------------------------------------------

		ETForceInlineHint void	Rehash( const SizeType newBucketCount );

	// ---------------------------------------------------

		ETForceInlineHint HashPredicate	GetHashPredicate() const;

		ETForceInlineHint EqualityPredicate	GetEqualityPredicate() const;

	// - DATA MEMBERS ------------------------------------

	private:
		BucketCollection	_buckets;
		UnderlyingContainer	_underlyingContainer;
	};

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/Containers/IntrusiveHashSet.inl>
//------------------------------------------------------------------//