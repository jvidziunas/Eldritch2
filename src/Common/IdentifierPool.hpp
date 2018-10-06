/*==================================================================*\
  IdentifierPool.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Containers/ArrayList.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Detail {

	template <typename Value>
	struct ValueRange {
		// - TYPE PUBLISHING ---------------------------------

	public:
		using ValueType = Value;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref ValueRange instance.
		ValueRange(Value begin, Value end) ETNoexceptHint;
		//!	Constructs this @ref ValueRange instance.
		ValueRange() ETNoexceptHint = default;

		~ValueRange() = default;

		// ---------------------------------------------------

	public:
		bool Contains(Value value) const ETNoexceptHint;

		// ---------------------------------------------------

	public:
		ValueType GetSize() const ETNoexceptHint;

		bool IsEmpty() const ETNoexceptHint;

		// - DATA MEMBERS ------------------------------------

	public:
		ValueType begin;
		ValueType end;

		// ---------------------------------------------------

		template <typename Value>
		friend bool IsContiguous(const ValueRange<Value>& lhs, const ValueRange<Value>& rhs) ETNoexceptHint;

		template <typename Value>
		friend ValueRange<Value> Union(const ValueRange<Value>& lhs, const ValueRange<Value>& rhs) ETNoexceptHint;

		template <typename Value>
		friend bool operator<(const ValueRange<Value>&, const ValueRange<Value>&) ETNoexceptHint;
	};

} // namespace Detail

template <typename Identifier, class Allocator = MallocAllocator>
class IdentifierPool {
	// - TYPE PUBLISHING ---------------------------------

public:
	using RangeList     = ArrayList<Detail::ValueRange<Identifier>, Allocator>;
	using RangeType     = typename RangeList::ValueType;
	using AllocatorType = typename RangeList::AllocatorType;
	using ValueType     = typename RangeType::ValueType;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref IdentifierPool instance.
	IdentifierPool(const AllocatorType& allocator, std::initializer_list<RangeType> ranges);
	//!	Constructs this @ref IdentifierPool instance.
	IdentifierPool(const AllocatorType& allocator, ValueType begin, ValueType end);
	//!	Constructs this @ref IdentifierPool instance.
	IdentifierPool(const AllocatorType& allocator);
	//!	Constructs this @ref IdentifierPool instance.
	IdentifierPool(const IdentifierPool&) = default;
	//!	Constructs this @ref IdentifierPool instance.
	IdentifierPool(IdentifierPool&&) = default;

	~IdentifierPool() = default;

	// ---------------------------------------------------

public:
	Pair<ValueType, bool> Allocate(ValueType count);
	Pair<ValueType, bool> Allocate();

	void Deallocate(ValueType identifier);
	void Deallocate(RangeType range);

	// ---------------------------------------------------

public:
	bool IsEmpty() const ETNoexceptHint;

	void Clear();

	// ---------------------------------------------------

public:
	ValueType GetLargestRange() const ETNoexceptHint;

	// ---------------------------------------------------

public:
	const AllocatorType& GetAllocator() const ETNoexceptHint;

	// ---------------------------------------------------

public:
	IdentifierPool& Assign(std::initializer_list<RangeType> ranges);
	IdentifierPool& Assign(ValueType begin, ValueType end);

	IdentifierPool& operator=(const IdentifierPool&) = default;
	IdentifierPool& operator=(IdentifierPool&&) = default;

	// - DATA MEMBERS ------------------------------------

private:
	RangeList _ranges;

	// ---------------------------------------------------

	template <typename Identifier, class Allocator>
	friend void Swap(IdentifierPool<Identifier, Allocator>&, IdentifierPool<Identifier, Allocator>&);
};

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/IdentifierPool.inl>
//------------------------------------------------------------------//
