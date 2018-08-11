/*==================================================================*\
  GraphicsPipeline.ShaderPipeline.cpp
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
#include <Graphics/Vulkan/VulkanTools.hpp>
#include <Graphics/Vulkan/Gpu.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	using namespace ::Eldritch2::Graphics::Vulkan::AssetViews;

	namespace {

		ETInlineHint VkResult CreatePipeline(Gpu& gpu, const VkSpecializationInfo& specializations, VkRenderPass renderPass, VkPipelineLayout layout, VkPipeline base, const SpirVShaderSetAsset::Usage& asset, ShaderPipeline::Usage& usage, VkPipeline& outPipeline) {
			static const VkDynamicState dynamicStates[] = {
				VK_DYNAMIC_STATE_VIEWPORT,
				VK_DYNAMIC_STATE_SCISSOR,
				VK_DYNAMIC_STATE_DEPTH_BIAS,
				VK_DYNAMIC_STATE_STENCIL_COMPARE_MASK,
				VK_DYNAMIC_STATE_STENCIL_WRITE_MASK,
				VK_DYNAMIC_STATE_STENCIL_REFERENCE
			};
			static const VkPipelineDynamicStateCreateInfo dynamicStateInfo {
				VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
				/*pNext =*/nullptr,
				/*flags =*/0u,
				_countof(dynamicStates), dynamicStates
			};
			static const VkPipelineVertexInputStateCreateInfo vertexInputInfo {};
			VkPipelineShaderStageCreateInfo                   stages[_countof(ShaderPipeline::Usage::shaders)];
			char                                              entryPoints[32u][_countof(stages)];

			for (uint32 shader(0u); shader < _countof(stages); ++shader) {
				stages[shader] = VkPipelineShaderStageCreateInfo {
					VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
					/*pNext =*/nullptr,
					/*flags =*/0u,
					/*stage =*/VkShaderStageFlagBits(0u),
					usage.shaders[shader],
					/*pName =*/entryPoints[shader],
					&specializations
				};
			}

			const VkGraphicsPipelineCreateInfo pipelineInfo {
				VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
				/*pNext =*/nullptr,
				/*flags =*/VkPipelineCreateFlags(base ? VK_PIPELINE_CREATE_ALLOW_DERIVATIVES_BIT : VK_PIPELINE_CREATE_DERIVATIVE_BIT),
				/*stageCount =*/0u,
				/*pStages =*/stages,
				eastl::addressof(vertexInputInfo),
				/*pInputAssemblyState =*/nullptr,
				/*pTessellationState =*/nullptr,
				/*pViewportState =*/nullptr, // Viewport state is dynamic.
				eastl::addressof(asset.rasterizationInfo),
				eastl::addressof(asset.multisampleInfo),
				eastl::addressof(asset.depthStencilInfo),
				eastl::addressof(asset.colorBlendInfo),
				eastl::addressof(dynamicStateInfo),
				layout,
				renderPass,
				/*subpass=*/0u,
				/*basePipelineHandle =*/base,
				/*basePipelineIndex =*/-1
			};

			return vkCreateGraphicsPipelines(gpu, gpu.GetPipelineCache(), 1u, &pipelineInfo, gpu.GetAllocationCallbacks(), &outPipeline);
		}

	} // anonymous namespace

	ShaderPipeline::Usage::Usage(Usage&& usage) :
		Usage() {
		Swap(*this, usage);
	}

	// ---------------------------------------------------

	ShaderPipeline::Usage::Usage() :
		shaders { nullptr } {}

	// ---------------------------------------------------

	ShaderPipeline::Usage::~Usage() {
		for (VkShaderModule shader : shaders) {
			ET_ASSERT(shader == nullptr, "Leaking Vulkan shader module!");
		}
	}

	// ---------------------------------------------------

	VkResult ShaderPipeline::Usage::BindResources(Gpu& gpu, const SpirVShaderSetAsset& set, uint32 usageIndex) {
		using ::Eldritch2::Swap;

		const SpirVShaderSetAsset::Usage& usage(set[usageIndex]);
		VkShaderModule                    shaders[_countof(this->shaders)];
		uint32                            shaderCount(0u);

		ET_AT_SCOPE_EXIT(for (VkShaderModule shader
							  : shaders) vkDestroyShaderModule(gpu, shader, gpu.GetAllocationCallbacks()));
		for (VkShaderStageFlagBits stage : { VK_SHADER_STAGE_VERTEX_BIT, VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT, VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT, VK_SHADER_STAGE_GEOMETRY_BIT, VK_SHADER_STAGE_FRAGMENT_BIT }) {
			if ((usage.enabledShaders & stage) == 0u) {
				continue;
			}

			const VkShaderModuleCreateInfo shaderInfo {
				VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
				/*pNext =*/nullptr,
				/*flags =*/0u,
				usage.GetBytecodeSizeInBytes(),
				usage.GetBytecode()
			};
			ET_FAIL_UNLESS(vkCreateShaderModule(gpu, &shaderInfo, gpu.GetAllocationCallbacks(), &shaders[shaderCount++]));
		}

		Swap(this->shaders, shaders);

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void ShaderPipeline::Usage::FreeResources(Gpu& gpu) {
		for (VkShaderModule& shader : shaders) {
			vkDestroyShaderModule(gpu, eastl::exchange(shader, nullptr), gpu.GetAllocationCallbacks());
		}
	}

	// ---------------------------------------------------

	ShaderPipeline::ShaderPipeline() :
		_pipelinesByPass(MallocAllocator("Vulkan Shader Pipeline List Allocator")),
		_usagesByPass(MallocAllocator("Vulkan Shader Pipeline Usage List Allocator")) {}

	// ---------------------------------------------------

	ShaderPipeline::ShaderPipeline(ShaderPipeline&& pipeline) :
		ShaderPipeline() {
		Swap(*this, pipeline);
	}

	// ---------------------------------------------------

	ShaderPipeline::~ShaderPipeline() {
		ET_ASSERT(_pipelinesByPass.IsEmpty(), "Leaking Vulkan graphics pipelines!");
	}

	// ---------------------------------------------------

	VkResult ShaderPipeline::BindResources(Gpu& gpu, GraphicsPipeline& pipeline, const SpirVShaderSetAsset& set) {
		using ::Eldritch2::Swap;

		const VkDescriptorSetLayout      layouts[]        = { pipeline.GetDescriptorLayout() };
		static const VkPushConstantRange constantRanges[] = { VkPushConstantRange {
			VK_SHADER_STAGE_ALL_GRAPHICS,
			/*offset =*/0u,
			/*size =*/128 } };

		VkPipelineLayout                 pipelineLayout;
		const VkPipelineLayoutCreateInfo pipelineLayoutInfo {
			VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
			/*pNext =*/nullptr,
			/*flags =*/0u,
			_countof(layouts), layouts,
			_countof(constantRanges), constantRanges
		};
		ET_FAIL_UNLESS(vkCreatePipelineLayout(gpu, &pipelineLayoutInfo, gpu.GetAllocationCallbacks(), &pipelineLayout));
		ET_AT_SCOPE_EXIT(vkDestroyPipelineLayout(gpu, pipelineLayout, gpu.GetAllocationCallbacks()));

		ArrayList<VkPipeline> pipelinesByPass(_pipelinesByPass.GetAllocator());
		ArrayList<Usage>      usagesByPass(_usagesByPass.GetAllocator());
		ET_AT_SCOPE_EXIT(for (VkPipeline pipeline
							  : pipelinesByPass) vkDestroyPipeline(gpu, pipeline, gpu.GetAllocationCallbacks()));
		ET_AT_SCOPE_EXIT(for (Usage& usage
							  : usagesByPass) usage.FreeResources(gpu));

		VkPipeline                 basePipeline(nullptr);
		const VkSpecializationInfo specializations {
			/*mapEntryCount =*/0u,
			/*pMapEntries =*/nullptr,
			/*dataSize =*/0u,
			/*pData =*/nullptr,
		};

		const GraphicsPipeline::PassDescription* const passes(pipeline.GetPasses());
		for (uint32 pass(0u); pass < pipeline.GetPassCount(); ++pass) {
			const uint32 usage(set.Find(passes[pass].name));
			if (usage == SpirVShaderSetAsset::InvalidUsageId) {
				// Asset doesn't participate in this type of draw operation. Signal with null pipeline handle.
				continue;
			}

			ET_FAIL_UNLESS(usagesByPass[pass].BindResources(gpu, set, usage));
			ET_FAIL_UNLESS(CreatePipeline(gpu, specializations, passes[pass].renderPass, pipelineLayout, basePipeline, set[usage], usagesByPass[pass], pipelinesByPass[pass]));
			if (basePipeline == nullptr) {
				basePipeline = pipelinesByPass[pass];
			}
		}

		Swap(_pipelineLayout, pipelineLayout);
		Swap(_pipelinesByPass, pipelinesByPass);
		Swap(_usagesByPass, usagesByPass);

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void ShaderPipeline::FreeResources(Gpu& gpu) {
		for (Usage& usage : _usagesByPass) {
			usage.FreeResources(gpu);
		}

		for (VkPipeline pipeline : _pipelinesByPass) {
			vkDestroyPipeline(gpu, pipeline, gpu.GetAllocationCallbacks());
		}

		_pipelinesByPass.Clear(ReleaseMemorySemantics());
		_usagesByPass.Clear(ReleaseMemorySemantics());
	}

	// ---------------------------------------------------

	void Swap(ShaderPipeline::Usage& lhs, ShaderPipeline::Usage& rhs) {
		using ::Eldritch2::Swap;

		Swap(lhs.shaders, rhs.shaders);
	}

	// ---------------------------------------------------

	void Swap(ShaderPipeline& lhs, ShaderPipeline& rhs) {
		using ::Eldritch2::Swap;

		Swap(lhs._pipelineLayout, rhs._pipelineLayout);
		Swap(lhs._pipelinesByPass, rhs._pipelinesByPass);
		Swap(lhs._usagesByPass, rhs._usagesByPass);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
