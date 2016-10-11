/*==================================================================*\
  ResizableArray.hpp
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
//------------------------------------------------------------------//
#if ET_COMPILER_IS_MSVC && !defined( EA_COMPILER_HAS_C99_FORMAT_MACROS )
//	MSVC complains about *lots* of macro redefinitions in eabase/inttypes.h.
#	define EA_COMPILER_HAS_C99_FORMAT_MACROS
#endif
#include <eastl/vector.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Detail {

	struct ResizableArrayBase {
		struct	UnorderedSemantics {};
		struct	ReleaseMemorySemantics {};
	};

}	// namespace Detail

	template <typename Value, typename Allocator = Eldritch2::ChildAllocator>
	class ResizableArray : public Detail::ResizableArrayBase {
	// - TYPE PUBLISHING ---------------------------------

	private:
		using UnderlyingContainer	= eastl::vector<Value, Detail::EaStlAllocatorAdapterMixin<Allocator>>;

	public:
		using ValueType				= typename UnderlyingContainer::value_type;
		using ConstPointer			= typename UnderlyingContainer::const_pointer;
		using Pointer				= typename UnderlyingContainer::pointer;
		using ConstReference		= typename UnderlyingContainer::const_reference;
		using Reference				= typename UnderlyingContainer::reference;
		using AllocatorType			= typename UnderlyingContainer::allocator_type::PublicType;
		using ConstReverseIterator	= typename UnderlyingContainer::const_reverse_iterator;
		using ReverseIterator		= typename UnderlyingContainer::reverse_iterator;
		using ConstIterator			= typename UnderlyingContainer::const_iterator;
		using Iterator				= typename UnderlyingContainer::iterator;
		using SizeType				= typename UnderlyingContainer::size_type;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//! Constructs this @ref ResizableArray instance.
		template <class = eastl::enable_if<eastl::is_copy_constructible<ValueType>::value>::type>
		ResizableArray( const ResizableArray& containerTemplate, const AllocatorType& allocator = Allocator() );
	//! Constructs this @ref ResizableArray instance.
	/*!	@param[in] first Pointer to the beginning element in a continuous array of objects the container will copy into itself.
		@param[in] last Pointer to one past the end of the array headed by the _first_ parameter. May be equal to _first_.
		@param[in] allocator Movable reference to an allocator to be moved into the container. */
		ResizableArray( ConstPointer first, ConstPointer last, const AllocatorType& allocator = Allocator() );
	//! Constructs this @ref ResizableArray instance.
	/*!	@param[in] initialSize Initial capacity hint, in elements.
		@param[in] allocator Movable reference to an allocator to be moved into the container. */
		ResizableArray( SizeType initialSize, const AllocatorType& allocator = Allocator() );
	//! Constructs this @ref ResizableArray instance.
	/*!	@param[in] initialSize Initial capacity hint, in elements.
		@param[in] allocator Movable reference to an allocator to be moved into the container. */
		ResizableArray( const AllocatorType& allocator = Allocator() );
	//! Constructs this @ref ResizableArray instance.
		ResizableArray( ResizableArray&& );

		~ResizableArray() = default;

	// - ALGORITHMS --------------------------------------

	public:
	//! Locates the first element in the container that compares equal to the passed-in reference.
	/*!	@param[in] itemTemplate The item to search for.
		@param[in] searchHint Iterator to the first element in the container to compare. The iterator must belong to this container.
		@returns If there is at least one element stored in the container equal to _itemTemplate_, then the function returns an iterator to the first such element. Otherwise, it returns the end iterator for the container. */
		ConstIterator	Find( ConstReference itemTemplate, ConstIterator searchHint ) const;
	//! Locates the first element in the container that compares equal to the passed-in reference.
	/*!	@param[in] itemTemplate The item to search for.
		@param[in] searchHint Iterator to the first element in the container to compare. The iterator must belong to this container.
		@returns If there is at least one element stored in the container equal to _itemTemplate_, then the function returns an iterator to the first such element. Otherwise, it returns the end iterator for the container. */
		Iterator		Find( ConstReference itemTemplate, Iterator searchHint );
	//! Locates the first element in the container that compares equal to the passed-in reference.
	/*!	@param[in] itemTemplate The item to search for.
		@returns If there is at least one element stored in the container equal to _itemTemplate_, then the function returns an iterator to the first such element. Otherwise, it returns the end iterator for the container. */
		ConstIterator	Find( ConstReference itemTemplate ) const;
	//! Locates the first element in the container that compares equal to the passed-in reference.
	/*!	@param[in] itemTemplate The item to search for.
		@returns If there is at least one element stored in the container equal to _itemTemplate_, then the function returns an iterator to the first such element. Otherwise, it returns the end iterator for the container. */
		Iterator		Find( ConstReference itemTemplate );

	//! Examines the elements in the ResizableArray to determine if any are equivalent to the argument.
	/*!	@param[in] itemTemplate The item to search for.
		@returns *true* if an item equal to the template was found, *false* if none exist in the container. */
		bool			Contains( ConstReference itemTemplate ) const;

	//! Rearranges the elements within this ResizeableArray such that all that satisfy the given predicate are situated before any that do not.
	/*!	@remark Note that there are no guarantees placed on ordering beyond the passed/did not pass distinction.
		@param[in] predicate The filter predicate to be run. This method uses operator() on the predicate, which should take a single argument of type ConstReference and return a bool indicating if the element passed the check.
		@returns An iterator to the beginning of the now-segregated range, or the end iterator for the container if all elements passed the check. */
		template <typename Predicate>
		Iterator		RemoveIf( Predicate predicate );

	//! Rearranges the elements within this ResizeableArray according to a customizable ordering.
	/*!	@param[in] predicate The ordering predicate to use. This method invokes operator() on the predicate, which should take two arguments of type ConstReference and return a book indicating if the element in the first argument should be placed before the element in the second. */
		template <typename ComparisonPredicate>
		void			Sort( ComparisonPredicate predicate );
		
	// - CONTAINER DUPLICATION ---------------------------

	public:
		template <class = eastl::enable_if<eastl::is_copy_constructible<ValueType>::value>::type>
		ResizableArray&	operator=( const ResizableArray& );
		ResizableArray&	operator=( ResizableArray&& );

		void			Swap( ResizableArray& container );

	// - ELEMENT ITERATION -------------------------------

	public:
		ConstIterator			ConstBegin() const;

		ConstIterator			ConstEnd() const;

		ConstReverseIterator	ConstReverseBegin() const;

		ConstReverseIterator	ConstReverseEnd() const;

		ConstReverseIterator	ReverseBegin() const;
		ReverseIterator			ReverseBegin();
		
		ConstReverseIterator	ReverseEnd() const;
		ReverseIterator			ReverseEnd();

		ConstIterator			Begin() const;
		Iterator				Begin();
		
		ConstIterator			End() const;
		Iterator				End();

	// - END POINT MANIPULATION --------------------------

	public:
		ConstReference	Front() const;
		Reference		Front();
		
		ConstReference	Back() const;
		Reference		Back();

		template <class = eastl::enable_if<eastl::is_copy_constructible<ValueType>::value>::type>
		void			PushBack( const ValueType& item );
		void			PushBack( ValueType&& item );

		template <typename... ElementConstructorArguments>
		void			EmplaceBack( ElementConstructorArguments&&... elementConstructorArguments );

		void			PopBack();

	// - ELEMENT ACCESS ----------------------------------

	public:
		ConstReference	operator[]( SizeType elementIndex ) const;
		Reference		operator[]( SizeType elementIndex );
		
		ConstReference	At( SizeType elementIndex ) const;
		Reference		At( SizeType elementIndex );

		ConstPointer	Data() const;
		Pointer			Data();

	// - CONTAINER MANIPULATION --------------------------

	public:
		void		Assign( ConstPointer first, ConstPointer last );

		template <class = eastl::enable_if<eastl::is_copy_constructible<ValueType>::value>::type>
		void		Insert( Iterator position, SizeType itemCount, const ValueType& itemTemplate );
		template <class = eastl::enable_if<eastl::is_copy_constructible<ValueType>::value>::type>
		Iterator	Insert( Iterator position, const ValueType& itemTemplate );
		Iterator	Insert( Iterator position, ValueType&& itemTemplate );

		template <typename... ElementConstructorArguments>
		Iterator	Emplace( Iterator position, ElementConstructorArguments&&... elementConstructorArguments );

		SizeType	Erase( const ValueType& value );
		void		Erase( Iterator position, const UnorderedSemantics );
		Iterator	Erase( Iterator first, Iterator last );
		Iterator	Erase( Iterator position );

		void		Clear( const ReleaseMemorySemantics );
	//! Removes all elements from this vector and calls their destructors.
	/*!	@remarks Doesn't release memory. */
		void		Clear();

		template <class = eastl::enable_if<eastl::is_copy_constructible<ValueType>::value>::type>
		void		Resize( SizeType elementCount, const ValueType& value );
		void		Resize( SizeType elementCount );

	// - CONTENT QUERY -----------------------------------

	public:
		SizeType			GetSize() const;

		bool				IsEmpty() const;

		explicit operator	bool() const;

	// - CAPACITY QUERY ----------------------------------

	public:
		SizeType	GetCapacity() const;

		void		SetCapacity( SizeType capacityInElements );

		void		Reserve( SizeType capacityInElementsHint );

	// - ALLOCATOR ACCESS --------------------------------

	public:
		const AllocatorType&	GetAllocator() const;

	// - ITERATOR DEBUGGING ------------------------------

	public:
		bool	ValidateIterator( ConstIterator iterator ) const;

	// - DATA MEMBERS ------------------------------------

	private:
		UnderlyingContainer	_underlyingContainer;
	};

// ---------------------------------------------------

	extern const Detail::ResizableArrayBase::ReleaseMemorySemantics	ReleaseMemorySemantics;
	extern const Detail::ResizableArrayBase::UnorderedSemantics		UnorderedSemantics;

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/Containers/ResizableArray.inl>
//------------------------------------------------------------------//