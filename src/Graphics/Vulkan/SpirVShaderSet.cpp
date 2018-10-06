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
#include <Graphics/Vulkan/GraphicsPipeline.hpp>
#include <Graphics/Vulkan/SpirVShaderSet.hpp>
#include <Graphics/Vulkan/VulkanTools.hpp>
#include <Graphics/Vulkan/Gpu.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	ShaderPipeline::Shader::Shader(Shader&& usage) ETNoexceptHint : Shader() {
		Swap(*this, usage);
	}

	// ---------------------------------------------------

	ShaderPipeline::Shader::Shader() ETNoexceptHint {
		Fill(_modules, End(_modules), nullptr);
	}

	// ---------------------------------------------------

	ShaderPipeline::Shader::~Shader() {
		for (VkShaderModule module : _modules) {
			ET_ASSERT(module == nullptr, "Leaking Vulkan shader module {}!", fmt::ptr(module));
		}
	}

	// ---------------------------------------------------

	VkResult ShaderPipeline::Shader::BindResources(Gpu& gpu, const SpirVShader& shader) {
		using ::Eldritch2::Swap;

		static_assert(ETCountOf(shader.bytecodeByStage) == ETCountOf(_modules), "");
		VkShaderModule  modules[ETCountOf(_modules)];
		VkShaderModule* modulesEnd;
		ET_AT_SCOPE_EXIT(for (VkShaderModule module
							  : modules) vkDestroyShaderModule(gpu, module, gpu.GetAllocationCallbacks()));
		for (Range<const uint32*> bytecode : shader.bytecodeByStage) {
			if (bytecode.IsEmpty()) {
				continue;
			}

			const VkShaderModuleCreateInfo shaderInfo {
				VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
				/*pNext =*/nullptr,
				/*flags =*/0u,
				bytecode.GetSize() * sizeof(uint32),
				bytecode.Begin()
			};
			ET_ABORT_UNLESS(vkCreateShaderModule(gpu, ETAddressOf(shaderInfo), gpu.GetAllocationCallbacks(), modulesEnd++));
		}

		Swap(_modules, modules);

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void ShaderPipeline::Shader::FreeResources(Gpu& gpu) {
		for (VkShaderModule& module : _modules) {
			vkDestroyShaderModule(gpu, eastl::exchange(module, nullptr), gpu.GetAllocationCallbacks());
		}
	}

	// ---------------------------------------------------

	ShaderPipeline::ShaderPipeline() :
		_pipelineByPass(MallocAllocator("Vulkan Shader Pipeline By Pass List Allocator")) {}

	// ---------------------------------------------------

	ShaderPipeline::ShaderPipeline(ShaderPipeline&& pipeline) :
		ShaderPipeline() {
		Swap(*this, pipeline);
	}

	// ---------------------------------------------------

	ShaderPipeline::~ShaderPipeline() {
		ET_ASSERT(_pipelineByPass.IsEmpty(), "Leaking Vulkan shader pipelines!");
	}

	// ---------------------------------------------------

	VkResult ShaderPipeline::BindResources(Gpu& gpu, const GraphicsPipeline& pipeline, const SpirVShaderSet& shaders) {
		using ::Eldritch2::Swap;

		static const VkPushConstantRange constantRanges[] = { VkPushConstantRange {
			VK_SHADER_STAGE_ALL_GRAPHICS,
			/*offset =*/0u,
			/*size =*/128u } };

		VkPipelineLayout                 layout;
		const VkPipelineLayoutCreateInfo layoutInfo {
			VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
			/*pNext =*/nullptr,
			/*flags =*/0u,
			ETCountOf(pipeline.GetShaderResources().GetDescriptorSetLayouts()), pipeline.GetShaderResources().GetDescriptorSetLayouts(),
			ETCountOf(constantRanges), constantRanges
		};
		ET_ABORT_UNLESS(vkCreatePipelineLayout(gpu, ETAddressOf(layoutInfo), gpu.GetAllocationCallbacks(), ETAddressOf(layout)));
		ET_AT_SCOPE_EXIT(vkDestroyPipelineLayout(gpu, layout, gpu.GetAllocationCallbacks()));

		const auto                                                    passIndexByName(IndexPipeline(pipeline.GetPasses()));
		SoArrayList<VkPipeline, VkGraphicsPipelineCreateInfo, Shader> pendingPipelines(MallocAllocator(""), Min(shaders.GetSize(), pipeline.GetPasses().GetSize()));
		for (const SpirVShader& shader : shaders) {
			pendingPipelines.EmplaceBack(VK_NULL_HANDLE, VkGraphicsPipelineCreateInfo {}, Shader {});
		}

		ET_ABORT_UNLESS(vkCreateGraphicsPipelines(gpu, gpu.GetPipelineCache(), uint32(pendingPipelines.GetSize()), pendingPipelines.GetData<VkGraphicsPipelineCreateInfo>(), gpu.GetAllocationCallbacks(), pendingPipelines.GetData<VkPipeline>()));

		SoArrayList<VkPipeline, Shader> pipelineByPass(_pipelineByPass.GetAllocator(), pipeline.GetPasses().GetSize());
		ET_AT_SCOPE_EXIT(for (SoArrayList<VkPipeline, Shader>::Reference pipeline
							  : pipelineByPass) {
			vkDestroyPipeline(gpu, eastl::get<VkPipeline&>(pipeline), gpu.GetAllocationCallbacks());
			eastl::get<Shader&>(pipeline).FreeResources(gpu);
		});

		Swap(_layout, layout);
		Swap(_pipelineByPass, pipelineByPass);

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void ShaderPipeline::FreeResources(Gpu& gpu) {
		for (SoArrayList<VkPipeline, Shader>::Reference pipeline : _pipelineByPass) {
			vkDestroyPipeline(gpu, eastl::get<VkPipeline&>(pipeline), gpu.GetAllocationCallbacks());
			eastl::get<Shader&>(pipeline).FreeResources(gpu);
		}

		_pipelineByPass.Clear();
	}

	// ---------------------------------------------------

	void Swap(ShaderPipeline::Shader& lhs, ShaderPipeline::Shader& rhs) ETNoexceptHint {
		using ::Eldritch2::Swap;

		Swap(lhs._modules, rhs._modules);
	}

	// ---------------------------------------------------

	void Swap(ShaderPipeline& lhs, ShaderPipeline& rhs) {
		using ::Eldritch2::Swap;

		Swap(lhs._layout, rhs._layout);
		Swap(lhs._pipelineByPass, rhs._pipelineByPass);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
