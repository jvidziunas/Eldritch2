/*==================================================================*\
  HostMixin.inl
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

#ifdef CopyMemory
#	undef CopyMemory
#endif

namespace Eldritch2 { namespace Graphics { namespace Vulkan {
	namespace {

		struct ET16ByteAligned AllocationHeader {
			size_t allocationSize;
		};

		// ---------------------------------------------------

		ETInlineHint ETPureFunctionHint AllocationHeader& GetHeader(void* allocation) {
			return static_cast<AllocationHeader*>(allocation)[-1];
		}

		// ---------------------------------------------------

		template <typename EldritchAllocator>
		ETRestrictHint void* Allocate(void* allocator, size_t sizeInBytes, size_t alignmentInBytes, VkSystemAllocationScope /*scope*/) {
			if (alignmentInBytes > __alignof(AllocationHeader)) {
				return nullptr;
			}

			sizeInBytes += sizeof(AllocationHeader);

			AllocationHeader* result(static_cast<AllocationHeader*>(static_cast<EldritchAllocator*>(allocator)->Allocate(sizeInBytes, __alignof(AllocationHeader), 0u, AllocationDuration::Normal)));
			if (!result) {
				return nullptr;
			}

			*result++ = AllocationHeader{ sizeInBytes };

			return result;
		}

		// ---------------------------------------------------

		template <typename EldritchAllocator>
		ETRestrictHint void* Reallocate(void* allocator, void* originalMemory, size_t sizeInBytes, size_t alignmentInBytes, VkSystemAllocationScope scope) {
			void* const result(Allocate<EldritchAllocator>(allocator, sizeInBytes, alignmentInBytes, scope));

			if (result && originalMemory) {
				CopyMemory(result, originalMemory, GetHeader(originalMemory).allocationSize);
			}

			Free<EldritchAllocator>(allocator, originalMemory);

			return result;
		}

		// ---------------------------------------------------

		template <typename EldritchAllocator>
		void Free(void* allocator, void* memory) {
			if (!memory) {
				return;
			}

			AllocationHeader& header(GetHeader(memory));

			static_cast<EldritchAllocator*>(allocator)->Deallocate(&header, header.allocationSize);
		}

	} // anonymous namespace

	template <class EldritchAllocator>
	template <typename... Arguments, class /*SFINAE*/>
	ETInlineHint HostMixin<EldritchAllocator>::HostMixin(Arguments&&... arguments) :
		EldritchAllocator(eastl::forward<Arguments>(arguments)...) {
		_callbacks.pUserData             = static_cast<EldritchAllocator*>(this);
		_callbacks.pfnAllocation         = &Graphics::Vulkan::Allocate<EldritchAllocator>;
		_callbacks.pfnReallocation       = &Graphics::Vulkan::Reallocate<EldritchAllocator>;
		_callbacks.pfnFree               = &Graphics::Vulkan::Free<EldritchAllocator>;
		_callbacks.pfnInternalAllocation = [](void* /*allocator*/, size_t /*sizeInBytes*/, VkInternalAllocationType /*type*/, VkSystemAllocationScope /*scope*/) {};
		_callbacks.pfnInternalFree       = [](void* /*allocator*/, size_t /*sizeInBytes*/, VkInternalAllocationType /*type*/, VkSystemAllocationScope /*scope*/) {};
	}

	// ---------------------------------------------------

	template <class EldritchAllocator>
	ETInlineHint const VkAllocationCallbacks* HostMixin<EldritchAllocator>::AsAllocationCallbacks() const {
		return eastl::addressof(_callbacks);
	}

	// ---------------------------------------------------

	template <class EldritchAllocator>
	ETInlineHint HostMixin<EldritchAllocator>::operator const VkAllocationCallbacks*() const {
		return eastl::addressof(_callbacks);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
