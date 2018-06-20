/*==================================================================*\
  GraphicsPipeline.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/GraphicsPipelineBuilder.hpp>
#include <Graphics/Vulkan/GraphicsPipeline.hpp>
#include <Graphics/Vulkan/VulkanTools.hpp>
#include <Graphics/Vulkan/Gpu.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	GraphicsPipeline::Pass::Pass(GeometryType source) :
		renderPass(nullptr),
		source(source) {}

	// ---------------------------------------------------

	GraphicsPipeline::Pass::~Pass() {
		ET_ASSERT(renderPass == nullptr, "Leaking Vulkan render pass!");
	}

	// ---------------------------------------------------

	GraphicsPipeline::GraphicsPipeline() :
		_passes(MallocAllocator("Vulkan Render Pipeline Stage List Allocator")),
		_descriptorLayout(nullptr),
		_timingPool(nullptr) {}

	// ---------------------------------------------------

	GraphicsPipeline::GraphicsPipeline(GraphicsPipeline&& pipeline) :
		GraphicsPipeline() {
		Swap(*this, pipeline);
	}

	// ---------------------------------------------------

	GraphicsPipeline::~GraphicsPipeline() {
		ET_ASSERT(_descriptorLayout == nullptr, "Leaking Vulkan descriptor set layout!");
		ET_ASSERT(_timingPool == nullptr, "Leaking Vulkan query pool!");
	}

	// ---------------------------------------------------

	VkResult GraphicsPipeline::BindResources(Gpu& gpu, const GraphicsPipelineBuilder& builder) {
		using ::Eldritch2::Swap;

		static const VkDescriptorSetLayoutBinding descriptorBindings[] = {
			VkDescriptorSetLayoutBinding{
				/*binding =*/0u,
				VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
				/*descriptorCount =*/512u,
				VK_SHADER_STAGE_ALL,
				/*pImmutableSamplers =*/nullptr }
		};

		if (builder.GetPassCount() == 0u) {
			return VK_ERROR_INITIALIZATION_FAILED;
		}

		VkDescriptorSetLayout                 descriptorLayout;
		const VkDescriptorSetLayoutCreateInfo descriptorLayoutInfo{
			VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
			/*pNext =*/nullptr,
			/*flags =*/0u,
			static_cast<uint32_t>(_countof(descriptorBindings)), descriptorBindings
		};
		ET_FAIL_UNLESS(vkCreateDescriptorSetLayout(gpu, &descriptorLayoutInfo, gpu.GetAllocationCallbacks(), &descriptorLayout));
		ET_AT_SCOPE_EXIT(vkDestroyDescriptorSetLayout(gpu, descriptorLayout, gpu.GetAllocationCallbacks()));

		VkQueryPool                 timingPool;
		const VkQueryPoolCreateInfo timingPoolInfo{
			VK_STRUCTURE_TYPE_QUERY_POOL_CREATE_INFO,
			/*pNext =*/nullptr,
			/*flags =*/0u,
			VK_QUERY_TYPE_TIMESTAMP,
			/*queryCount =*/uint32(2u * builder.GetPassCount()), // Start/stop timestamp for each pass.
			/*pipelineStatistics =*/0u                           // Timestamp queries do not have flags.
		};
		ET_FAIL_UNLESS(vkCreateQueryPool(gpu, &timingPoolInfo, gpu.GetAllocationCallbacks(), &timingPool));
		ET_AT_SCOPE_EXIT(vkDestroyQueryPool(gpu, timingPool, gpu.GetAllocationCallbacks()));

		ArrayList<Pass> passes(_passes.GetAllocator());
		ET_AT_SCOPE_EXIT(for (Pass& pass
							  : passes) pass.FreeResources(gpu));

		passes.Resize(builder.GetPassCount());
		for (uint32 pass(0u); pass < builder.GetPassCount(); ++pass) {
			ET_FAIL_UNLESS(passes[pass].BindResources(gpu, builder));
		}

		Swap(_descriptorLayout, descriptorLayout);
		Swap(_timingPool, timingPool);
		Swap(_passes, passes);

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void GraphicsPipeline::FreeResources(Gpu& gpu) {
		vkDestroyQueryPool(gpu, eastl::exchange(_timingPool, nullptr), gpu.GetAllocationCallbacks());
		vkDestroyDescriptorSetLayout(gpu, eastl::exchange(_descriptorLayout, nullptr), gpu.GetAllocationCallbacks());
		for (Pass& pass : _passes) {
			pass.FreeResources(gpu);
		}

		_passes.Clear(ReleaseMemorySemantics());
	}

	// ---------------------------------------------------

	void Swap(GraphicsPipeline& lhs, GraphicsPipeline& rhs) {
		using ::Eldritch2::Swap;

		Swap(lhs._passes, rhs._passes);
		Swap(lhs._descriptorLayout, rhs._descriptorLayout);
		Swap(lhs._timingPool, rhs._timingPool);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
