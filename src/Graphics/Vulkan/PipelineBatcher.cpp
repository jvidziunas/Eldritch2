/*==================================================================*\
  PipelineBatcher.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/PipelineBatcher.hpp>
#include <Graphics/Vulkan/SpirVShaderSet.hpp>
#include <Graphics/Vulkan/VulkanTools.hpp>
#include <Graphics/PipelineBuilder.hpp>
#include <Graphics/Vulkan/Gpu.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	VkResult PipelineBatcher::CommandPool::BindResources(Gpu& gpu) {
		using ::Eldritch2::Swap;

		VkCommandPool                 pool;
		const VkCommandPoolCreateInfo poolInfo{
			VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
			/*pNext =*/nullptr,
			/*flags =*/0u,
			gpu.GetQueueFamilyByClass(Drawing)
		};
		ET_ABORT_UNLESS(vkCreateCommandPool(gpu, ETAddressOf(poolInfo), gpu.GetAllocationCallbacks(), ETAddressOf(pool)));
		ET_AT_SCOPE_EXIT(vkDestroyCommandPool(gpu, pool, gpu.GetAllocationCallbacks()));

		Swap(this->pool, pool);

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void PipelineBatcher::CommandPool::FreeResources(Gpu& gpu) ETNoexceptHint {
		vkDestroyCommandPool(gpu, Exchange(pool, NullVulkanHandle), gpu.GetAllocationCallbacks());
	}

	// ---------------------------------------------------

	PipelineBatcher::PipelineBatcher() ETNoexceptHint : _cache(NullVulkanHandle),
														_layout(NullVulkanHandle),
														_stages(StageList::AllocatorType("Pipeline Batcher Stage List Allocator")),
														_commandPools(CommandPoolList::AllocatorType("Pipeline Batcher Command Pool List Allocator")),
														_pipelineCommands(PipelineCommandBufferList::AllocatorType("Pipeline Batcher Commands List Allocator")) {}

	// ---------------------------------------------------

	PipelineBatcher::PipelineBatcher(PipelineBatcher&& batches) ETNoexceptHint : PipelineBatcher() {
		Swap(*this, batches);
	}

	// ---------------------------------------------------

	VkResult PipelineBatcher::Compile(Gpu& gpu, const SpirVShaderSet& shaders, uint32 constantsByteSize, const byte constants[]) {
		using CompiledPipelineList = ArrayMap<StageIndex, VkPipeline>;

		const auto CompilePipelines([&](CompiledPipelineList& outPipelines, StageList::ConstSliceType stages) -> VkResult {
			enum : size_t {
				Pipelines   = 0u,
				CreateInfos = 2u,
			};

			SoaList<VkPipeline, StageIndex, VkGraphicsPipelineCreateInfo> graphicsPipelines(MallocAllocator("Graphics Pipeline List Allocator"), Minimum(shaders.GetSize(), stages.GetSize()));
			ET_AT_SCOPE_EXIT(graphicsPipelines.ClearAndDispose([&](VkPipeline pipeline, PipelineBatcher::StageIndex /*stage*/, const VkGraphicsPipelineCreateInfo& /*createInfo*/) ETNoexceptHint {
				vkDestroyPipeline(gpu, pipeline, gpu.GetAllocationCallbacks());
			}));

			StageIndex stageIndex(0u);
			for (StageList::ConstReference stage : stages) {
				SpirVShaderSet::ConstIterator shader(shaders.Find(Get<PassNames>(stage)));
				if (shader == shaders.End()) {
					continue;
				}

				const auto index(stageIndex++);
				graphicsPipelines.Append(NullVulkanHandle, index, VkGraphicsPipelineCreateInfo{ VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
																								/*pNext =*/nullptr,
																								/*flags =*/VkPipelineCreateFlags(graphicsPipelines.IsEmpty() ? VK_PIPELINE_CREATE_ALLOW_DERIVATIVES_BIT : VK_PIPELINE_CREATE_DERIVATIVE_BIT),
																								/*stageCount =*/0u,
																								/*pStages =*/nullptr,
																								/*pVertexInputState =*/nullptr,
																								/*pInputAssemblyState =*/nullptr,
																								/*pTessellationState =*/nullptr,
																								/*pViewportState =*/nullptr,
																								/*pRasterizationState =*/nullptr,
																								/*pMultisampleState =*/nullptr,
																								/*pDepthStencilState =*/nullptr,
																								/*pColorBlendState =*/nullptr,
																								/*pDynamicState =*/nullptr, _layout,
																								/*renderPass =*/Get<RenderPasses>(stage),
																								/*subpass =*/0u,
																								/*basePipelineHandle =*/NullVulkanHandle,
																								/*basePipelineIndex =*/graphicsPipelines.IsEmpty() ? -1 : 0 });
			}

			CompiledPipelineList pipelines(CompiledPipelineList::AllocatorType("lol"), LessThan<StageIndex>(), Minimum(shaders.GetSize(), stages.GetSize()));
			ET_ABORT_UNLESS(vkCreateGraphicsPipelines(gpu, _cache, graphicsPipelines.GetSize(), graphicsPipelines.GetData<CreateInfos>(), gpu.GetAllocationCallbacks(), graphicsPipelines.GetData<Pipelines>()));

			return VK_SUCCESS;
		});

		// ---

		ETAssert(constantsByteSize == 0u || constants, "constants must be non-null if constantsByteSize != 0 (constantsByteSize = {})", constantsByteSize);

		CompiledPipelineList pipelines;

		ET_ABORT_UNLESS(CompilePipelines(pipelines, _stages));
		for (CompiledPipelineList::ConstReference pipeline : pipelines) {
			CommandPool&                      pool(FindPool(pipeline.second));
			VkCommandBuffer                   commands;
			const VkCommandBufferAllocateInfo commandsInfo{
				VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
				/*pNext =*/nullptr,
				pool.pool,
				VK_COMMAND_BUFFER_LEVEL_SECONDARY,
				/*commandBufferCount =*/1u
			};
			ET_ABORT_UNLESS(vkAllocateCommandBuffers(gpu, ETAddressOf(commandsInfo), ETAddressOf(commands)));
			ET_AT_SCOPE_EXIT(if (commands) vkFreeCommandBuffers(gpu, pool.pool, /*commandBufferCount =*/1u, ETAddressOf(commands)));

			const VkCommandBufferInheritanceInfo inheritanceInfo{
				VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO,
				/*pNext =*/nullptr,
				/*renderPass =*/NullVulkanHandle,
				/*subpass =*/0u,
				/*framebuffer =*/NullVulkanHandle,
				/*occlusionQueryEnable =*/VkBool32(VK_FALSE),
				/*queryFlags =*/0u,
				/*pipelineStatistics =*/0u
			};
			const VkCommandBufferBeginInfo beginInfo{
				VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
				/*pNext =*/nullptr,
				/*flags =*/VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT,
				ETAddressOf(inheritanceInfo)
			};
			ET_ABORT_UNLESS(vkBeginCommandBuffer(commands, ETAddressOf(beginInfo)));
			vkCmdBindPipeline(commands, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.second);
			vkCmdPushConstants(commands, _layout, VK_SHADER_STAGE_ALL, /*offset =*/0u, constantsByteSize, constants);
			// vkCmdDrawIndexedIndirect(commands, _drawParameters, /*offset =*/0u, /*drawCount =*/1u, /*stride =*/sizeof(VkDrawIndexedIndirectCommand));
			ET_ABORT_UNLESS(vkEndCommandBuffer(commands));
		}

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	VkResult PipelineBatcher::BindResources(Gpu& gpu, Span<const byte*> cacheSeed, const PipelineBuilder& builder, CommandPoolList::SizeType poolCount) {
		using ::Eldritch2::Swap;

		ETAssert(poolCount > 0, "commandPoolCount must be > 0!");

		VkPipelineCache                 cache;
		const VkPipelineCacheCreateInfo cacheInfo{
			VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO,
			/*pNext =*/nullptr,
			/*flags =*/0u,
			cacheSeed.GetSize(),
			cacheSeed.Begin()
		};
		ET_ABORT_UNLESS(vkCreatePipelineCache(gpu, ETAddressOf(cacheInfo), gpu.GetAllocationCallbacks(), ETAddressOf(cache)));
		ET_AT_SCOPE_EXIT(vkDestroyPipelineCache(gpu, cache, gpu.GetAllocationCallbacks()));

		const VkPushConstantRange constantRanges[] = {
			VkPushConstantRange{
				// Pipeline constants
				/*stageFlags =*/VK_SHADER_STAGE_ALL_GRAPHICS,
				/*offset =*/0u,
				/*size =*/0u,
			},
			VkPushConstantRange{
				// Stage constants
				/*stageFlags =*/VK_SHADER_STAGE_ALL_GRAPHICS, // FIXME: This should be determined from stage type
				/*offset =*/0u,
				/*size =*/0u,
			},
			VkPushConstantRange{
				// Draw constants
				/*stageFlags =*/VK_SHADER_STAGE_ALL_GRAPHICS, // FIXME: This should be determined from stage type
				/*offset =*/0u,
				/*size =*/0u,
			},
		};
		VkPipelineLayout                 layout;
		const VkPipelineLayoutCreateInfo layoutInfo{
			VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
			/*pNext =*/nullptr,
			/*flags =*/0u,
			/*setLayoutCount =*/0u,
			/*pSetLayouts =*/nullptr,
			ETCountOf(constantRanges), constantRanges
		};
		ET_ABORT_UNLESS(vkCreatePipelineLayout(gpu, ETAddressOf(layoutInfo), gpu.GetAllocationCallbacks(), ETAddressOf(layout)));
		ET_AT_SCOPE_EXIT(vkDestroyPipelineLayout(gpu, layout, gpu.GetAllocationCallbacks()));

		CommandPoolList commandPools(_commandPools.GetAllocator(), poolCount);
		ET_AT_SCOPE_EXIT(commandPools.ClearAndDispose([&](CommandPool& pool) ETNoexceptHint { pool.FreeResources(gpu); }));
		while (poolCount--) {
			ET_ABORT_UNLESS(commandPools.EmplaceBack().BindResources(gpu));
		}

		StageList stages(_stages.GetAllocator(), builder.GetStages().GetSize());
		ET_AT_SCOPE_EXIT(stages.ClearAndDispose([&](const String& /*name*/, VkRenderPass pass) ETNoexceptHint { vkDestroyRenderPass(gpu, pass, gpu.GetAllocationCallbacks()); }));
		for (const StageBuilder& source : builder.GetStages()) {
			StageList::Reference         stage(stages.EmplaceBack(String(String::AllocatorType("Render Pass Name Allocator"), StringSpan(source.shader)), NullVulkanHandle));
			const VkRenderPassCreateInfo renderPassInfo{
				VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
				/*pNext =*/nullptr,
			};
			ET_ABORT_UNLESS(vkCreateRenderPass(gpu, ETAddressOf(renderPassInfo), gpu.GetAllocationCallbacks(), ETAddressOf(Get<RenderPasses>(stage))));
		}

		Swap(_cache, cache);
		Swap(_layout, layout);
		Swap(_commandPools, commandPools);
		Swap(_stages, stages);

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void PipelineBatcher::FreeResources(Gpu& gpu) ETNoexceptHint {
		_stages.ClearAndDispose([&](const String& /*name*/, VkRenderPass pass) ETNoexceptHint { vkDestroyRenderPass(gpu, pass, gpu.GetAllocationCallbacks()); });
		_commandPools.ClearAndDispose([&](CommandPool& pool) ETNoexceptHint { pool.FreeResources(gpu); });
		vkDestroyPipelineLayout(gpu, Exchange(_layout, NullVulkanHandle), gpu.GetAllocationCallbacks());
		vkDestroyPipelineCache(gpu, Exchange(_cache, NullVulkanHandle), gpu.GetAllocationCallbacks());
	}

	// ---------------------------------------------------

	void Swap(PipelineBatcher& lhs, PipelineBatcher& rhs) ETNoexceptHint {
		using ::Eldritch2::Swap;

		Swap(lhs._cache, rhs._cache);
		Swap(lhs._layout, rhs._layout);
		Swap(lhs._stages, rhs._stages);
		Swap(lhs._commandPools, rhs._commandPools);
		Swap(lhs._pipelineCommands, rhs._pipelineCommands);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
