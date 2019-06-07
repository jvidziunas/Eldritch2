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
	explicit EaStlAllocatorMixin(const EaStlAllocatorMixin&) ETNoexceptHintIf(IsNoThrowCopyConstructible<PublicType>()) = default;
	//! Constructs this @ref EaStlAllocatorMixin instance.
	explicit EaStlAllocatorMixin(EaStlAllocatorMixin&&) ETNoexceptHintIf(IsNoThrowMoveConstructible<PublicType>()) = default;
	//! Constructs this @ref EaStlAllocatorMixin instance.
	template <typename... Arguments>
	EaStlAllocatorMixin(Arguments&&...) ETNoexceptHintIf(IsNoThrowConstructible<PublicType, Arguments&&...>());

	~EaStlAllocatorMixin() = default;

	// ---------------------------------------------------

public:
	ETRestrictHint void* allocate(size_t sizeInBytes, size_t alignmentInBytes, size_t offsetInBytes, int flags = 0);
	ETRestrictHint void* allocate(size_t sizeInBytes, int flags = 0);

	void deallocate(void* pointer, size_t sizeInBytes);

	// ---------------------------------------------------

public:
	const char* get_name() const ETNoexceptHint;

	void set_name(const char* name) ETNoexceptHint;

	// ---------------------------------------------------

public:
	EaStlAllocatorMixin& operator=(const EaStlAllocatorMixin&) = default;
	EaStlAllocatorMixin& operator=(EaStlAllocatorMixin&&) = default;

	// ---------------------------------------------------

	template <typename EldritchAllocator>
	friend void Swap(EaStlAllocatorMixin<EldritchAllocator>&, EaStlAllocatorMixin<EldritchAllocator>&) ETNoexceptHint;
};

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Memory/EaStlAllocatorMixin.inl>
//------------------------------------------------------------------//
