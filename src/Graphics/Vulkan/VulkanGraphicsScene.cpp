/*==================================================================*\
  VulkanGraphicsScene.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/VulkanGraphicsScene.hpp>
#include <Graphics/Vulkan/VulkanTools.hpp>
#include <Graphics/Vulkan/Vulkan.hpp>
#include <Scheduling/JobExecutor.hpp>
#include <Graphics/Vulkan/Gpu.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {
namespace {

	static const Vector GeometryCellDimensions( 10.0f, 10.0f, 10.0f, 1.0f );
	static const Vector LightCellDimensions( 10.0f, 10.0f, 10.0f, 1.0f );

// ---------------------------------------------------

	ETPureFunctionHint ETInlineHint VkViewport GetViewport( float widthScale, float heightScale, bool invertDepth = true ) {
		return VkViewport{
			0.0f,						// x
			0.0f,						// y
			widthScale,					// width,
			heightScale,				// height,
			invertDepth ? 1.0f : 0.0f,	// minDepth
			invertDepth ? 0.0f : 1.0f	// maxDepth
		};
	}

// ---------------------------------------------------

	ETPureFunctionHint ETInlineHint VkRect2D GetScissorRect( uint32_t horizontalResolution, uint32_t verticalResolution, float widthScale, float heightScale ) {
		return VkRect2D{
			VkOffset2D{ AsInt( horizontalResolution * widthScale ), AsInt( verticalResolution * heightScale ) },
			VkExtent2D{ uint32_t( AsInt( horizontalResolution * widthScale ) ), uint32( AsInt( verticalResolution * heightScale ) ) }
		};
	}

}	// anonymous namespace

	using namespace ::Eldritch2::Graphics::Vulkan::AssetViews;
	using namespace ::Eldritch2::Scheduling;

	VulkanGraphicsScene::VulkanGraphicsScene() : GraphicsScene( GeometryCellDimensions, LightCellDimensions ), _frameId( 0u ) {}

// ---------------------------------------------------

	VkResult VulkanGraphicsScene::SubmitGpuCommands( JobExecutor& executor, Vulkan& vulkan ) {
		BuildAccelerators( executor );

	/*	We now have spatial acceleration structures set up; it's time to determine the visible set for each camera
	 *	registered with the scene. */
		executor.AwaitWork(
			[&] ( JobExecutor& executor ) {
				ArrayList<View>	views( MallocAllocator( "Frame View Stack Allocator" ) );
				Frame&			frame( _queuedFrames[_frameId++ % _countof(_queuedFrames)] );
				Gpu&			gpu( vulkan.GetPrimaryDevice() );

			//	We cannot use iterators for this pass since we may add one or more child views to the end of the list.
				for (size_t index( 0u ); index < views.GetSize(); ++index) {
					const View& view( views[index] );

					for (const PortalView* portal : GetConcept( PortalViewType::PortalViews )) {
					}
				}

			//	Wait for previous frame commands to drain to avoid data hazards.
				executor.AwaitCondition( vkWaitForFences( gpu, 1u, &frame.commandsConsumed, VK_FALSE, 0u ) != VK_TIMEOUT );

				if (Failed( vkResetFences( gpu, 1u, &frame.commandsConsumed ) ) ||
					Failed( frame.commands.ResetPool( gpu ) ) ||
					Failed( frame.commands.BeginRecording( VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT ) ) ) {
					return;
				}

				for (const View& view : views) {
					frame.commands.SetStencilReference( VK_STENCIL_FRONT_AND_BACK, view.id );
					frame.commands.SetStencilWriteMask( VK_STENCIL_FRONT_AND_BACK, 0xFFFF );
					frame.commands.SetStencilReadMask( VK_STENCIL_FRONT_AND_BACK,  0xFFFF );
					frame.commands.SetScissors( 0u, 1u, &view.scissorArea );
					frame.commands.SetDepthBias( 0.0f, 0.0f, 0.0f );

					frame.commands.Draw( 0u, 0u, 0u, 0u );
				}

				if (Failed( frame.commands.FinishRecording() )) { return; }

			//	Submit command buffers for execution.
				gpu.SubmitAsync( QueueConcept::Drawing, frame.commandsConsumed, {
					AsSubmitInfo(
					//	No waits.
						{ frame.commands.Get() }	
					//	No signals.
					)
				} );
			},
			[this] ( JobExecutor& /*executor*/ ) {
			/*	Destroy old resources from the previous frame. Since all commands that may depend on the object have finished execution,
			 *	there is no longer a data hazard. */
				_heap.DestroyGarbage();
			}
		);

		return VK_SUCCESS;
	}

// ---------------------------------------------------

	VkResult VulkanGraphicsScene::BindResources( JobExecutor& /*executor*/, Vulkan& vulkan, VkDeviceSize transformArenaSize ) {
		using Eldritch2::Swap;

		Gpu&	gpu( vulkan.GetPrimaryDevice() );

		GpuHeap heap;
		ET_FAIL_UNLESS( heap.BindResources( gpu, GpuHeapBlockSize ) );
		ET_AT_SCOPE_EXIT( heap.FreeResources( gpu ) );

		for (Frame& frame : _queuedFrames) {
			DescriptorTable	descriptors;
			ET_FAIL_UNLESS( descriptors.BindResources( gpu, DescriptorPoolSizeInElements ) );
			ET_AT_SCOPE_EXIT( descriptors.FreeResources( gpu ) );

			UniformBuffer transforms;
			ET_FAIL_UNLESS( transforms.BindResources( heap, transformArenaSize ) );
			ET_AT_SCOPE_EXIT( transforms.FreeResources( heap ) );

			CommandList commands;
			ET_FAIL_UNLESS( commands.BindResources( gpu, QueueConcept::Drawing, 0u ) );
			ET_AT_SCOPE_EXIT( commands.FreeResources( gpu ) );

			VkFence commandsConsumed;
			const VkFenceCreateInfo commandsConsumedInfo{
				VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
				nullptr,
				VK_FENCE_CREATE_SIGNALED_BIT
			};
			ET_FAIL_UNLESS( vkCreateFence( gpu, &commandsConsumedInfo, gpu.GetAllocationCallbacks(), &commandsConsumed ) );
			ET_AT_SCOPE_EXIT( vkDestroyFence( gpu, commandsConsumed, gpu.GetAllocationCallbacks() ) );

			Swap( frame.descriptors,        descriptors );
			Swap( frame.transforms,			transforms );
			Swap( frame.commands,			commands );
			Swap( frame.commandsConsumed,	commandsConsumed );
		}

		Swap( _heap, heap );

		_frameId = 0u;

		return VK_SUCCESS;
	}

// ---------------------------------------------------

	void VulkanGraphicsScene::FreeResources( JobExecutor& /*executor*/, Vulkan& vulkan ) {
		Gpu&	gpu( vulkan.GetPrimaryDevice() );

		for (Frame& frame : _queuedFrames) {
			for (RenderPipeline& pipeline : frame.pipelines) pipeline.FreeResources( gpu );

			vkDestroyFence( gpu, eastl::exchange( frame.commandsConsumed, nullptr ), gpu.GetAllocationCallbacks() );

			frame.commands.FreeResources( gpu );
			frame.transforms.FreeResources( _heap );
			frame.descriptors.FreeResources( gpu );
		}

		_heap.FreeResources( gpu );
	}

}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2