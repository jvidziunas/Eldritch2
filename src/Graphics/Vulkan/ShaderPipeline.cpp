/*==================================================================*\
  ShaderPipeline.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/AssetViews/SpirVShaderSetAsset.hpp>
#include <Graphics/Vulkan/GraphicsPipeline.hpp>
#include <Graphics/Vulkan/ShaderPipeline.hpp>
#include <Graphics/Vulkan/VulkanTools.hpp>
#include <Graphics/Vulkan/Gpu.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	using namespace ::Eldritch2::Graphics::Vulkan::AssetViews;

	namespace {

		static const VkDynamicState dynamicStates[] = {
			VK_DYNAMIC_STATE_VIEWPORT,
			VK_DYNAMIC_STATE_SCISSOR,
			VK_DYNAMIC_STATE_DEPTH_BIAS,
			VK_DYNAMIC_STATE_STENCIL_COMPARE_MASK,
			VK_DYNAMIC_STATE_STENCIL_WRITE_MASK,
			VK_DYNAMIC_STATE_STENCIL_REFERENCE
		};

		static const VkPipelineDynamicStateCreateInfo dynamicStateInfo{
			VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
			/*pNext =*/nullptr,
			/*flags =*/0u,
			_countof(dynamicStates), dynamicStates
		};

	} // anonymous namespace

	ShaderPipeline::ShaderPipeline() :
		_layout(nullptr),
		_pipelinesByPass(MallocAllocator("Vulkan Shader Pipeline List Allocator")) {}

	// ---------------------------------------------------

	ShaderPipeline::ShaderPipeline(ShaderPipeline&& pipeline) :
		ShaderPipeline() {
		Swap(*this, pipeline);
	}

	// ---------------------------------------------------

	ShaderPipeline::~ShaderPipeline() {
		ET_ASSERT(_layout == nullptr, "Leaking Vulkan pipeline layout!");
		ET_ASSERT(_pipelinesByPass.IsEmpty(), "Leaking Vulkan graphics pipelines!");
	}

	// ---------------------------------------------------

	VkResult ShaderPipeline::BindResources(Gpu& gpu, GraphicsPipeline& context, const SpirVShaderSetAsset& asset) {
		using ::Eldritch2::Swap;

		static const VkPipelineVertexInputStateCreateInfo vertexInputStateInfo{};
		const VkDescriptorSetLayout                       descriptorLayouts[] = { context.GetDescriptorLayout() };
		const VkPushConstantRange                         constantRanges[]    = { VkPushConstantRange{
            VK_SHADER_STAGE_ALL_GRAPHICS,
            /*offset =*/0u,
            /*size =*/128 } };

		VkPipelineLayout                 layout;
		const VkPipelineLayoutCreateInfo layoutInfo{
			VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
			/*pNext =*/nullptr,
			/*flags =*/0u,
			_countof(descriptorLayouts), descriptorLayouts,
			_countof(constantRanges), constantRanges
		};
		ET_FAIL_UNLESS(vkCreatePipelineLayout(gpu, &layoutInfo, gpu.GetAllocationCallbacks(), &layout));
		ET_AT_SCOPE_EXIT(vkDestroyPipelineLayout(gpu, layout, gpu.GetAllocationCallbacks()));

		ArrayList<VkGraphicsPipelineCreateInfo> pipelineInfos(MallocAllocator("Vulkan Shader Pipeline Info List Allocator"));
		for (uint32 subpass(0u); subpass < context.GetPassCount(); ++subpass) {
			const auto usage(asset.Find("lol"));
			if (false) {
				VkShaderModule                 shader;
				const VkShaderModuleCreateInfo shaderInfo{
					VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
					/*pNext =*/nullptr,
					/*flags =*/0u,
					asset.GetBytecodeSize(),
					asset.GetBytecode()
				};
				ET_FAIL_UNLESS(vkCreateShaderModule(gpu, &shaderInfo, gpu.GetAllocationCallbacks(), &shader));
				ET_AT_SCOPE_EXIT(vkDestroyShaderModule(gpu, shader, gpu.GetAllocationCallbacks()));
				const bool isFirstPipeline(pipelineInfos.IsEmpty());

				pipelineInfos.Append(VkGraphicsPipelineCreateInfo{
					VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
					/*pNext =*/nullptr,
					/*flags =*/VkPipelineCreateFlags(isFirstPipeline ? VK_PIPELINE_CREATE_ALLOW_DERIVATIVES_BIT : VK_PIPELINE_CREATE_DERIVATIVE_BIT),
					/*stageCount =*/0u,
					/*pStages =*/nullptr,
					eastl::addressof(vertexInputStateInfo),
					/*pInputAssemblyState =*/nullptr,
					/*pTessellationState =*/nullptr,
					/*pViewportState =*/nullptr, // Viewport state is dynamic.
					eastl::addressof(usage->rasterizationInfo),
					eastl::addressof(usage->multisampleInfo),
					eastl::addressof(usage->depthStencilInfo),
					eastl::addressof(usage->colorBlendInfo),
					eastl::addressof(dynamicStateInfo),
					layout,
					context,
					subpass,
					/*basePipelineHandle =*/VK_NULL_HANDLE,
					/*basePipelineIndex =*/isFirstPipeline ? ~0 : 0 });
			}
		}

		ArrayList<VkPipeline> pipelines(_pipelinesByPass.GetAllocator());
		ET_AT_SCOPE_EXIT(for (VkPipeline pipeline
							  : pipelines) vkDestroyPipeline(gpu, pipeline, gpu.GetAllocationCallbacks()));
		ET_FAIL_UNLESS(vkCreateGraphicsPipelines(gpu, gpu.GetPipelineCache(), pipelineInfos.GetSize(), pipelineInfos.GetData(), gpu.GetAllocationCallbacks(), pipelines.GetData()));

		Swap(_layout, layout);
		Swap(_pipelinesByPass, pipelines);

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void ShaderPipeline::FreeResources(Gpu& gpu) {
		for (VkPipeline pipeline : _pipelinesByPass) {
			vkDestroyPipeline(gpu, pipeline, gpu.GetAllocationCallbacks());
		}

		_pipelinesByPass.Clear(ReleaseMemorySemantics());
	}

	// ---------------------------------------------------

	void Swap(ShaderPipeline& lhs, ShaderPipeline& rhs) {
		using ::Eldritch2::Swap;

		Swap(lhs._layout, rhs._layout);
		Swap(lhs._pipelinesByPass, rhs._pipelinesByPass);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
