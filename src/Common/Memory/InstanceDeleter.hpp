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

#if !defined(ET_INSTANCE_DELETER_SIZE)
#	define ET_INSTANCE_DELETER_SIZE (2u * sizeof(void*))
#endif

#if !defined(ET_INSTANCE_ARRAY_DELETER_SIZE)
#	define ET_INSTANCE_ARRAY_DELETER_SIZE (3u * sizeof(void*))
#endif

namespace Eldritch2 {

class InstanceDeleter {
	// - TYPE PUBLISHING ---------------------------------

public:
	using DeleterFunction = FixedFunction<ET_INSTANCE_DELETER_SIZE, void (void*)>;

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
	void operator()(void* object) const ETNoexceptHint;

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
	using DeleterFunction = FixedFunction<ET_INSTANCE_ARRAY_DELETER_SIZE, void(void*)>;

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
	void operator()(void* objects) const ETNoexceptHint;

	// ---------------------------------------------------

public:
	InstanceArrayDeleter& operator=(const InstanceArrayDeleter&) = default;

	// - DATA MEMBERS ------------------------------------

private:
	DeleterFunction _deleter;
};

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Memory/InstanceDeleter.inl>
//------------------------------------------------------------------//
