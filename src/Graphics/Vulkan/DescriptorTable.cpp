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
		_lastCurrent(0u),
		_resources(MallocAllocator("Descriptor Table Resource List Allocator")),
		_pool(nullptr) {
		Fill(_setLayouts, End(_setLayouts), nullptr);
		Fill(_sets, End(_sets), nullptr);
	}

	// ---------------------------------------------------

	DescriptorTable::DescriptorTable(DescriptorTable&& table) :
		DescriptorTable() {
		Swap(*this, table);
	}

	// ---------------------------------------------------

	DescriptorTable::~DescriptorTable() {
		ET_ASSERT(_pool == nullptr, "Leaking Vulkan descriptor pool!");
	}

	// ---------------------------------------------------

	void DescriptorTable::PushToGpu(Gpu& gpu) {
		ArrayList<VkDescriptorImageInfo> images;

		const VkWriteDescriptorSet writes[] = {
			VkWriteDescriptorSet {
				VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
				/*pNext =*/nullptr,
				_sets[0],
				/*dstBinding =*/0u,
				/*dstArrayElement =*/0u,
				/*descriptorCount =*/uint32(images.GetSize()),
				VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
				images.GetData(),
				/*pBufferInfo =*/nullptr,
				/*pTexelBufferView =*/nullptr }
		};

		vkUpdateDescriptorSets(gpu, uint32(ETCountOf(writes)), writes, /*descriptorCopyCount =*/0u, /*pDescriptorCopies =*/nullptr);
	}

	// ---------------------------------------------------

	VkResult DescriptorTable::BindResources(Gpu& gpu, std::initializer_list<VkDescriptorPoolSize> pools, std::initializer_list<VkDescriptorSetLayoutBinding> bindings) {
		using ::Eldritch2::Swap;

		VkDescriptorPool                 pool;
		const VkDescriptorPoolCreateInfo poolInfo {
			VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
			/*pNext =*/nullptr,
			/*flags =*/0u,
			/*maxSets =*/1u, // We always allocate exactly one descriptor set (see below) from the pool.
			uint32(pools.size()), pools.begin()
		};
		ET_ABORT_UNLESS(vkCreateDescriptorPool(gpu, ETAddressOf(poolInfo), gpu.GetAllocationCallbacks(), ETAddressOf(pool)));
		ET_AT_SCOPE_EXIT(vkDestroyDescriptorPool(gpu, pool, gpu.GetAllocationCallbacks()));

		VkDescriptorSetLayout                 setLayouts[ETCountOf(_setLayouts)];
		const VkDescriptorSetLayoutCreateInfo descriptorLayoutInfo {
			VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
			/*pNext =*/nullptr,
			/*flags =*/0u,
			uint32(bindings.size()), bindings.begin()
		};
		ET_ABORT_UNLESS(vkCreateDescriptorSetLayout(gpu, ETAddressOf(descriptorLayoutInfo), gpu.GetAllocationCallbacks(), setLayouts));
		ET_AT_SCOPE_EXIT(vkDestroyDescriptorSetLayout(gpu, setLayouts[0], gpu.GetAllocationCallbacks()));

		VkDescriptorSet                   sets[ETCountOf(_sets)];
		const VkDescriptorSetAllocateInfo setsInfo {
			VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
			/*pNext =*/nullptr,
			pool,
			uint32(ETCountOf(sets)), setLayouts
		};
		ET_ABORT_UNLESS(vkAllocateDescriptorSets(gpu, ETAddressOf(setsInfo), sets));
		ET_AT_SCOPE_EXIT(if (pool) vkFreeDescriptorSets(gpu, pool, uint32(ETCountOf(sets)), sets));

		Swap(_pool, pool);
		Swap(_setLayouts, setLayouts);
		Swap(_sets, sets);

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void DescriptorTable::FreeResources(Gpu& gpu) {
		if (VkDescriptorPool pool = eastl::exchange(_pool, nullptr)) {
			vkFreeDescriptorSets(gpu, pool, ETCountOf(_sets), _sets);
			vkDestroyDescriptorPool(gpu, pool, gpu.GetAllocationCallbacks());
		}

		for (VkDescriptorSetLayout& layout : _setLayouts) {
			vkDestroyDescriptorSetLayout(gpu, eastl::exchange(layout, nullptr), gpu.GetAllocationCallbacks());
		}
	}

	// ---------------------------------------------------

	void Swap(DescriptorTable& lhs, DescriptorTable& rhs) {
		using ::Eldritch2::Swap;

		Swap(lhs._lastCurrent, rhs._lastCurrent);
		Swap(lhs._resources, rhs._resources);
		Swap(lhs._pool, rhs._pool);
		Swap(lhs._setLayouts, rhs._setLayouts);
		Swap(lhs._sets, rhs._sets);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
