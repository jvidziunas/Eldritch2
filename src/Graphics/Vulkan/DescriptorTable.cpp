/*==================================================================*\
  DescriptorTable.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/DescriptorTable.hpp>
#include <Graphics/Vulkan/VulkanTools.hpp>
#include <Graphics/Vulkan/Gpu.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	DescriptorTable::DescriptorTable() :
		_pool(nullptr),
		_descriptors(nullptr) {
	}

	// ---------------------------------------------------

	DescriptorTable::DescriptorTable(DescriptorTable&& table) :
		DescriptorTable() {
		Swap(*this, table);
	}

	// ---------------------------------------------------

	DescriptorTable::~DescriptorTable() {
		ET_ASSERT(_pool == nullptr, "Leaking Vulkan descriptor pool!");
		ET_ASSERT(_descriptors == nullptr, "Leaking Vulkan descriptor set!");
	}

	// ---------------------------------------------------

	void DescriptorTable::PushDescriptors(Gpu& gpu, uint32_t slot, uint32_t count, const VkDescriptorImageInfo* images) {
		const VkWriteDescriptorSet write{
			VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
			/*pNext =*/nullptr,
			_descriptors,
			/*dstBinding =*/0u,
			slot,
			count,
			VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
			images,
			/*pBufferInfo =*/nullptr,
			/*pTexelBufferView =*/nullptr
		};

		vkUpdateDescriptorSets(gpu, 1u, &write, 0u, nullptr);
	}

	// ---------------------------------------------------

	VkResult DescriptorTable::BindResources(Gpu& gpu, VkDescriptorSetLayout layout, uint32_t poolCount, const VkDescriptorPoolSize* pools) {
		using ::Eldritch2::Swap;

		VkDescriptorPool                 pool;
		const VkDescriptorPoolCreateInfo poolInfo{
			VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
			/*pNext =*/nullptr,
			/*flags =*/0u,
			/*maxSets =*/1u, // We always allocate exactly one descriptor set (see below) from the pool.
			poolCount,
			pools
		};
		ET_FAIL_UNLESS(vkCreateDescriptorPool(gpu, &poolInfo, gpu.GetAllocationCallbacks(), &pool));
		ET_AT_SCOPE_EXIT(vkDestroyDescriptorPool(gpu, pool, gpu.GetAllocationCallbacks()));

		VkDescriptorSet                   descriptors;
		const VkDescriptorSetAllocateInfo descriptorsInfo{
			VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
			/*pNext =*/nullptr,
			pool,
			1u, &layout
		};
		ET_FAIL_UNLESS(vkAllocateDescriptorSets(gpu, &descriptorsInfo, &descriptors));
		ET_AT_SCOPE_EXIT(if (pool) vkFreeDescriptorSets(gpu, pool, 1u, &descriptors));

		Swap(_pool, pool);
		Swap(_descriptors, descriptors);

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void DescriptorTable::FreeResources(Gpu& gpu) {
		VkDescriptorPool pool(eastl::exchange(_pool, nullptr));
		if (pool == nullptr) {
			return;
		}

		vkFreeDescriptorSets(gpu, pool, 1u, &_descriptors);
		vkDestroyDescriptorPool(gpu, pool, gpu.GetAllocationCallbacks());
	}

	// ---------------------------------------------------

	void Swap(DescriptorTable& lhs, DescriptorTable& rhs) {
		using ::Eldritch2::Swap;

		Swap(lhs._pool, rhs._pool);
		Swap(lhs._descriptors, rhs._descriptors);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
