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
#include <Common/Containers/Span.hpp>
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
	using ConstSliceType       = typename Span<ConstIterator>;
	using SliceType            = typename Span<Iterator>;

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
	ArrayList(const AllocatorType& allocator, InitializerList<ValueType>);
	//! Constructs this @ref ArrayList instance.
	/*!	@param[in] allocator Allocator template for the container. */
	ArrayList(const AllocatorType& allocator, const ArrayList&);
	//! Constructs this @ref ArrayList instance.
	ArrayList(const ArrayList&) = default;
	//! Constructs this @ref ArrayList instance.
	ArrayList(ArrayList&&) ETNoexceptHint  = default;

	~ArrayList() = default;

	// - ALGORITHMS --------------------------------------

public:
	//! Locates the first element in the container that compares equal to the passed-in reference.
	/*!	@param[in] item The item to search for.
		@param[in] start Iterator to the first element in the container to compare. The iterator must belong to this container.
		@returns If there is at least one element stored in the container equal to _itemTemplate_, then the function returns an iterator
			to the first such element. Otherwise, it returns the end iterator for the container. */
	ConstIterator Find(ConstReference value, ConstIterator where) const ETNoexceptHint;
	//! Locates the first element in the container that compares equal to the passed-in reference.
	/*!	@param[in] item The item to search for.
		@param[in] start Iterator to the first element in the container to compare. The iterator must belong to this container.
		@returns If there is at least one element stored in the container equal to _itemTemplate_, then the function returns an iterator
			to the first such element. Otherwise, it returns the end iterator for the container. */
	Iterator Find(ConstReference value, Iterator where) ETNoexceptHint;
	//! Locates the first element in the container that compares equal to the passed-in reference.
	/*!	@param[in] item The item to search for.
		@returns If there is at least one element stored in the container equal to _itemTemplate_, then the function returns an iterator
			to the first such element. Otherwise, it returns the end iterator for the container. */
	ConstIterator Find(ConstReference value) const ETNoexceptHint;
	//! Locates the first element in the container that compares equal to the passed-in reference.
	/*!	@param[in] item The item to search for.
		@returns If there is at least one element stored in the container equal to _itemTemplate_, then the function returns an iterator
			to the first such element. Otherwise, it returns the end iterator for the container. */
	Iterator Find(ConstReference value) ETNoexceptHint;

	//! Examines the elements in the ArrayList to determine if any are equivalent to the argument.
	/*!	@param[in] item The item to search for.
		@returns *true* if an item equal to the template was found, *false* if none exist in the container. */
	bool Contains(ConstReference value) const ETNoexceptHint;

	template <typename UnaryPredicate>
	void EraseIf(UnaryPredicate condition);

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

	ConstSliceType Slice(SizeType offset, SizeType length) const ETNoexceptHint;
	SliceType      Slice(SizeType offset, SizeType length) ETNoexceptHint;

	operator ConstSliceType() const ETNoexceptHint;
	operator SliceType() ETNoexceptHint;

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
	void     Insert(Iterator where, SizeType itemCount, ConstReference value);
	Iterator Insert(Iterator where, ConstReference value);
	Iterator Insert(Iterator where, ValueType&& value);

	template <typename... Arguments>
	Iterator Emplace(Iterator where, Arguments&&... arguments);

	ReverseIterator Erase(ReverseIterator first, ReverseIterator last);
	ReverseIterator Erase(ReverseIterator where);
	Iterator        Erase(Iterator first, Iterator last);
	Iterator        Erase(Iterator where);

	Iterator EraseFirst(ConstReference value);

	Iterator EraseLast(ConstReference value);

	void EraseUnordered(ReverseIterator where);
	void EraseUnordered(Iterator where);

	Iterator EraseFirstUnordered(ConstReference value);

	Iterator EraseLastUnordered(ConstReference value);

	void Append(ConstReference value);
	void Append(ValueType&& value);

	template <typename... Arguments>
	Reference EmplaceBack(Arguments&&... arguments);

	void Pop();

	template <typename UnaryPredicate>
	void ClearAndDispose(const ReleaseMemorySemantics, UnaryPredicate disposer) ETNoexceptHintIf(IsNoThrowCallable<UnaryPredicate, Reference>());
	template <typename UnaryPredicate>
	void ClearAndDispose(UnaryPredicate disposer) ETNoexceptHintIf(IsNoThrowCallable<UnaryPredicate, Reference>());

	void Clear(const ReleaseMemorySemantics) ETNoexceptHint;
	//! Removes all elements from this vector and calls their destructors.
	/*!	@remarks Doesn't release memory. */
	void Clear() ETNoexceptHint;

	void Resize(SizeType size, ConstReference filler);
	void Resize(SizeType size);

	// - CAPACITY QUERY ----------------------------------

public:
	SizeType GetCapacity() const ETNoexceptHint;

	SizeType GetSize() const ETNoexceptHint;

	void SetCapacity(SizeType capacity);

	void Reserve(SizeType minimumCapacity);

	explicit operator bool() const ETNoexceptHint;

	bool IsEmpty() const ETNoexceptHint;

	ArrayList& ShrinkToFit();

	// - CONTAINER DUPLICATION ---------------------------

public:
	template <typename InputIterator>
	void Assign(InputIterator first, InputIterator last);
	void Assign(InitializerList<ValueType>);

	ArrayList& operator=(const ArrayList&) = default;
	ArrayList& operator=(ArrayList&&) = default;

	// - ALLOCATOR ACCESS --------------------------------

public:
	const AllocatorType& GetAllocator() const ETNoexceptHint;

	// - ITERATOR DEBUGGING ------------------------------

public:
	bool Validate(ConstIterator iterator) const ETNoexceptHint;

	// - DATA MEMBERS ------------------------------------

private:
	UnderlyingContainer _container;

	// ---------------------------------------------------

	template <typename Value, typename Allocator>
	friend void Swap(ArrayList<Value, Allocator>&, ArrayList<Value, Allocator>&) ETNoexceptHint;
};

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Containers/ArrayList.inl>
//------------------------------------------------------------------//
