/*==================================================================*\
  EaStlAllocatorMixin.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Mpl/Compiler.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

template <typename EldritchAllocator>
class EaStlAllocatorMixin : public EldritchAllocator {
	// - TYPE PUBLISHING ---------------------------------

public:
	using PublicType = typename EldritchAllocator;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//! Constructs this @ref EaStlAllocatorMixin instance.
	template <typename... Arguments>
	EaStlAllocatorMixin(Arguments&&... arguments) ETNoexceptHint;
	//! Constructs this @ref EaStlAllocatorMixin instance.
	EaStlAllocatorMixin(const EaStlAllocatorMixin&) = default;

	~EaStlAllocatorMixin() = default;

	// ---------------------------------------------------

public:
	ETRestrictHint void* allocate(size_t sizeInBytes, size_t alignmentInBytes, size_t offsetInBytes, int flags = 0);
	ETRestrictHint void* allocate(size_t sizeInBytes, int flags = 0);

	void deallocate(void* pointer, size_t sizeInBytes);

	// ---------------------------------------------------

public:
	const char* get_name() const;

	void set_name(const char* name);

	// ---------------------------------------------------

public:
	ETInlineHint EaStlAllocatorMixin& operator=(const EaStlAllocatorMixin&) = default;

	// ---------------------------------------------------

	template <typename EldritchAllocator>
	friend void Swap(EaStlAllocatorMixin<EldritchAllocator>&, EaStlAllocatorMixin<EldritchAllocator>&);
};

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Memory/EaStlAllocatorMixin.inl>
//------------------------------------------------------------------//
