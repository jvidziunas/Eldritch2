/*==================================================================*\
  InstanceDeleters.inl
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Memory/Allocator.hpp>
#include <Common/Assert.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

template <typename Allocator, typename T>
ETInlineHint ETForceInlineHint InstanceDeleter::InstanceDeleter(Allocator& allocator, T* const /*object*/) :
	_deleter([&](void* object) ETNoexceptHint {
		if (!object) {
			return;
		}
		eastl::destruct(static_cast<T*>(object));
		allocator.Deallocate(object, sizeof(T));
	}) {}

// ---------------------------------------------------

ETInlineHint ETForceInlineHint InstanceDeleter::InstanceDeleter() :
	_deleter([](void* object) ETNoexceptHint {
		ETAssert(object == nullptr, "Leaking memory! Object {} assigned to a unique pointer without a matching deleter.", fmt::ptr(object));
	}) {}

// ---------------------------------------------------

ETInlineHint ETForceInlineHint void InstanceDeleter::operator()(void* object) const ETNoexceptHint {
	_deleter(object);
}

// ---------------------------------------------------

template <typename Allocator, typename T>
ETInlineHint ETForceInlineHint InstanceArrayDeleter::InstanceArrayDeleter(Allocator& allocator, T* const /*objects*/, size_t countInObjects) :
	_deleter([&allocator, countInObjects](void* array) ETNoexceptHint {
		if (!array) {
			return;
		}
		eastl::destruct(static_cast<T*>(array), static_cast<T*>(array) + countInObjects);
		allocator.Deallocate(array, countInObjects * sizeof(T));
	}) {}

// ---------------------------------------------------

ETInlineHint ETForceInlineHint InstanceArrayDeleter::InstanceArrayDeleter() :
	_deleter([](void* array) ETNoexceptHint {
		ETAssert(array == nullptr, "Leaking memory! Array {} assigned to a unique pointer without a matching deleter.", fmt::ptr(array));
	}) {}

// ---------------------------------------------------

ETInlineHint ETForceInlineHint void InstanceArrayDeleter::operator()(void* objects) const ETNoexceptHint {
	_deleter(objects);
}

} // namespace Eldritch2
