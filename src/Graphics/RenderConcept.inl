/*==================================================================*\
  RenderConcept.inl
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scheduling/JobExecutor.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics {

	template <typename Instance, class Allocator>
	ETInlineHint ETForceInlineHint RenderConcept<Instance, Allocator>::RenderConcept(const AllocatorType& allocator) :
		HierarchyType(allocator),
		_shouldRebuildHierarchy(false) {}

	// ---------------------------------------------------

	template <typename Instance, class Allocator>
	ETInlineHint ETForceInlineHint void RenderConcept<Instance, Allocator>::Insert(const ValueType& value) {
		Lock _(_hierarchyMutex);

		_hierarchy.Insert(value);
		_shouldRebuildHierarchy.store(true, std::memory_order_release);
	}

	// ---------------------------------------------------

	template <typename Instance, class Allocator>
	ETInlineHint ETForceInlineHint void RenderConcept<Instance, Allocator>::Erase(const ValueType& value) {
		Lock _(_hierarchyMutex);

		_hierarchy.Erase(_hierarchy.Find(value));
		_shouldRebuildHierarchy.store(true, std::memory_order_release);
	}

	// ---------------------------------------------------

	template <typename Instance, class Allocator>
	ETInlineHint ETForceInlineHint bool RenderConcept<Instance, Allocator>::ShouldRebuildHierarchy(MemoryOrder order) ETNoexceptHint {
		return _shouldRebuildHierarchy.load(order);
	}

	// ---------------------------------------------------

	template <typename Value, class Allocator>
	ETInlineHint ETForceInlineHint ViewConcept<Value, Allocator>::ViewConcept(const AllocatorType& allocator) :
		_views(allocator) {}

	// ---------------------------------------------------

	template <typename Value, class Allocator>
	ETInlineHint ETForceInlineHint typename ViewConcept<Value, Allocator>::ConstIterator ViewConcept<Value, Allocator>::Begin() const ETNoexceptHint {
		return _views.Begin();
	}

	// ---------------------------------------------------

	template <typename Value, class Allocator>
	ETInlineHint ETForceInlineHint typename ViewConcept<Value, Allocator>::ConstIterator ViewConcept<Value, Allocator>::End() const ETNoexceptHint {
		return _views.End();
	}

	// ---------------------------------------------------

	template <typename Instance, class Allocator, typename Extractor>
	ETInlineHint ETForceInlineHint void Rebuild(Scheduling::JobExecutor& /*executor*/, RenderConcept<Instance, Allocator>& /*concept*/, Extractor /*extractor*/) {
		// executor.Transform(concept.Begin(), concept.End(), nullptr, extractor);
	}

}} // namespace Eldritch2::Graphics
