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
#include <Common/Function.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

class InstanceDeleter {
	// - TYPE PUBLISHING ---------------------------------

public:
	using DeleterFunction = Function<void(void*)>;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//! Constructs this @ref InstanceDeleter instance.
	template <typename Allocator, typename T>
	InstanceDeleter(Allocator& allocator, T* const object);
	//! Constructs this @ref InstanceDeleter instance.
	InstanceDeleter(const InstanceDeleter&) = default;
	//! Constructs this @ref InstanceDeleter instance.
	InstanceDeleter();

	~InstanceDeleter() = default;

	// ---------------------------------------------------

public:
	void operator()(void* object) const;

	// ---------------------------------------------------

public:
	InstanceDeleter& operator=(const InstanceDeleter&) = default;

	// - DATA MEMBERS ------------------------------------

private:
	DeleterFunction _deleter;
};

// ---------------------------------------------------

class InstanceArrayDeleter {
	// - TYPE PUBLISHING ---------------------------------

public:
	using DeleterFunction = Function<void(void*, size_t)>;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//! Constructs this @ref InstanceArrayDeleter instance.
	template <typename Allocator, typename T>
	InstanceArrayDeleter(Allocator& allocator, T* const objects, size_t countInObjects);
	//! Constructs this @ref InstanceArrayDeleter instance.
	InstanceArrayDeleter(const InstanceArrayDeleter&) = default;
	//! Constructs this @ref InstanceArrayDeleter instance.
	InstanceArrayDeleter();

	~InstanceArrayDeleter() = default;

	// ---------------------------------------------------

public:
	size_t GetSize() const;

	// ---------------------------------------------------

public:
	void operator()(void* objects) const;

	// ---------------------------------------------------

public:
	InstanceArrayDeleter& operator=(const InstanceArrayDeleter&) = default;

	// - DATA MEMBERS ------------------------------------

private:
	DeleterFunction _deleter;
	size_t          _countInObjects;
};

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Memory/InstanceDeleter.inl>
//------------------------------------------------------------------//
