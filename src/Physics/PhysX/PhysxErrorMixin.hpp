/*==================================================================*\
  PhysxErrorMixin.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  �2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
//	(6326) MSVC doesn't like some of the compile-time constant comparison PhysX does. We can't fix this, but we can at least disable the warning.
ET_PUSH_MSVC_WARNING_STATE(disable : 6326)
#include <foundation/PxErrorCallback.h>
ET_POP_MSVC_WARNING_STATE()
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Physics { namespace PhysX {

	template <typename BackingLog>
	class PhysxErrorMixin : public BackingLog, public physx::PxErrorCallback {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref PhysxAllocatorMixin instance.
		template <typename... ConstructorArguments>
		PhysxErrorMixin(ConstructorArguments&&... constructorArguments);

		~PhysxErrorMixin() = default;

		// ---------------------------------------------------

	public:
		void reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line) override;
	};

}}} // namespace Eldritch2::Physics::PhysX

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Physics/PhysX/PhysxErrorMixin.inl>
//------------------------------------------------------------------//
