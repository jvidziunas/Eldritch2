/*==================================================================*\
  GpuResourceApi.DescriptorTable.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/GpuResourceApi.hpp>
#include <Graphics/Vulkan/VulkanTools.hpp>
#include <Graphics/Vulkan/Gpu.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	DescriptorTable::DescriptorTable() ETNoexceptHint : _lastCurrent(0u),
														_images(MallocAllocator("Descriptor Table Image List Allocator")),
														_pool(NullVulkanHandle),
														_descriptors(NullVulkanHandle) {}

	// ---------------------------------------------------

	DescriptorTable::DescriptorTable(DescriptorTable&& table) ETNoexceptHint : DescriptorTable() {
		Swap(*this, table);
	}

	// ---------------------------------------------------

	DescriptorTable::~DescriptorTable() {
		ETAssert(_pool == NullVulkanHandle, "Leaking Vulkan descriptor pool {}!", fmt::ptr(_pool));
	}

	// ---------------------------------------------------

	VkResult DescriptorTable::BindResourceViews(Gpu& gpu, ResourceIndex begin, ResourceIndex end) {
		const auto root(_images.GetData());
		ForEach(Get<SourceImages>(root) + begin, Get<SourceImages>(root) + end, Get<ImageViews>(root) + begin, [&](const GpuImage* image, VkImageView& tableView) ETNoexceptHint {
			VkImageView                 view;
			const VkImageViewCreateInfo viewCreateInfo{
				VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
				/*pNext =*/nullptr,
				0u,
				VkImage(*image),
				// VkImageViewType            viewType;
				// VkFormat                   format;
				// VkComponentMapping         components;
				// VkImageSubresourceRange    subresourceRange;
			};

			if (Succeeded(vkCreateImageView(gpu, ETAddressOf(viewCreateInfo), gpu.GetAllocationCallbacks(), ETAddressOf(view)))) {
				gpu.Finalize([oldView = Exchange(tableView, view)](Gpu& gpu) ETNoexceptHint {
					// Destroy the old image view if it exists/no-op if creating a new image view.
					vkDestroyImageView(gpu, oldView, gpu.GetAllocationCallbacks());
				});
			}
		});

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	VkResult DescriptorTable::PushToGpu(Gpu& gpu, ResourceIndex begin, ResourceIndex end) {
		const ResourceIndex          count(end - begin);
		VkDescriptorImageInfo* const images(ETStackAlloc(VkDescriptorImageInfo, count));
		const VkWriteDescriptorSet   writes[] = {
            VkWriteDescriptorSet{
                VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
                /*pNext =*/nullptr,
                _descriptors,
                /*dstBinding =*/0u,
                /*dstArrayElement =*/begin,
                /*descriptorCount =*/count,
                VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                images,
                /*pBufferInfo =*/nullptr,
                /*pTexelBufferView =*/nullptr }
		};

		vkUpdateDescriptorSets(gpu, uint32(ETCountOf(writes)), writes, /*descriptorCopyCount =*/0u, /*pDescriptorCopies =*/nullptr);

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	VkResult DescriptorTable::BindResources(Gpu& gpu, VkDescriptorSetLayout layout, InitializerList<VkDescriptorPoolSize> pools) {
		using ::Eldritch2::Swap;

		VkDescriptorPool                 pool;
		const VkDescriptorPoolCreateInfo poolInfo{
			VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
			/*pNext =*/nullptr,
			/*flags =*/0u,
			/*maxSets =*/1u, // We always allocate exactly one descriptor set (see below) from the pool.
			uint32(pools.size()), pools.begin()
		};
		ET_ABORT_UNLESS(vkCreateDescriptorPool(gpu, ETAddressOf(poolInfo), gpu.GetAllocationCallbacks(), ETAddressOf(pool)));
		ET_AT_SCOPE_EXIT(vkDestroyDescriptorPool(gpu, pool, gpu.GetAllocationCallbacks()));

		VkDescriptorSet                   descriptors;
		const VkDescriptorSetAllocateInfo descriptorsInfo{
			VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
			/*pNext =*/nullptr,
			pool,
			/*descriptorSetCount =*/1u,
			ETAddressOf(layout)
		};
		ET_ABORT_UNLESS(vkAllocateDescriptorSets(gpu, ETAddressOf(descriptorsInfo), ETAddressOf(descriptors)));
		ET_AT_SCOPE_EXIT(if (pool) vkFreeDescriptorSets(gpu, pool, /*descriptorSetCount =*/1u, ETAddressOf(descriptors)));

		ImageList images(_images.GetAllocator());
		ET_AT_SCOPE_EXIT(images.ClearAndDispose([&](const GpuImage* /*source*/, VkImageView view) ETNoexceptHint {
			vkDestroyImageView(gpu, view, gpu.GetAllocationCallbacks());
		}));

		Swap(_pool, pool);
		Swap(_descriptors, descriptors);
		Swap(_images, images);

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void DescriptorTable::FreeResources(Gpu& gpu) ETNoexceptHint {
		_images.ClearAndDispose([&](const GpuImage* /*source*/, VkImageView view) ETNoexceptHint {
			vkDestroyImageView(gpu, view, gpu.GetAllocationCallbacks());
		});

		if (VkDescriptorPool pool = Exchange(_pool, nullptr)) {
			const VkDescriptorSet sets[] = { Exchange(_descriptors, nullptr) };
			vkFreeDescriptorSets(gpu, pool, ETCountOf(sets), sets);
			vkDestroyDescriptorPool(gpu, pool, gpu.GetAllocationCallbacks());
		}
	}

	// ---------------------------------------------------

	void Swap(DescriptorTable& lhs, DescriptorTable& rhs) ETNoexceptHint {
		using ::Eldritch2::Swap;

		Swap(lhs._lastCurrent, rhs._lastCurrent);
		Swap(lhs._images, rhs._images);
		Swap(lhs._pool, rhs._pool);
		Swap(lhs._descriptors, rhs._descriptors);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
