/*==================================================================*\
  PhysxAllocatorMixin.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <foundation/PxAllocatorCallback.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Physics { namespace PhysX {

	template <typename BackingAllocator>
	class PhysxAllocatorMixin : public BackingAllocator, public physx::PxAllocatorCallback {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref PhysxAllocatorMixin instance.
		template <typename... ConstructorArguments>
		PhysxAllocatorMixin(ConstructorArguments&&... constructorArguments);

		~PhysxAllocatorMixin() = default;

		// ---------------------------------------------------

	public:
		void* allocate(size_t sizeInBytes, const char* typeName, const char* filename, int line) override;

		void deallocate(void* memory) override;
	};

}}} // namespace Eldritch2::Physics::PhysX

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Physics/PhysX/PhysxAllocatorMixin.inl>
//------------------------------------------------------------------//
