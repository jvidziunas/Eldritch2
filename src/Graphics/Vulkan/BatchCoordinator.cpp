/*==================================================================*\
  BatchCoordinator.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/GraphicsPipelineBuilder.hpp>
#include <Graphics/Vulkan/BatchCoordinator.hpp>
#include <Graphics/Vulkan/SpirVShaderSet.hpp>
#include <Graphics/Vulkan/VulkanTools.hpp>
#include <Graphics/Vulkan/Gpu.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	namespace {

		static ETConstexpr VkDynamicState fixedResolutionDynamicStates[] = {
			VK_DYNAMIC_STATE_SCISSOR,
			VK_DYNAMIC_STATE_DEPTH_BIAS,
			VK_DYNAMIC_STATE_STENCIL_COMPARE_MASK,
			VK_DYNAMIC_STATE_STENCIL_WRITE_MASK,
			VK_DYNAMIC_STATE_STENCIL_REFERENCE
		};

		static ETConstexpr VkDynamicState dynamicStates[] = {
			VK_DYNAMIC_STATE_VIEWPORT,
			VK_DYNAMIC_STATE_SCISSOR,
			VK_DYNAMIC_STATE_DEPTH_BIAS,
			VK_DYNAMIC_STATE_STENCIL_COMPARE_MASK,
			VK_DYNAMIC_STATE_STENCIL_WRITE_MASK,
			VK_DYNAMIC_STATE_STENCIL_REFERENCE
		};

		// ---

		class GraphicsPipelineState {
			// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//! Constructs this @ref GraphicsPipelineState instance.
			ETInlineHint ETForceInlineHint GraphicsPipelineState(const SpirVShader& shader, const PipelinePassDescription& pass) ETNoexceptHint :
				rasterizationState {
					VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
					/*pNext =*/nullptr,
					shader.rasterizer.flags,
					shader.rasterizer.shouldClampDepth,
					shader.rasterizer.shouldDiscardOutput,
					shader.rasterizer.fill,
					shader.rasterizer.cullMode,
					shader.rasterizer.frontFace,
					/*depthBiasEnable =*/shader.rasterizer.depthBias != 0.0f || shader.rasterizer.slopeDepthBias != 0.0f ? VK_TRUE : VK_FALSE,
					shader.rasterizer.depthBias,
					shader.rasterizer.depthBiasClamp,
					shader.rasterizer.slopeDepthBias,
					/*lineWidth =*/1.0f
				},
				multisampleState {
					VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
					/*pNext =*/nullptr,
					shader.multisample.flags,
					/*rasterizationSamples =*/VkSampleCountFlagBits(pass.framebuffer.samplesPerPixel),
					shader.multisample.shouldSubsampleShading,
					shader.multisample.minSampleShading,
					/*pSampleMask =*/nullptr,
					shader.multisample.shouldSendAlphaToCoverage,
					shader.multisample.shouldForceAlphaToOne
				},
				depthStencilState {
					VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
					/*pNext =*/nullptr,
					shader.depthStencil.flags,
					shader.depthStencil.shouldTest,
					shader.depthStencil.shouldWrite,
					shader.depthStencil.depthOperator,
					shader.depthStencil.shouldClipBounds,
					shader.depthStencil.shouldTestStencil,
					shader.depthStencil.frontStencilOperator,
					shader.depthStencil.backStencilOperator,
					shader.depthStencil.minDepthBounds,
					shader.depthStencil.maxDepthBounds
				},
				colorBlendState {
					VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
					/*pNext =*/nullptr,
					shader.blending.flags,
					shader.blending.useBitwiseBlend,
					shader.blending.bitwiseBlend,
					/*attachmentCount =*/0u,
					/*pAttachments =*/nullptr,
					/*blendConstants =*/ { 0.0f, 0.0f, 0.0f, 0.0f }
				},
				dynamicState {
					VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
					/*pNext =*/nullptr,
				} {}
			//! Constructs this @ref GraphicsPipelineState instance.
			ETInlineHint ETForceInlineHint GraphicsPipelineState(const GraphicsPipelineState& info) ETNoexceptHint :
				rasterizationState(info.rasterizationState),
				multisampleState(info.multisampleState),
				depthStencilState(info.depthStencilState),
				colorBlendState(info.colorBlendState),
				dynamicState(info.dynamicState) {
				Copy(Begin(info.entryPoints), End(info.entryPoints), entryPoints);
				Copy(Begin(info.stages), End(info.stages), stages);
			}

			~GraphicsPipelineState() = default;

			// - DATA MEMBERS ------------------------------------

		public:
			VkPipelineRasterizationStateCreateInfo rasterizationState;
			VkPipelineMultisampleStateCreateInfo   multisampleState;
			VkPipelineDepthStencilStateCreateInfo  depthStencilState;
			VkPipelineColorBlendStateCreateInfo    colorBlendState;
			VkPipelineDynamicStateCreateInfo       dynamicState;
			char                                   entryPoints[32][5];
			VkPipelineShaderStageCreateInfo        stages[5];
		};

		// ---------------------------------------------------

		template <typename Container, typename Value>
		ETInlineHint ETForceInlineHint const Value* FindElement(const Container& list, const Value& value) ETNoexceptHint {
			return LowerBound(list.Begin<Value>(), list.End<Value>(), value, LessThan<Value>());
		}

	} // anonymous namespace

	VkResult BatchCoordinator::CommandPool::BindResources(Gpu& gpu) {
		using ::Eldritch2::Swap;

		VkCommandPool                 pool;
		const VkCommandPoolCreateInfo poolInfo {
			VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
			/*pNext =*/nullptr,
			/*flags =*/0u,
			gpu.GetQueueFamilyByConcept(Drawing)
		};
		ET_ABORT_UNLESS(vkCreateCommandPool(gpu, ETAddressOf(poolInfo), gpu.GetAllocationCallbacks(), ETAddressOf(pool)));

		Swap(this->pool, pool);

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void BatchCoordinator::CommandPool::FreeResources(Gpu& gpu) {
		vkDestroyCommandPool(gpu, eastl::exchange(pool, nullptr), gpu.GetAllocationCallbacks());
	}

	// ---------------------------------------------------

	BatchCoordinator::BatchCoordinator() :
		_drawParameters(),
		_commandsByPipeline(MallocAllocator("Batch Coordinator Commands By Mesh List Allocator")) {}

	// ---------------------------------------------------

	BatchCoordinator::BatchCoordinator(BatchCoordinator&& batches) :
		BatchCoordinator() {
		Swap(*this, batches);
	}

	// ---------------------------------------------------

	bool BatchCoordinator::Contains(VkPipeline pipeline) const ETNoexceptHint {
		return FindElement(_commandsByPipeline, pipeline) == _commandsByPipeline.End<VkPipeline>();
	}

	// ---------------------------------------------------

	VkResult BatchCoordinator::BindShaderSet(Gpu& gpu, const SpirVShaderSet& shaders, const void* batchConstants, uint32 constantsByteSize) {
		ET_ASSERT(constantsByteSize == 0u || batchConstants, "batchConstants must be non-null if constantsByteSize != 0 (constantsByteSize = {})", constantsByteSize);
		const auto pipelines( BuildPipelines( gpu, /*layout =*/VK_NULL_HANDLE, shaders ) );

		for (const ArrayMap<size_t, VkPipeline>::ValueType& pipeline : pipelines) {
			CommandPool& pool( FindPool( pipeline.second ) );

			VkCommandBuffer                   commands;
			const VkCommandBufferAllocateInfo commandsInfo{
				VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
				/*pNext =*/nullptr,
				pool.pool,
				VK_COMMAND_BUFFER_LEVEL_SECONDARY,
				/*commandBufferCount =*/1u
			};
			ET_ABORT_UNLESS( vkAllocateCommandBuffers( gpu, ETAddressOf( commandsInfo ), ETAddressOf( commands ) ) );
			ET_AT_SCOPE_EXIT( if (commands) vkFreeCommandBuffers( gpu, pool.pool, /*commandBufferCount =*/1u, ETAddressOf( commands ) ) );

			const VkCommandBufferInheritanceInfo inheritanceInfo{
				VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO,
				/*pNext =*/nullptr,
				/*renderPass =*/VK_NULL_HANDLE,
				/*subpass =*/0u,
				/*framebuffer =*/VK_NULL_HANDLE,
				/*occlusionQueryEnable =*/VK_FALSE,
				/*queryFlags =*/0u,
				/*pipelineStatistics =*/0u
			};
			const VkCommandBufferBeginInfo beginInfo{
				VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
				/*pNext =*/nullptr,
				/*flags =*/VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT,
				ETAddressOf( inheritanceInfo )
			};
			ET_ABORT_UNLESS( vkBeginCommandBuffer( commands, ETAddressOf( beginInfo ) ) );
			vkCmdBindPipeline( commands, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.second );
			vkCmdPushConstants( commands, _layout, VK_SHADER_STAGE_ALL, /*offset =*/0u, constantsByteSize, batchConstants );
			vkCmdDrawIndexedIndirect( commands, _drawParameters, /*offset =*/0u, /*drawCount =*/1u, /*stride =*/sizeof( VkDrawIndexedIndirectCommand ) );
			ET_ABORT_UNLESS( vkEndCommandBuffer( commands ) );
		}

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	VkResult BatchCoordinator::BindResources(Gpu& gpu, const GraphicsPipelineBuilder& pipeline, size_t commandPoolCount) {
		ET_ASSERT(commandPoolCount > 0, "commandPoolCount must be > 0!");

		UniformBuffer drawParameters;
		ET_ABORT_UNLESS(drawParameters.BindResources(gpu, /*parameterBufferSize =*/4096u));
		ET_AT_SCOPE_EXIT(drawParameters.FreeResources(gpu));

		ArrayList<CommandPool> commandPools(_commandPools.GetAllocator(), commandPoolCount);
		ET_AT_SCOPE_EXIT(for (CommandPool& pool
							  : commandPools) pool.FreeResources(gpu));
		for (; commandPoolCount; --commandPoolCount) {
			ET_ABORT_UNLESS(commandPools.EmplaceBack().BindResources(gpu));
		}

		SoArrayList<VkRenderPass, String> renderPasses( _renderPasses.GetAllocator(), pipeline.GetPasses().GetSize() );
		ET_AT_SCOPE_EXIT( for (VkRenderPass pass : Range<VkRenderPass*>( renderPasses.Begin<VkRenderPass>(), renderPasses.End<VkRenderPass>() )) {
			vkDestroyRenderPass( gpu, pass, gpu.GetAllocationCallbacks() );
		} );

		for (const PipelinePassDescription& pipelinePass : pipeline.GetPasses()) {
			VkRenderPass pass;
			const VkRenderPassCreateInfo passInfo{
				VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
				/*pNext =*/nullptr,
			};
			ET_ABORT_UNLESS( vkCreateRenderPass( gpu, ETAddressOf( passInfo ), gpu.GetAllocationCallbacks(), ETAddressOf(pass)) );
			renderPasses.Append(pass, String(MallocAllocator("Render Pass Name Allocator"), StringView( pipelinePass.shader )));
		}

		Swap(_drawParameters, drawParameters);
		Swap(_commandPools, commandPools);
		Swap( _renderPasses, renderPasses );

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void BatchCoordinator::FreeResources(Gpu& gpu) {
		for (CommandPool& pool : _commandPools) {
			pool.FreeResources(gpu);
		}

		_drawParameters.FreeResources(gpu);
		_commandPools.Clear();
	}

	// ---------------------------------------------------

	BatchCoordinator::PipelineList BatchCoordinator::BuildComputePipelines( Gpu& gpu, VkPipelineLayout layout, const SpirVShaderSet& shaders ) const {
		SoArrayList<VkPipeline, VkComputePipelineCreateInfo> pipelines( MallocAllocator( "Pipeline List Allocator" ), Min( shaders.GetSize(), _renderPasses.GetSize() ) );

		for (SoArrayList<VkRenderPass, String>::ConstReference pass : _renderPasses) {
			pipelines.Append( VK_NULL_HANDLE, VkComputePipelineCreateInfo{ VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO,
							  /*pNext =*/nullptr,
							  /*flags =*/pipelines.IsEmpty() ? VK_PIPELINE_CREATE_ALLOW_DERIVATIVES_BIT : VK_PIPELINE_CREATE_DERIVATIVE_BIT,
							  /*stage =*/VkPipelineShaderStageCreateInfo {
								  VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
								  /*pNext =*/nullptr,
								  /*flags =*/0u,
								  /*stage =*/VK_SHADER_STAGE_COMPUTE_BIT,
								  /*module =*/VK_NULL_HANDLE,
								  /*pName =*/"",
								  /*pSpecializationInfo =*/nullptr,
							  },
							  layout,
							  /*basePipelineHandle =*/VK_NULL_HANDLE,
							  /*basePipelineIndex =*/pipelines.IsEmpty() ? uint32( -1 ) : 0u } );
		}

		vkCreateComputePipelines( gpu, gpu.GetPipelineCache(), pipelines.GetSize(), pipelines.Begin<VkComputePipelineCreateInfo>(), gpu.GetAllocationCallbacks(), pipelines.Begin<VkPipeline>() );
	}

	// ---------------------------------------------------

	BatchCoordinator::PipelineList BatchCoordinator::BuildPipelines( Gpu& gpu, VkPipelineLayout layout, const SpirVShaderSet& shaders ) const {
		SoArrayList<VkPipeline, GraphicsPipelineState, VkGraphicsPipelineCreateInfo> pipelines( MallocAllocator( "Pipeline List Allocator" ), Min( shaders.GetSize(), _renderPasses.GetSize() ) );

		for (SoArrayList<VkRenderPass, String>::ConstReference pass : _renderPasses) {
			const auto shader( shaders.Find( StringView( eastl::get<const String&>( pass ), Hash<StringView>(), EqualTo<StringView>() )));
			pipelines.EmplaceBack( VK_NULL_HANDLE, shader, VkGraphicsPipelineCreateInfo{ VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
								   /*pNext =*/nullptr,
								   /*flags =*/pipelines.IsEmpty() ? VK_PIPELINE_CREATE_ALLOW_DERIVATIVES_BIT : VK_PIPELINE_CREATE_DERIVATIVE_BIT,
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
								   /*pDynamicState =*/nullptr, layout,
								   /*renderPass =*/VK_NULL_HANDLE,
								   /*subpass =*/0u,
								   /*basePipelineHandle =*/VK_NULL_HANDLE,
								   /*basePipelineIndex =*/pipelines.IsEmpty() ? uint32( -1 ) : 0u } );
		}

		vkCreateGraphicsPipelines( gpu, gpu.GetPipelineCache(), pipelines.GetSize(), pipelines.Begin<VkGraphicsPipelineCreateInfo>(), gpu.GetAllocationCallbacks(), pipelines.Begin<VkPipeline>() );
		return {};
	}

	// ---------------------------------------------------

	void Swap(BatchCoordinator& lhs, BatchCoordinator& rhs) {
		using ::Eldritch2::Swap;

		Swap(lhs._layout, rhs._layout);
		Swap(lhs._drawParameters, rhs._drawParameters);
		Swap(lhs._commandPools, rhs._commandPools);
		Swap(rhs._commandsByPipeline, rhs._commandsByPipeline);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
