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

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	template <class EldritchAllocator>
	template <typename... Arguments>
	ETInlineHint HostMixin<EldritchAllocator>::HostMixin(Arguments&&... arguments) ETNoexceptHintIf(IsNoThrowConstructible<EldritchAllocator, Arguments...>()) :
		EldritchAllocator(Forward<Arguments>(arguments)...) {
		using Header = typename EldritchAllocator::AllocationHeader<16u>;
		static ETConstexpr auto Noop([](void* /*allocator*/, size_t /*byteSize*/, VkInternalAllocationType /*type*/, VkSystemAllocationScope /*scope*/) ETNoexceptHint -> void {});
		static ETConstexpr auto Free([](void* allocator, void* allocation) ETNoexceptHint -> void {
			if (Header* const header = Header::Get(allocation)) {
				static_cast<EldritchAllocator*>(allocator)->Deallocate(allocation, sizeof(Header) + header->userByteSize);
			}
		});

		static ETConstexpr auto Reallocate([](void* allocator, void* old, size_t byteSize, size_t byteAlignment, VkSystemAllocationScope /*scope*/) ETNoexceptHint -> void* {
			ETAssert(byteAlignment <= Header::DataAlignment, "alignment {} is too strict to be honored by host allocator", byteAlignment);

			const auto result(new(*static_cast<EldritchAllocator*>(allocator), byteSize) Header(byteSize));
			if (const Header* const oldHeader = Header::Get(old)) {
				Copy(oldHeader->userBytes, oldHeader->userBytes + oldHeader->userByteSize, result->userBytes);
				Free(allocator, old);
			}

			return result->userBytes;
		});

		static ETConstexpr auto Allocate([](void* allocator, size_t byteSize, size_t byteAlignment, VkSystemAllocationScope scope) -> void* {
			return Reallocate(allocator, /*allocation =*/nullptr, byteSize, byteAlignment, scope);
		});

		// ---

		_callbacks.pUserData             = static_cast<EldritchAllocator*>(this);
		_callbacks.pfnAllocation         = Allocate;
		_callbacks.pfnReallocation       = Reallocate;
		_callbacks.pfnFree               = Free;
		_callbacks.pfnInternalAllocation = Noop;
		_callbacks.pfnInternalFree       = Noop;
	}

	// ---------------------------------------------------

	template <class EldritchAllocator>
	ETConstexpr ETForceInlineHint const VkAllocationCallbacks* HostMixin<EldritchAllocator>::GetCallbacks() const ETNoexceptHint {
		return ETAddressOf(_callbacks);
	}

	// ---------------------------------------------------

	template <class EldritchAllocator>
	ETConstexpr ETForceInlineHint HostMixin<EldritchAllocator>::operator const VkAllocationCallbacks*() const ETNoexceptHint {
		return ETAddressOf(_callbacks);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
