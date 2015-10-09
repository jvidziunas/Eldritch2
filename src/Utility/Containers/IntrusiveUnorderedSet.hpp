/*==================================================================*\
  IntrusiveUnorderedSet.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/ResizableArray.hpp>
//------------------------------------------------------------------//
#if( ET_COMPILER_IS_MSVC )
#	pragma warning( push )
// Disable warning about sizeof() operator; Boost uses this for some template wizardry.
#	pragma warning( disable : 6334 )
#endif
#include <boost/intrusive/unordered_set.hpp>
#if( ET_COMPILER_IS_MSVC )
#	pragma warning( pop )
#endif
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename StoredObject, typename HashPredicate, typename EqualityPredicate = ::std::equal_to<StoredObject>, typename BucketAllocator = ::Eldritch2::ChildAllocator>
	class IntrusiveUnorderedSet {
		// - TYPE PUBLISHING ---------------------------------

	protected:
		typedef ::boost::intrusive::unordered_set<StoredObject,
												  ::boost::intrusive::hash<HashPredicate>,
												  ::boost::intrusive::equal<EqualityPredicate>,
												  ::boost::intrusive::compare_hash<true>>	UnderlyingContainer;
		typedef typename UnderlyingContainer::value_traits									UnderlyingValueTraits;
		typedef ::Eldritch2::ResizableArray<typename ::boost::intrusive::unordered_bucket<UnderlyingValueTraits>::type, BucketAllocator>	BucketCollection;
		

	public:
		typedef typename UnderlyingContainer::value_type			ValueType;
		typedef typename UnderlyingContainer::reference				Reference;
		typedef typename UnderlyingContainer::const_reference		ConstReference;
		typedef typename UnderlyingContainer::size_type				SizeType;
		typedef typename UnderlyingContainer::key_equal				EqualityPredicate;
		typedef typename UnderlyingContainer::hasher				HashPredicate;
		typedef typename UnderlyingContainer::iterator				Iterator;
		typedef typename UnderlyingContainer::const_iterator		ConstIterator;
		typedef typename UnderlyingContainer::insert_commit_data	InsertCommitData;
		typedef BucketAllocator										BucketAllocatorType;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		// Constructs this IntrusiveUnorderedSet instance.
		template <typename... AllocatorConstructorArguments>
		ETInlineHint explicit IntrusiveUnorderedSet( const SizeType initialBucketCount, AllocatorConstructorArguments&&... allocatorConstructorArguments );
		// Constructs this IntrusiveUnorderedSet instance.
		template <typename... AllocatorConstructorArguments>
		ETInlineHint IntrusiveUnorderedSet( const SizeType initialBucketCount, const HashPredicate& hasher, const EqualityPredicate& equalityPredicate, AllocatorConstructorArguments&&... allocatorConstructorArguments );

		// Destroys this IntrusiveUnorderedSet instance.
		ETInlineHint ~IntrusiveUnorderedSet();

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

		ETForceInlineHint void	Swap( const ::Eldritch2::IntrusiveUnorderedSet<StoredObject, HashPredicate, EqualityPredicate, BucketAllocator>& containerTemplate );

		template <typename Cloner, typename Disposer>
		ETForceInlineHint void	CloneFrom( const ::Eldritch2::IntrusiveUnorderedSet<StoredObject, HashPredicate, EqualityPredicate, BucketAllocator>& containerTemplate, Cloner cloner, Disposer disposer );

	// - CONTAINER MANIPULATION --------------------------

		template <typename Key>
		ETForceInlineHint::std::pair<Iterator, bool>	InsertCheck( const Key& key, InsertCommitData& commitData );
		template <typename Key, typename KeyHashPredicate, typename KeyEqualityPredicate>
		ETForceInlineHint::std::pair<Iterator, bool>	InsertCheck( const Key& key, KeyHashPredicate keyHashPredicate, KeyEqualityPredicate keyEqualityPredicate, InsertCommitData& commitData );

		ETForceInlineHint ::std::pair<Iterator, bool>	Insert( Reference value );
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
#include <Utility/Containers/IntrusiveUnorderedSet.inl>
//------------------------------------------------------------------//