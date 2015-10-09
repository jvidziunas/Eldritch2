/*==================================================================*\
  IntrusiveUnorderedMultiset.hpp
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
//------------------------------------------------------------------//

namespace Eldritch2 {
	template <typename Iterator>
	class	Range;
}

namespace Eldritch2 {

	template <typename StoredObject, typename CollectionOptions, typename HashPredicate, typename EqualityPredicate = ::std::equal_to<StoredObject>, typename BucketAllocator = ::Eldritch2::ChildAllocator>
	class IntrusiveUnorderedMultiset {
	// - TYPE PUBLISHING ---------------------------------

	protected:
		typedef ::Eldritch2::ResizableArray<typename ::boost::intrusive::unordered_bucket<CollectionOptions>::type, BucketAllocator>	BucketCollection;
		typedef ::boost::intrusive::unordered_multiset<StoredObject,
													   CollectionOptions,
													   ::boost::intrusive::hash<HashPredicate>,
													   ::boost::intrusive::equal<EqualityPredicate>,
													   ::boost::intrusive::compare_hash<true>>			UnderlyingContainer;

	public:
		typedef typename UnderlyingContainer::value_type		ValueType;
		typedef typename UnderlyingContainer::reference			Reference;
		typedef typename UnderlyingContainer::const_reference	ConstReference;
		typedef typename UnderlyingContainer::size_type			SizeType;
		typedef typename UnderlyingContainer::key_equal			EqualityPredicate;
		typedef typename UnderlyingContainer::hasher			HashPredicate;
		typedef typename UnderlyingContainer::iterator			Iterator;
		typedef typename UnderlyingContainer::const_iterator	ConstIterator;
		typedef BucketAllocator									BucketAllocatorType;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		// Constructs this IntrusiveUnorderedMultiset instance.
		template <typename... AllocatorConstructorArguments>
		ETInlineHint explicit IntrusiveUnorderedMultiset( const SizeType initialBucketCount, AllocatorConstructorArguments&&... allocatorConstructorArguments );
		// Constructs this IntrusiveUnorderedMultiset instance.
		template <typename... AllocatorConstructorArguments>
		ETInlineHint IntrusiveUnorderedMultiset( const SizeType initialBucketCount, const HashPredicate& hasher, const EqualityPredicate& equalityPredicate, AllocatorConstructorArguments&&... allocatorConstructorArguments );

		// Destroys this IntrusiveUnorderedMultiset instance.
		ETInlineHint ~IntrusiveUnorderedMultiset();

	// - ALGORITHMS --------------------------------------

		::Eldritch2::Range<Iterator>		EqualRange( ConstReference value );
		::Eldritch2::Range<ConstIterator>	EqualRange( ConstReference value ) const;

		template <typename Key, typename KeyHashPredicate, typename KeyEqualityPredicate>
		::Eldritch2::Range<Iterator>		EqualRange( const Key& key, KeyHashPredicate keyHashPredicate, KeyEqualityPredicate keyEqualityPredicate );
		template <typename Key, typename KeyHashPredicate, typename KeyEqualityPredicate>
		::Eldritch2::Range<ConstIterator>	EqualRange( const Key& key, KeyHashPredicate keyHashPredicate, KeyEqualityPredicate keyEqualityPredicate ) const;
		template <typename Key>
		::Eldritch2::Range<Iterator>		EqualRange( const Key& key );
		template <typename Key>
		::Eldritch2::Range<ConstIterator>	EqualRange( const Key& key ) const;

		template <typename Key, typename KeyHashPredicate, typename KeyEqualityPredicate>
		ETForceInlineHint SizeType	RemoveIf( const Key& key, KeyHashPredicate keyHashPredicate, KeyEqualityPredicate keyEqualityPredicate );

		template<typename Key, typename KeyHashPredicate, typename KeyEqualityPredicate, typename Disposer>
		ETForceInlineHint SizeType	RemoveIfAndDispose( const Key& key, KeyHashPredicate keyHashPredicate, KeyEqualityPredicate keyEqualityPredicate, Disposer disposer );

	// - ELEMENT ITERATION -------------------------------

		ETForceInlineHint Iterator		Begin();
		ETForceInlineHint ConstIterator	Begin() const;
		
		ETForceInlineHint Iterator		End();
		ETForceInlineHint ConstIterator End() const;

		ETForceInlineHint ConstIterator ConstBegin() const;

		ETForceInlineHint ConstIterator ConstEnd() const;

	// - CONTAINER DUPLICATION ---------------------------

		template <class AlternateAllocator, typename Cloner, typename Disposer>
		ETForceInlineHint void	CloneFrom( const ::Eldritch2::IntrusiveUnorderedMultiset<StoredObject, CollectionOptions, HashPredicate, EqualityPredicate, AlternateAllocator>& containerTemplate, Cloner cloner, Disposer disposer );

	// - CONTAINER MANIPULATION --------------------------

		ETForceInlineHint Iterator	Insert( Reference value );
		template <typename SourceIterator>
		ETForceInlineHint void		Insert( SourceIterator rangeBegin, SourceIterator rangeEnd );

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
#include <Utility/Containers/IntrusiveUnorderedMultiset.inl>
//------------------------------------------------------------------//

#if( ET_COMPILER_IS_MSVC )
#	pragma warning( pop )
#endif