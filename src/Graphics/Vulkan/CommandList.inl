/*==================================================================*\
  CommandList.inl
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

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	ETInlineHint ETForceInlineHint AbstractCommandList::AbstractCommandList(AbstractCommandList&& list) ETNoexceptHint : _pool(Exchange(list._pool, nullptr)), _commands(Exchange(list._commands, nullptr)) {}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint AbstractCommandList::AbstractCommandList() ETNoexceptHint : _pool(nullptr), _commands(nullptr) {}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint AbstractCommandList::~AbstractCommandList() {
		ETAssert(_pool == nullptr, "Leaking Vulkan command pool {}!", fmt::ptr(_pool));
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint VkResult AbstractCommandList::Submit(Gpu& gpu, VkFence consumed, uint32 submitCount, const VkSubmitInfo submits[]) ETNoexceptHint {
		ET_ABORT_UNLESS(vkEndCommandBuffer(_commands));

		if (consumed) {
			ET_ABORT_UNLESS(vkResetFences(gpu, /*fenceCount =*/1u, ETAddressOf(consumed)));
		}

		return gpu.GetQueue(Drawing).Submit(consumed, submitCount, submits);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint VkResult AbstractCommandList::BindResources(Gpu& gpu, QueueClass queue) {
		using ::Eldritch2::Swap;

		VkCommandPool                 pool;
		const VkCommandPoolCreateInfo poolInfo{
			VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
			/*pNext =*/nullptr,
			VK_COMMAND_POOL_CREATE_TRANSIENT_BIT,
			gpu.GetQueueFamilyByClass(queue)
		};

		ET_ABORT_UNLESS(vkCreateCommandPool(gpu, ETAddressOf(poolInfo), gpu.GetAllocationCallbacks(), ETAddressOf(pool)));
		ET_AT_SCOPE_EXIT(vkDestroyCommandPool(gpu, pool, gpu.GetAllocationCallbacks()));

		VkCommandBuffer                   commands;
		const VkCommandBufferAllocateInfo commandsInfo{
			VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
			/*pNext =*/nullptr,
			pool,
			VK_COMMAND_BUFFER_LEVEL_PRIMARY,
			1u
		};

		ET_ABORT_UNLESS(vkAllocateCommandBuffers(gpu, ETAddressOf(commandsInfo), ETAddressOf(commands)));
		ET_AT_SCOPE_EXIT(if (pool != nullptr) { vkFreeCommandBuffers(gpu, pool, commandsInfo.commandBufferCount, ETAddressOf(commands)); });

		const VkCommandBufferBeginInfo beginInfo{
			VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
			/*pNext =*/nullptr,
			VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
			/*pInheritanceInfo =*/nullptr
		};
		ET_ABORT_UNLESS(vkBeginCommandBuffer(commands, ETAddressOf(beginInfo)));

		Swap(_pool, pool);
		Swap(_commands, commands);

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void AbstractCommandList::FreeResources(Gpu& gpu) ETNoexceptHint {
		if (VkCommandPool pool = Exchange(_pool, nullptr)) {
			const VkCommandBuffer commands[] = { Exchange(_commands, nullptr) };
			vkFreeCommandBuffers(gpu, pool, ETCountOf(commands), commands);
			vkDestroyCommandPool(gpu, pool, gpu.GetAllocationCallbacks());
		}
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint VkSparseBufferMemoryBindInfo TransferCommandList::CreateBindInfo(VkBuffer buffer) ETNoexceptHint {
		return VkSparseBufferMemoryBindInfo{ buffer, /*bindCount =*/0u, /*pBinds =*/0u };
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint VkSparseImageMemoryBindInfo TransferCommandList::CreateBindInfo(VkImage image) ETNoexceptHint {
		return VkSparseImageMemoryBindInfo{ image, /*bindCount =*/0u, /*pBinds =*/0u };
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint VkSparseImageOpaqueMemoryBindInfo TransferCommandList::CreateOpaqueBindInfo(VkImage image) ETNoexceptHint {
		return VkSparseImageOpaqueMemoryBindInfo{ image, /*bindCount =*/0u, /*pBinds =*/0u };
	}

}}} // namespace Eldritch2::Graphics::Vulkan
