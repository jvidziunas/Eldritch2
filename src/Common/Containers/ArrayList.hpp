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
	/*!	@param[in] allocator Allocator template for the container.
		@param[in] capacityHint Initial capacity hint, in elements. */
	ArrayList(const AllocatorType& allocator = AllocatorType(), SizeType capacityHint = 0u);
	//! Constructs this @ref ArrayList instance.
	/*!	@param[in] allocator Allocator template for the container. */
	template <typename InputIterator>
	ArrayList(const AllocatorType& allocator, InputIterator begin, InputIterator end);
	//! Constructs this @ref ArrayList instance.
	/*!	@param[in] allocator Allocator template for the container. */
	ArrayList(const AllocatorType& allocator, std::initializer_list<ValueType>);
	//! Constructs this @ref ArrayList instance.
	/*!	@param[in] allocator Allocator template for the container. */
	ArrayList(const AllocatorType& allocator, const ArrayList&);
	//! Constructs this @ref ArrayList instance.
	ArrayList(const ArrayList&) = default;
	//! Constructs this @ref ArrayList instance.
	ArrayList(ArrayList&&) = default;

	~ArrayList() = default;

	// - ALGORITHMS --------------------------------------

public:
	//! Locates the first element in the container that compares equal to the passed-in reference.
	/*!	@param[in] item The item to search for.
		@param[in] start Iterator to the first element in the container to compare. The iterator must belong to this container.
		@returns If there is at least one element stored in the container equal to _itemTemplate_, then the function returns an iterator
			to the first such element. Otherwise, it returns the end iterator for the container. */
	ConstIterator Find(ConstReference value, ConstIterator where) const;
	//! Locates the first element in the container that compares equal to the passed-in reference.
	/*!	@param[in] item The item to search for.
		@param[in] start Iterator to the first element in the container to compare. The iterator must belong to this container.
		@returns If there is at least one element stored in the container equal to _itemTemplate_, then the function returns an iterator
			to the first such element. Otherwise, it returns the end iterator for the container. */
	Iterator Find(ConstReference value, Iterator where);
	//! Locates the first element in the container that compares equal to the passed-in reference.
	/*!	@param[in] item The item to search for.
		@returns If there is at least one element stored in the container equal to _itemTemplate_, then the function returns an iterator
			to the first such element. Otherwise, it returns the end iterator for the container. */
	ConstIterator Find(ConstReference value) const;
	//! Locates the first element in the container that compares equal to the passed-in reference.
	/*!	@param[in] item The item to search for.
		@returns If there is at least one element stored in the container equal to _itemTemplate_, then the function returns an iterator
			to the first such element. Otherwise, it returns the end iterator for the container. */
	Iterator Find(ConstReference value);

	//! Examines the elements in the ArrayList to determine if any are equivalent to the argument.
	/*!	@param[in] item The item to search for.
		@returns *true* if an item equal to the template was found, *false* if none exist in the container. */
	bool Contains(ConstReference value) const;

	template <typename UnaryPredicate>
	void EraseIf(UnaryPredicate condition);

	//! Rearranges the elements within this ResizeableArray according to a customizable ordering.
	/*!	@param[in] sort The ordering predicate to use. This method invokes operator() on the predicate, which should take two arguments
			of type ConstReference and return a bool indicating if the element in the first argument should be placed before the element
			in the second. */
	template <typename BinaryPredicate>
	void Sort(BinaryPredicate sort);

	// - ELEMENT ITERATION -------------------------------

public:
	ConstReverseIterator ConstReverseBegin() const ETNoexceptHint;

	ConstReverseIterator ConstReverseEnd() const ETNoexceptHint;

	ConstReverseIterator ReverseBegin() const ETNoexceptHint;
	ReverseIterator      ReverseBegin() ETNoexceptHint;

	ConstReverseIterator ReverseEnd() const ETNoexceptHint;
	ReverseIterator      ReverseEnd() ETNoexceptHint;

	ConstIterator ConstBegin() const ETNoexceptHint;

	ConstIterator ConstEnd() const ETNoexceptHint;

	ConstIterator Begin() const ETNoexceptHint;
	Iterator      Begin() ETNoexceptHint;

	ConstIterator End() const ETNoexceptHint;
	Iterator      End() ETNoexceptHint;

	// - ELEMENT ACCESS ----------------------------------

public:
	ConstReference Front() const ETNoexceptHint;
	Reference      Front() ETNoexceptHint;

	ConstReference Back() const ETNoexceptHint;
	Reference      Back() ETNoexceptHint;

	ConstReference operator[](SizeType index) const ETNoexceptHint;
	Reference      operator[](SizeType index) ETNoexceptHint;

	ConstReference At(SizeType index) const;
	Reference      At(SizeType index);

	ConstPointer GetData() const ETNoexceptHint;
	Pointer      GetData() ETNoexceptHint;

	// - CONTAINER MANIPULATION --------------------------

public:
	template <typename InputIterator>
	void     Insert(Iterator where, InputIterator first, InputIterator last);
	void     Insert(Iterator where, SizeType itemCount, const ValueType& value);
	Iterator Insert(Iterator where, const ValueType& value);
	Iterator Insert(Iterator where, ValueType&& value);

	template <typename... Arguments>
	Iterator Emplace(Iterator where, Arguments&&... arguments);

	ReverseIterator Erase(ReverseIterator first, ReverseIterator last);
	ReverseIterator Erase(ReverseIterator where);
	Iterator        Erase(Iterator first, Iterator last);
	Iterator        Erase(Iterator where);

	void EraseUnordered(ReverseIterator where);
	void EraseUnordered(Iterator where);

	Iterator EraseFirst(const ValueType& value);

	Iterator EraseLast(const ValueType& value);

	Iterator EraseFirstUnordered(const ValueType& value);

	Iterator EraseLastUnordered(const ValueType& value);

	void Append(const ValueType& value);
	void Append(ValueType&& value);

	template <typename... Arguments>
	Reference EmplaceBack(Arguments&&... arguments);

	void Pop();

	void Clear(const ReleaseMemorySemantics);
	//! Removes all elements from this vector and calls their destructors.
	/*!	@remarks Doesn't release memory. */
	void Clear();

	void Resize(SizeType elementCount, const ValueType& filler);
	void Resize(SizeType elementCount);

	// - CAPACITY QUERY ----------------------------------

public:
	SizeType GetCapacity() const ETNoexceptHint;

	SizeType GetSize() const ETNoexceptHint;

	void SetCapacity(SizeType capacityInElements);

	void Reserve(SizeType capacityHintInElements);

	void ShrinkToFit();

	explicit operator bool() const ETNoexceptHint;

	bool IsEmpty() const ETNoexceptHint;

	// - CONTAINER DUPLICATION ---------------------------

public:
	template <typename InputIterator>
	void Assign(InputIterator first, InputIterator last);
	void Assign(std::initializer_list<ValueType>);

	ArrayList& operator=(const ArrayList&) = default;
	ArrayList& operator=(ArrayList&&) = default;

	// - ALLOCATOR ACCESS --------------------------------

public:
	const AllocatorType& GetAllocator() const ETNoexceptHint;

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
