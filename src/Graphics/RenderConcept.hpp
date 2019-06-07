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

	template <typename Instance, typename Key = uint32>
	class RenderSortPredicate {
		// - TYPE PUBLISHING ---------------------------------

	public:
		using ConstIterator = const Instance*;
		using Iterator      = Instance*;
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
		void SortValues(Scheduling::JobExecutor& executor, Iterator begin, Iterator end, KeyType outKeys[]) ETNoexceptHint;

		template <typename Split>
		uint32 FindSplits(Scheduling::JobExecutor& executor, ConstIterator begin, ConstIterator end, Split splits[]) ETNoexceptHint;

		// - DATA MEMBERS ------------------------------------

	private:
		Vector _inverseCellSize;
	};

	// ---

	template <typename Instance, class Allocator = Eldritch2::MallocAllocator>
	class RenderConcept : public ArrayBvh<Instance, RenderSortPredicate<Instance>, Allocator> {
		// - TYPE PUBLISHING ---------------------------------

	public:
		using BvhType       = ArrayBvh<Instance, RenderSortPredicate<Instance>, Allocator>;
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

		// ---------------------------------------------------

		template <typename ViewType, typename Instance2, class Allocator2>
		friend void BuildShadowViewList(ArrayList<ViewType>& views, const RenderConcept<Instance2, Allocator2>& source);

		template <typename ViewType, typename Instance2, class Allocator2>
		friend void BuildViewList(ArrayList<ViewType>& views, const RenderConcept<Instance2, Allocator2>& source);
	};

}} // namespace Eldritch2::Graphics

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/RenderConcept.inl>
//------------------------------------------------------------------//
