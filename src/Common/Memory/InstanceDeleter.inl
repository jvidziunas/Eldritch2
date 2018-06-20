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
//------------------------------------------------------------------//
#include <EASTL/memory.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Detail {

	template <typename T>
	ETInlineHint void DeleterFunction(Allocator& allocator, void* objects, size_t objectCount) {
		eastl::destruct(static_cast<T*>(objects), static_cast<T*>(objects) + objectCount);

		allocator.Deallocate(objects, sizeof(T) * objectCount);
	}

	// ---------------------------------------------------

	template <typename T>
	ETInlineHint void DeleterFunction(Allocator& allocator, void* object) {
		eastl::destruct(static_cast<T*>(object));

		allocator.Deallocate(object, sizeof(T));
	}

} // namespace Detail

template <typename T>
ETInlineHint InstanceDeleter::InstanceDeleter(
	Allocator& allocator,
	T* const   object) :
	_allocator(eastl::addressof(allocator)),
	_object(object),
	_deleter(Detail::DeleterFunction<T>) {
}

// ---------------------------------------------------

ETInlineHint InstanceDeleter::InstanceDeleter() :
	_allocator(nullptr),
	_deleter([](Allocator&, void*) {}) {}

// ---------------------------------------------------

ETInlineHint Allocator& InstanceDeleter::GetAllocator() const {
	return *_allocator;
}

// ---------------------------------------------------

template <typename T>
ETInlineHint void InstanceDeleter::operator()(T* const object) const {
	if (!object) {
		return;
	}

	_deleter(*_allocator, _object);
}

// ---------------------------------------------------

template <typename T>
ETInlineHint InstanceArrayDeleter::InstanceArrayDeleter(
	Allocator& allocator,
	T* const /*objects*/,
	size_t countInObjects) :
	_countInObjects(countInObjects),
	_allocator(eastl::addressof(allocator)),
	_deleter(Detail::DeleterFunction<T>) {
}

// ---------------------------------------------------

ETInlineHint InstanceArrayDeleter::InstanceArrayDeleter() :
	_countInObjects(0u),
	_allocator(nullptr),
	_deleter([](Allocator&, void*, size_t) {}) {}

// ---------------------------------------------------

ETInlineHint Allocator& InstanceArrayDeleter::GetAllocator() const {
	return *_allocator;
}

// ---------------------------------------------------

ETInlineHint size_t InstanceArrayDeleter::GetSize() const {
	return _countInObjects;
}

// ---------------------------------------------------

template <typename T>
ETInlineHint void InstanceArrayDeleter::operator()(T* const objects) const {
	if (!objects) {
		return;
	}

	_deleter(*_allocator, objects, _countInObjects);
}

} // namespace Eldritch2
