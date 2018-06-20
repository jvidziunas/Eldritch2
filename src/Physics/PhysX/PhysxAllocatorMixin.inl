/*==================================================================*\
  PhysxAllocatorMixin.inl
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 { namespace Physics { namespace PhysX {
	namespace {

		enum : size_t {
			RequiredAlignment = 16u,
			PaddingInBytes    = RequiredAlignment
		};

	} // anonymous namespace

	template <typename BackingAllocator>
	template <typename... ConstructorArguments>
	ETInlineHint PhysxAllocatorMixin<BackingAllocator>::PhysxAllocatorMixin(ConstructorArguments&&... constructorArguments) :
		BackingAllocator(eastl::forward<ConstructorArguments>(constructorArguments)...) {}

	// ---------------------------------------------------

	template <typename BackingAllocator>
	ETInlineHint void* PhysxAllocatorMixin<BackingAllocator>::allocate(size_t sizeInBytes, const char* /*typeName*/, const char* /*filename*/, int /*line*/) {
		void* const result(BackingAllocator::Allocate(sizeInBytes + PaddingInBytes, RequiredAlignment, 0u, AllocationDuration::Normal));

		if (ET_UNLIKELY(result == nullptr)) {
			return nullptr;
		}

		static_cast<size_t*>(result)[PaddingInBytes / sizeof(size_t) - 1] = sizeInBytes;

		return static_cast<char*>(result) + PaddingInBytes;
	}

	// ---------------------------------------------------

	template <typename BackingAllocator>
	ETInlineHint void PhysxAllocatorMixin<BackingAllocator>::deallocate(void* memory) {
		if (ET_UNLIKELY(memory == nullptr)) {
			return;
		}

		BackingAllocator::Deallocate(static_cast<char*>(memory) - PaddingInBytes, static_cast<size_t*>(memory)[-1]);
	}

}}} // namespace Eldritch2::Physics::PhysX
