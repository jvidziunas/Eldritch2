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

namespace Eldritch2 {
namespace Scheduling {
	class JobExecutor;
}
} // namespace Eldritch2::Scheduling

namespace Eldritch2 { namespace Graphics {

	template <typename Instance, class Allocator = Eldritch2::MallocAllocator>
	class RenderConcept : public ArrayBvh<Instance*, Allocator> {
		// - TYPE PUBLISHING ---------------------------------

	public:
		using HierarchyType = ArrayBvh<Instance*, Allocator>;
		using AllocatorType = typename HierarchyType::AllocatorType;
		using ValueType     = typename HierarchyType::ValueType;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref RenderConcept instance.
		RenderConcept(const AllocatorType& allocator);
		//!	Disable copy construction.
		RenderConcept(const RenderConcept&) = delete;
		//!	Constructs this @ref RenderConcept instance.
		RenderConcept(RenderConcept&&) = default;

		~RenderConcept() = default;

		// ---------------------------------------------------

	public:
		void Insert(const ValueType& value);

		void Erase(const ValueType& value);

		// ---------------------------------------------------

	public:
		bool ShouldRebuildHierarchy(MemoryOrder order = std::memory_order_consume);

		// ---------------------------------------------------

		//!	Disable copy assignment.
		RenderConcept& operator=(const RenderConcept&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		Atomic<bool> _shouldRebuildHierarchy;
	};

	// ---

	template <typename Value, class Allocator>
	class ViewConcept {
		// - TYPE PUBLISHING ---------------------------------

	public:
		using ConstIterator = typename ArrayList<const Value*, Allocator>::ConstIterator;
		using AllocatorType = Allocator;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref ViewConcept instance.
		ViewConcept(const AllocatorType& allocator);
		//!	Disable copy construction.
		ViewConcept(const ViewConcept&) = delete;

		~ViewConcept() = default;

		// ---------------------------------------------------

	public:
		ConstIterator Begin() const;

		ConstIterator End() const;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		ViewConcept& operator=(const ViewConcept&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		mutable Mutex                          _mutex;
		ArrayList<const Value*, AllocatorType> _views;
	};

	// ---

	template <typename Instance, class Allocator, typename Extractor>
	void Rebuild(Scheduling::JobExecutor& executor, RenderConcept<Instance, Allocator>& concept, Extractor extractor);

}} // namespace Eldritch2::Graphics

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/RenderConcept.inl>
//------------------------------------------------------------------//
