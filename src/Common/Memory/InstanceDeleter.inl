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
ETInlineHint InstanceDeleter::InstanceDeleter(Allocator& allocator, T* const /*object*/) :
	_deleter([&allocator](void* object) {
		if (!object) {
			return;
		}
		eastl::destruct(static_cast<T*>(object));
		allocator.Deallocate(object, sizeof(T));
	}) {
}

// ---------------------------------------------------

ETInlineHint InstanceDeleter::InstanceDeleter() :
	_deleter([](void* object) {
		ET_ASSERT(object == nullptr, "Leaking memory! Object {} assigned to a unique pointer without a matching deleter.", object);
	}) {}

// ---------------------------------------------------

ETInlineHint void InstanceDeleter::operator()(void* object) const {
	_deleter(object);
}

// ---------------------------------------------------

template <typename Allocator, typename T>
ETInlineHint InstanceArrayDeleter::InstanceArrayDeleter(Allocator& allocator, T* const /*objects*/, size_t countInObjects) :
	_countInObjects(countInObjects),
	_deleter([&allocator](void* array, size_t countInObjects) {
		if (!array) {
			return;
		}
		eastl::destruct(static_cast<T*>(array), static_cast<T*>(array) + countInObjects);
		allocator.Deallocate(array, sizeof(T) * countInObjects);
	}) {
}

// ---------------------------------------------------

ETInlineHint InstanceArrayDeleter::InstanceArrayDeleter() :
	_countInObjects(0u),
	_deleter([](void* array, size_t) {
		ET_ASSERT(array == nullptr, "Leaking memory! Array {} assigned to a unique pointer without a matching deleter.", array);
	}) {
}

// ---------------------------------------------------

ETInlineHint size_t InstanceArrayDeleter::GetSize() const {
	return _countInObjects;
}

// ---------------------------------------------------

ETInlineHint void InstanceArrayDeleter::operator()(void* objects) const {
	_deleter(objects, GetSize());
}

} // namespace Eldritch2
