/*==================================================================*\
  ResizableArray.hpp
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
#include <Utility/Containers/Range.hpp>
//------------------------------------------------------------------//
#include <rdestl/vector.h>
//------------------------------------------------------------------//
#include <iterator>
//------------------------------------------------------------------//

namespace Eldritch2Detail {

	struct ResizableArrayBase {
		struct	UnorderedSemantics {};
		struct	ReleaseMemorySemantics {};
	};

}	// namespace Eldritch2Detail

namespace Eldritch2 {

	template <typename StoredObject, typename Allocator = ::Eldritch2::ChildAllocator>
	class ResizableArray : public ::Eldritch2Detail::ResizableArrayBase {
	// - TYPE PUBLISHING ---------------------------------

	protected:
		typedef ::Eldritch2Detail::RDESTLAllocatorAdapterMixin<Allocator>	PrivateAllocator;
		typedef ::rde::vector<StoredObject, PrivateAllocator>			UnderlyingContainer;

	public:
		typedef typename UnderlyingContainer::value_type		ValueType;
		typedef ValueType*										Pointer;
		typedef const ValueType*								ConstPointer;
		typedef ValueType&										Reference;
		typedef const ValueType&								ConstReference;
		typedef Allocator										AllocatorType;
		typedef typename UnderlyingContainer::iterator			Iterator;
		typedef typename UnderlyingContainer::const_iterator	ConstIterator;
		typedef ::std::reverse_iterator<Iterator>				ReverseIterator;
		typedef ::std::reverse_iterator<ConstIterator>			ReverseConstIterator;
		typedef typename UnderlyingContainer::size_type			SizeType;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		//! Constructs this @ref ResizableArray instance.
		/*!	@param[in] initialSize Initial capacity hint, in elements.
			@param[in] allocatorConstructorArguments Template parameter pack consisting of all arguments to be forwarded to the allocator constructor.
			*/
		template <typename... AllocatorConstructorArguments>
		ETInlineHint explicit ResizableArray( const SizeType initialSize, AllocatorConstructorArguments&&... allocatorConstructorArguments );
		//! Constructs this @ref ResizableArray instance.
		/*!	@param[in] first Pointer to the beginning element in a continuous array of objects the container will copy into itself.
			@param[in] last Pointer to one past the end of the array headed by the _first_ parameter. May be equal to _first_.
			@param[in] allocatorConstructorArguments Template parameter pack consisting of all arguments to be forwarded to the allocator constructor.
			*/
		template <typename... AllocatorConstructorArguments>
		ETInlineHint ResizableArray( const StoredObject* first, const StoredObject* last, AllocatorConstructorArguments&&... allocatorConstructorArguments );
		//! Constructs this @ref ResizableArray instance.
		/*!	@param[in] containerTemplate Reference to a ResizableArray whose elements will be copy constructed into the container.
			@param[in] allocatorConstructorArguments Template parameter pack consisting of all arguments to be forwarded to the allocator constructor.
			*/
		template <class AlternateAllocator, typename... AllocatorConstructorArguments>
		ETInlineHint ResizableArray( const ::Eldritch2::ResizableArray<StoredObject, AlternateAllocator>& containerTemplate, AllocatorConstructorArguments&&... allocatorConstructorArguments );

		//! Constructs this @ref ResizableArray instance.
		ETInlineHint ResizableArray();

		//! Destroys this ResizableArray instance.
		ETInlineHint ~ResizableArray() = default;

	// - ALGORITHMS --------------------------------------

		//! Locates the first element in the container that compares equal to the passed-in reference.
		/*!	@param[in] itemTemplate The item to search for.
			@param[in] searchHint Optional offset in the container to begin the search. Defaults to 0, or the first element in the container.
			@returns If there is at least one element stored in the container equal to _itemTemplate_, then the function returns an iterator to the first such element. Otherwise, it returns the end iterator for the container.
			*/
		ETInlineHint SizeType		IndexOf( ConstReference itemTemplate, const SizeType searchHint = static_cast<SizeType>(0) ) const;

		//! Locates the first element in the container that compares equal to the passed-in reference.
		/*!	@param[in] itemTemplate The item to search for.
			@returns If there is at least one element stored in the container equal to _itemTemplate_, then the function returns an iterator to the first such element. Otherwise, it returns the end iterator for the container.
			*/
		ETInlineHint Iterator		Find( ConstReference itemTemplate );
		//! Locates the first element in the container that compares equal to the passed-in reference.
		/*!	@param[in] itemTemplate The item to search for.
			@returns If there is at least one element stored in the container equal to _itemTemplate_, then the function returns an iterator to the first such element. Otherwise, it returns the end iterator for the container.
			*/
		ETInlineHint ConstIterator	Find( ConstReference itemTemplate ) const;
		//! Locates the first element in the container that compares equal to the passed-in reference.
		/*!	@param[in] itemTemplate The item to search for.
			@param[in] searchHint Iterator to the first element in the container to compare. The iterator must belong to this container.
			@returns If there is at least one element stored in the container equal to _itemTemplate_, then the function returns an iterator to the first such element. Otherwise, it returns the end iterator for the container.
			*/
		ETInlineHint Iterator		Find( ConstReference itemTemplate, Iterator searchHint );
		//! Locates the first element in the container that compares equal to the passed-in reference.
		/*!	@param[in] itemTemplate The item to search for.
			@param[in] searchHint Iterator to the first element in the container to compare. The iterator must belong to this container.
			@returns If there is at least one element stored in the container equal to _itemTemplate_, then the function returns an iterator to the first such element. Otherwise, it returns the end iterator for the container.
			*/
		ETInlineHint ConstIterator	Find( ConstReference itemTemplate, ConstIterator searchHint ) const;

		//! Examines the elements in the ResizableArray to determine if any are equivalent to the argument.
		/*!	@param[in] itemTemplate The item to search for.
			@returns *true* if an item equal to the template was found, *false* if none exist in the container.
			*/
		ETInlineHint bool	Contains( ConstReference itemTemplate ) const;

		//! Rearranges the elements within this ResizeableArray such that all that satisfy the given predicate are situated before any that do not.
		/*!	@remark Note that there are no guarantees placed on ordering beyond the passed/did not pass distinction.
			@param[in] predicate The filter predicate to be run. This method uses operator() on the predicate, which should take a single argument of type ConstReference and return a bool indicating if the element passed the check.
			@returns An iterator to the beginning of the now-segregated range, or the end iterator for the container if all elements passed the check.
			*/
		template <typename Predicate>
		ETInlineHint Iterator	RemoveIf( Predicate predicate );

		//! Rearranges the elements within this ResizeableArray according to a customizable ordering.
		/*!	@param[in] predicate The ordering predicate to use. This method invokes operator() on the predicate, which should take two arguments of type ConstReference and return a book indicating if the element in the first argument should be placed before the element in the second.
			*/
		template <typename ComparisonPredicate>
		ETInlineHint void	Sort( ComparisonPredicate predicate );
		
	// - CONTAINER DUPLICATION ---------------------------

		ETInlineHint ::Eldritch2::ResizableArray<StoredObject, Allocator>&	operator=( const ::Eldritch2::ResizableArray<StoredObject, Allocator>& containerTemplate );
	    
		ETInlineHint void	CloneFrom( const ::Eldritch2::ResizableArray<StoredObject, Allocator>& containerTemplate );

		ETInlineHint void	Swap( ::Eldritch2::ResizableArray<StoredObject, Allocator>& container );

	// - ELEMENT ITERATION -------------------------------

		ETInlineHint Iterator		Begin();
		ETInlineHint ConstIterator	Begin() const;

		ETInlineHint Iterator		End();
		ETInlineHint ConstIterator	End() const;

		ETInlineHint ConstIterator	ConstBegin() const;

		ETInlineHint ConstIterator	ConstEnd() const;

		ETInlineHint ReverseIterator		ReverseBegin();
		ETInlineHint ReverseConstIterator	ReverseBegin() const;

		ETInlineHint ReverseIterator		ReverseEnd();
		ETInlineHint ReverseConstIterator	ReverseEnd() const;

	// - END POINT MANIPULATION --------------------------

		ETInlineHint Reference		Front();
		ETInlineHint ConstReference	Front() const;

		ETInlineHint Reference		Back();
		ETInlineHint ConstReference	Back() const;

		ETInlineHint void	PushBack( ConstReference itemTemplate );
		ETInlineHint void	PushBack( ValueType&& itemTemplate );

		template <typename... ElementConstructorArguments>
		ETInlineHint void	EmplaceBack( ElementConstructorArguments&&... elementConstructorArguments );

		ETInlineHint void	PopBack();

	// - ELEMENT ACCESS ----------------------------------

		ETInlineHint Reference		operator[]( const SizeType elementIndex );
		ETInlineHint ConstReference	operator[]( const SizeType elementIndex ) const;

		ETInlineHint Reference		At( const SizeType elementIndex );
		ETInlineHint ConstReference	At( const SizeType elementIndex ) const;

		ETInlineHint Pointer		Data();
		ETInlineHint ConstPointer	Data() const;

	// - CONTAINER MANIPULATION --------------------------

		ETInlineHint void	Assign( ConstPointer first, ConstPointer last );

		ETInlineHint void		Insert( const SizeType initialIndex, const SizeType itemCount, ConstReference itemTemplate );
		ETInlineHint void		Insert( Iterator position, const SizeType itemCount, ConstReference itemTemplate );
		ETInlineHint Iterator	Insert( Iterator position, ConstReference itemTemplate );
		ETInlineHint Iterator	Insert( Iterator position, ValueType&& itemTemplate );

		template <typename... ElementConstructorArguments>
		ETInlineHint void		Emplace( const SizeType initialIndex, const SizeType itemCount, ElementConstructorArguments&&... elementConstructorArguments );
		template <typename... ElementConstructorArguments>
		ETInlineHint void		Emplace( Iterator position, const SizeType itemCount, ElementConstructorArguments&&... elementConstructorArguments );
		template <typename... ElementConstructorArguments>
		ETInlineHint Iterator	Emplace( Iterator position, ElementConstructorArguments&&... elementConstructorArguments );

		ETInlineHint Iterator	Erase( Iterator position );
		ETInlineHint void		Erase( Iterator position, const UnorderedSemantics );
		ETInlineHint Iterator	Erase( Iterator first, Iterator last );

		template <typename DisposerPredicate>
		ETInlineHint Iterator	EraseAndDispose( const Iterator position, DisposerPredicate disposer );
		template <typename DisposerPredicate>
		ETInlineHint void		EraseAndDispose( const Iterator position, DisposerPredicate disposer, const UnorderedSemantics );

		template <typename DisposerPredicate>
		ETInlineHint Iterator	EraseAndDispose( Iterator beginPosition, Iterator endPosition, DisposerPredicate disposer );

		// Removes all elements from this vector (calls their destructors).
		// Doesn't release memory.
		ETInlineHint void	Clear();
		// EA STL concept.
		// Resets container to an initialized, unallocated state.
		// Safe only for value types with trivial destructor.
		ETInlineHint void	Clear( const ReleaseMemorySemantics );

		template <typename Disposer>
		ETInlineHint void	ClearAndDispose( Disposer disposer );
		template <typename Disposer>
		ETInlineHint void	ClearAndDispose( Disposer disposer, const ReleaseMemorySemantics );

		ETInlineHint void	Resize( const SizeType elementCount );
		ETInlineHint void	Resize( const SizeType elementCount, ConstReference value );

	// - CONTENT QUERY -----------------------------------

		ETInlineHint SizeType	Size() const;

		ETInlineHint bool	IsEmpty() const;

		ETInlineHint	operator bool() const;

	// - CAPACITY QUERY ----------------------------------

		ETInlineHint SizeType	GetCapacity() const;

		// Extension: allows to limit amount of allocated memory.
		ETInlineHint void		SetCapacity( const SizeType newCapacity );

		ETInlineHint void		Reserve( const SizeType elementCountHint );

		ETInlineHint SizeType	GetHighWatermark() const;		

	// - ALLOCATOR ACCESS --------------------------------

		ETInlineHint const AllocatorType&	GetAllocator() const;

	// - ITERATOR DEBUGGING ------------------------------

		ETInlineHint bool	ValidateIterator( ConstIterator iterator ) const;

	// - DATA MEMBERS ------------------------------------

	private:
		UnderlyingContainer	_underlyingContainer;
	};

// ---------------------------------------------------

	extern const ::Eldritch2Detail::ResizableArrayBase::UnorderedSemantics		UnorderedSemantics;
	extern const ::Eldritch2Detail::ResizableArrayBase::ReleaseMemorySemantics	ReleaseMemorySemantics;

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/Containers/ResizableArray.inl>
//------------------------------------------------------------------//