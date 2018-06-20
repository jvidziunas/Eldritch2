/*==================================================================*\
  ArrayList.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Memory/EaStlAllocatorMixin.hpp>
#include <Common/Containers/RangeAdapters.hpp>
#include <Common/Memory/MallocAllocator.hpp>
//------------------------------------------------------------------//
#include <eastl/vector.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

struct UnorderedSemantics {};
struct ReleaseMemorySemantics {};

// ---

template <typename Value, typename Allocator = MallocAllocator>
class ArrayList {
	// - TYPE PUBLISHING ---------------------------------

private:
	using UnderlyingContainer = eastl::vector<Value, EaStlAllocatorMixin<Allocator>>;

public:
	using ValueType            = typename UnderlyingContainer::value_type;
	using ConstPointer         = typename UnderlyingContainer::const_pointer;
	using Pointer              = typename UnderlyingContainer::pointer;
	using ConstReference       = typename UnderlyingContainer::const_reference;
	using Reference            = typename UnderlyingContainer::reference;
	using AllocatorType        = typename UnderlyingContainer::allocator_type::PublicType;
	using ConstReverseIterator = typename UnderlyingContainer::const_reverse_iterator;
	using ReverseIterator      = typename UnderlyingContainer::reverse_iterator;
	using ConstIterator        = typename UnderlyingContainer::const_iterator;
	using Iterator             = typename UnderlyingContainer::iterator;
	using SizeType             = typename UnderlyingContainer::size_type;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//! Constructs this @ref ArrayList instance.
	/*!	@param[in] capacityHint Initial capacity hint, in elements.
		@param[in] allocator Allocator template for the container */
	ArrayList(SizeType capacityHint, const AllocatorType& allocator);
	//! Constructs this @ref ArrayList instance.
	/*!	@param[in] allocator Allocator template for the container. */
	ArrayList(const AllocatorType& allocator);
	//! Constructs this @ref ArrayList instance.
	ArrayList(const ArrayList&);
	//! Constructs this @ref ArrayList instance.
	ArrayList(ArrayList&&);

	~ArrayList() = default;

	// - ALGORITHMS --------------------------------------

public:
	//! Locates the first element in the container that compares equal to the passed-in reference.
	/*!	@param[in] item The item to search for.
		@param[in] start Iterator to the first element in the container to compare. The iterator must belong to this container.
		@returns If there is at least one element stored in the container equal to _itemTemplate_, then the function returns an iterator to the first such element. Otherwise, it returns the end iterator for the container. */
	ConstIterator Find(ConstReference item, ConstIterator start) const;
	//! Locates the first element in the container that compares equal to the passed-in reference.
	/*!	@param[in] item The item to search for.
		@param[in] start Iterator to the first element in the container to compare. The iterator must belong to this container.
		@returns If there is at least one element stored in the container equal to _itemTemplate_, then the function returns an iterator to the first such element. Otherwise, it returns the end iterator for the container. */
	Iterator Find(ConstReference item, Iterator start);
	//! Locates the first element in the container that compares equal to the passed-in reference.
	/*!	@param[in] item The item to search for.
		@returns If there is at least one element stored in the container equal to _itemTemplate_, then the function returns an iterator to the first such element. Otherwise, it returns the end iterator for the container. */
	ConstIterator Find(ConstReference item) const;
	//! Locates the first element in the container that compares equal to the passed-in reference.
	/*!	@param[in] item The item to search for.
		@returns If there is at least one element stored in the container equal to _itemTemplate_, then the function returns an iterator to the first such element. Otherwise, it returns the end iterator for the container. */
	Iterator Find(ConstReference item);

	//! Examines the elements in the ArrayList to determine if any are equivalent to the argument.
	/*!	@param[in] item The item to search for.
		@returns *true* if an item equal to the template was found, *false* if none exist in the container. */
	bool Contains(ConstReference item) const;

	template <typename UnaryPredicate>
	void EraseIf(UnaryPredicate filter);

	//! Rearranges the elements within this ResizeableArray according to a customizable ordering.
	/*!	@param[in] sorter The ordering predicate to use. This method invokes operator() on the predicate, which should take two arguments of type ConstReference and return a book indicating if the element in the first argument should be placed before the element in the second. */
	template <typename BinaryPredicate>
	void Sort(BinaryPredicate sorter);

	// - CONTAINER DUPLICATION ---------------------------

public:
	ArrayList& operator=(const ArrayList&);
	ArrayList& operator=(ArrayList&&);

	template <typename InputIterator>
	void Assign(InputIterator first, InputIterator last);

	// - ELEMENT ITERATION -------------------------------

public:
	ConstReverseIterator ConstReverseBegin() const;

	ConstReverseIterator ConstReverseEnd() const;

	ConstReverseIterator ReverseBegin() const;
	ReverseIterator      ReverseBegin();

	ConstReverseIterator ReverseEnd() const;
	ReverseIterator      ReverseEnd();

	ConstIterator ConstBegin() const;

	ConstIterator ConstEnd() const;

	ConstIterator Begin() const;
	Iterator      Begin();

	ConstIterator End() const;
	Iterator      End();

	// - END POINT MANIPULATION --------------------------

public:
	ConstReference Front() const;
	Reference      Front();

	ConstReference Back() const;
	Reference      Back();

	void Append(const ValueType& item);
	void Append(ValueType&& item);

	template <typename... ElementConstructorArguments>
	void EmplaceBack(ElementConstructorArguments&&... arguments);

	void Pop();

	// - ELEMENT ACCESS ----------------------------------

public:
	ConstReference operator[](SizeType index) const;
	Reference      operator[](SizeType index);

	ConstReference At(SizeType index) const;
	Reference      At(SizeType index);

	ConstPointer GetData() const;
	Pointer      GetData();

	// - CONTAINER MANIPULATION --------------------------

public:
	void Insert(Iterator start, SizeType itemCount, const ValueType& item);
	template <typename InputIterator>
	void     Insert(Iterator position, InputIterator first, InputIterator last);
	Iterator Insert(Iterator position, const ValueType& item);
	Iterator Insert(Iterator position, ValueType&& item);

	template <typename... ElementConstructorArguments>
	Iterator Emplace(Iterator position, ElementConstructorArguments&&... arguments);

	void            Erase(ReverseIterator position, const UnorderedSemantics);
	void            Erase(Iterator position, const UnorderedSemantics);
	ReverseIterator Erase(ReverseIterator first, ReverseIterator last);
	ReverseIterator Erase(ReverseIterator position);
	Iterator        Erase(Iterator first, Iterator last);
	Iterator        Erase(Iterator position);

	void Clear(const ReleaseMemorySemantics);
	//! Removes all elements from this vector and calls their destructors.
	/*!	@remarks Doesn't release memory. */
	void Clear();

	void Resize(SizeType elementCount, const ValueType& item);
	void Resize(SizeType elementCount);

	// - CONTENT QUERY -----------------------------------

public:
	SizeType GetSize() const;

	bool IsEmpty() const;

	explicit operator bool() const;

	// - CAPACITY QUERY ----------------------------------

public:
	SizeType GetCapacity() const;

	void SetCapacity(SizeType capacityInElements);

	void Reserve(SizeType capacityHintInElements);

	void ShrinkToFit();

	// - ALLOCATOR ACCESS --------------------------------

public:
	const AllocatorType& GetAllocator() const;

	// - ITERATOR DEBUGGING ------------------------------

public:
	bool ValidateIterator(ConstIterator iterator) const;

	// - DATA MEMBERS ------------------------------------

private:
	UnderlyingContainer _container;

	// ---------------------------------------------------

	template <typename Value, typename Allocator>
	friend void Swap(ArrayList<Value, Allocator>&, ArrayList<Value, Allocator>&);
};

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Containers/ArrayList.inl>
//------------------------------------------------------------------//
