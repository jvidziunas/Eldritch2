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
#include <Common/Pair.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Detail {

	template <typename Identifier>
	class IdentifierPool {
		// - TYPE PUBLISHING ---------------------------------

	public:
		using IdentifierType = Identifier;

		// ---

		struct ValueRange {
			// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref ValueRange instance.
			ValueRange(IdentifierType first, IdentifierType last);
			//!	Constructs this @ref ValueRange instance.
			ValueRange() = default;

			~ValueRange() = default;

			// ---------------------------------------------------

		public:
			bool CanMergeForwardWith(const ValueRange& range) const;

			// ---------------------------------------------------

		public:
			IdentifierType GetSize() const;

			// ---------------------------------------------------

		public:
			bool Contains(IdentifierType identifier) const;

			bool IsEmpty() const;

			// - DATA MEMBERS ------------------------------------

		public:
			IdentifierType first;
			IdentifierType last;
		};
	};

} // namespace Detail

template <typename Identifier, class Allocator = MallocAllocator>
class IdentifierPool : Detail::IdentifierPool<Identifier> {
	// - TYPE PUBLISHING ---------------------------------

public:
	using ValueRange     = typename Detail::IdentifierPool<Identifier>::ValueRange;
	using IdentifierType = typename Detail::IdentifierPool<Identifier>::IdentifierType;
	using AllocatorType  = Allocator;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref IdentifierPool instance.
	explicit IdentifierPool(std::initializer_list<ValueRange> ranges, const AllocatorType& allocator);
	//!	Constructs this @ref IdentifierPool instance.
	explicit IdentifierPool(ValueRange initialRange, const AllocatorType& allocator);
	//!	Constructs this @ref IdentifierPool instance.
	explicit IdentifierPool(const AllocatorType& allocator);
	//!	Constructs this @ref IdentifierPool instance.
	IdentifierPool(IdentifierPool&&);
	//!	Disable copy construction.
	IdentifierPool(const IdentifierPool&) = delete;

	~IdentifierPool() = default;

	// ---------------------------------------------------

public:
	Pair<Identifier, bool> Allocate();

	Pair<Identifier, bool> AllocateRange(IdentifierType sizeInElements);

	void ReleaseRange(ValueRange range);

	void Release(IdentifierType identifier);

	// ---------------------------------------------------

public:
	void Clear();

	// ---------------------------------------------------

public:
	bool IsEmpty() const;

	// ---------------------------------------------------

	//!	Disable copy assignment.
	IdentifierPool& operator=(const IdentifierPool&) = delete;

	// ---------------------------------------------------

public:
	IdentifierType GetLargestSpanLength() const;

	// - DATA MEMBERS ------------------------------------

private:
	ArrayList<ValueRange, AllocatorType> _freeRanges;
};

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/IdentifierPool.inl>
//------------------------------------------------------------------//
