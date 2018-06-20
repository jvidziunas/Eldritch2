/*==================================================================*\
  InstanceDeleter.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Mpl/Compiler.hpp>
//------------------------------------------------------------------//
#include <eastl/bonus/compressed_pair.h>
#include <eastl/type_traits.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
class Allocator;
}

namespace Eldritch2 {

class InstanceDeleter {
	// - TYPE PUBLISHING ---------------------------------

public:
	using DeleterFunction = void (*)(Allocator&, void*);

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//! Constructs this @ref InstanceDeleter instance.
	template <typename T>
	InstanceDeleter(Allocator& allocator, T* const object);
	//! Constructs this @ref InstanceDeleter instance.
	InstanceDeleter(const InstanceDeleter&) = default;
	//! Constructs this @ref InstanceDeleter instance.
	InstanceDeleter();

	~InstanceDeleter() = default;

	// ---------------------------------------------------

public:
	Allocator& GetAllocator() const;

	// ---------------------------------------------------

public:
	template <typename T>
	void operator()(T* const object) const;

	// ---------------------------------------------------

public:
	InstanceDeleter& operator=(const InstanceDeleter&) = default;

	// - DATA MEMBERS ------------------------------------

private:
	Allocator*      _allocator;
	void*           _object;
	DeleterFunction _deleter;
};

// ---------------------------------------------------

class InstanceArrayDeleter {
	// - TYPE PUBLISHING ---------------------------------

public:
	using DeleterFunction = void (*)(Allocator&, void*, size_t);

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//! Constructs this @ref InstanceArrayDeleter instance.
	template <typename T>
	InstanceArrayDeleter(Allocator& allocator, T* const objects, size_t countInObjects);
	//! Constructs this @ref InstanceArrayDeleter instance.
	InstanceArrayDeleter(const InstanceArrayDeleter&) = default;
	//! Constructs this @ref InstanceArrayDeleter instance.
	InstanceArrayDeleter();

	~InstanceArrayDeleter() = default;

	// ---------------------------------------------------

public:
	Allocator& GetAllocator() const;

	size_t GetSize() const;

	// ---------------------------------------------------

public:
	template <typename T>
	void operator()(T* const objects) const;

	// ---------------------------------------------------

public:
	InstanceArrayDeleter& operator=(const InstanceArrayDeleter&) = default;

	// - DATA MEMBERS ------------------------------------

private:
	Allocator*      _allocator;
	size_t          _countInObjects;
	DeleterFunction _deleter;
};

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Memory/InstanceDeleter.inl>
//------------------------------------------------------------------//
