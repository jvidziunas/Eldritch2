/*==================================================================*\
  RenderConcept.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 { namespace Scheduling {
	class JobExecutor;
}} // namespace Eldritch2::Scheduling

namespace Eldritch2 { namespace Graphics {

	template <typename Record, typename Key = uint32>
	class RenderSortPredicate {
		// - TYPE PUBLISHING ---------------------------------

	public:
		using ConstIterator = const Record*;
		using Iterator      = Record*;
		using KeyType       = Key;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref RenderSortPredicate instance.
		ETConstexpr RenderSortPredicate(const RenderSortPredicate&) ETNoexceptHint = default;
		//!	Constructs this @ref RenderSortPredicate instance.
		RenderSortPredicate(Vector cellSize) ETNoexceptHint;

		~RenderSortPredicate() = default;

		// ---------------------------------------------------

	public:
		void Sort(Scheduling::JobExecutor& executor, Iterator begin, Iterator end, KeyType outKeys[]) ETNoexceptHint;

		template <typename Sector>
		uint32 Split(Scheduling::JobExecutor& executor, ConstIterator begin, ConstIterator end, Sector sectors[]) ETNoexceptHint;

		// - DATA MEMBERS ------------------------------------

	private:
		Vector _inverseCellSize;
	};

	// ---

	template <typename Record, class Allocator = Eldritch2::MallocAllocator>
	class RenderConcept : public ArrayBvh<Record, RenderSortPredicate<Record>, Allocator> {
		// - TYPE PUBLISHING ---------------------------------

	public:
		using BvhType       = ArrayBvh<Record, RenderSortPredicate<Record>, Allocator>;
		using AllocatorType = typename BvhType::AllocatorType;
		using SortType      = typename BvhType::SortType;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref RenderConcept instance.
		RenderConcept(const AllocatorType& allocator, const SortType& sort) ETNoexceptHint;
		//!	Disable copy construction.
		RenderConcept(const RenderConcept&) = default;
		//!	Constructs this @ref RenderConcept instance.
		RenderConcept(RenderConcept&&) ETNoexceptHint = default;

		~RenderConcept() = default;

		// ---------------------------------------------------

	public:
		bool ShouldRebuildHierarchy(MemoryOrder order = std::memory_order_consume) ETNoexceptHint;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		RenderConcept& operator=(const RenderConcept&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		Atomic<bool> _shouldRebuildHierarchy;
	};

}} // namespace Eldritch2::Graphics

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/RenderConcept.inl>
//------------------------------------------------------------------//
