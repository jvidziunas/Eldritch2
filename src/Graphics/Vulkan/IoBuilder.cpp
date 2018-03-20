/*==================================================================*\
  IoBuilder.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/VulkanTools.hpp>
#include <Graphics/Vulkan/IoBuilder.hpp>
#include <Scheduling/JobExecutor.hpp>
#include <Graphics/Vulkan/Gpu.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {

	using namespace ::Eldritch2::Scheduling;

namespace {

	enum : uint32_t {
		TightPack	= 0u
	};

// ---------------------------------------------------

	template <typename Offset>
	ETInlineHint bool LockRegion( std::atomic<Offset>& tracker, Offset extent, Offset& offset ) {
		VkDeviceSize transferOffset( tracker.load( std::memory_order_consume ) );

		do {
			if (transferOffset < extent) {
				return false;
			}
		} while (!tracker.compare_exchange_weak( transferOffset, transferOffset - extent, std::memory_order_release ));

		offset = transferOffset;
		return true;
	}

}	// anonymous namespace

	IoBuilder::IoBuilder(
	) : _imageBinds( MallocAllocator( "GPU I/O Builder Image Bind List Allocator" ) ),
		_opaqueImageBinds( MallocAllocator( "GPU I/O Builder Opaque Image Bind List Allocator" ) ),
		_imageBarriers( MallocAllocator( "GPU I/O Builder Image Barrier List Allocator" ) ),
		_bufferBarriers( MallocAllocator( "GPU I/O Builder Buffer Barrier List Allocator" ) ),
		_readOffset( 0u ),
		_writeOffset( 0u ),
		_phaseCompleted{ nullptr },
		_commandsConsumed( nullptr ),
		_ioCompleted( nullptr ) {
	}

// ---------------------------------------------------

	void IoBuilder::TransferToGpu( VkImage target, VkImageSubresourceLayers subresource, VkOffset3D offset, VkExtent3D extent ) {
		VkDeviceSize	transferOffset;
		VkDeviceSize	transferExtent( extent.width * extent.height * extent.depth );

		if (!LockRegion<VkDeviceSize>( _readOffset, transferExtent, transferOffset )) {
			return;
		}

		_commandLists[StandardUpload].CopyRegions( target, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, _frameTransfers, {
			VkBufferImageCopy{
				transferOffset,
				TightPack,
				TightPack,
				subresource,
				offset,
				extent,
			}
		} );

		_imageBarriers.Append( VkImageMemoryBarrier{
			VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
			nullptr,
			VK_ACCESS_SHADER_READ_BIT,
			VK_ACCESS_TRANSFER_WRITE_BIT,
			VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			_drawFamily,
			_transferFamily,
			target,
			VkImageSubresourceRange{
				subresource.aspectMask,
				subresource.mipLevel,
				1u,
				subresource.baseArrayLayer,
				subresource.layerCount
			}
		} );
	}

// ---------------------------------------------------

	void IoBuilder::TransferToGpu( VkBuffer target, VkDeviceSize offset, VkDeviceSize extent ) {
		VkDeviceSize	transferOffset;

		if (!LockRegion<VkDeviceSize>( _readOffset, extent, transferOffset )) {
			return;
		}

		_commandLists[StandardUpload].CopyRegions( target, _frameTransfers, {
			VkBufferCopy {
				transferOffset,	// Offset in transfer buffer
				offset,
				extent
			}
		} );

		_bufferBarriers.Append( VkBufferMemoryBarrier{
			VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER,
			nullptr,
			VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT,
			VK_ACCESS_TRANSFER_WRITE_BIT,
			_drawFamily,
			_transferFamily,
			target,
			offset,
			extent
		} );
	}

// ---------------------------------------------------

	void IoBuilder::TransferToHost( VkImage source, VkImageSubresourceLayers subresource, VkOffset3D offset, VkExtent3D extent ) {
		VkDeviceSize	transferOffset;
		VkDeviceSize	transferExtent( extent.width * extent.height * extent.depth );

		if (!LockRegion<VkDeviceSize>( _readOffset, transferExtent, transferOffset )) {
			return;
		}

		_commandLists[StandardDownload].CopyRegions( _frameTransfers, source, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, {
			VkBufferImageCopy{
				transferOffset,
				TightPack,
				TightPack,
				subresource,
				offset,
				extent,
			}
		} );

		_imageBarriers.Append( VkImageMemoryBarrier{
			VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
			nullptr,
			(VK_ACCESS_SHADER_WRITE_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT),
			VK_ACCESS_TRANSFER_READ_BIT,
			VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			_drawFamily,
			_transferFamily,
			source,
			VkImageSubresourceRange{
				subresource.aspectMask,
				subresource.mipLevel,
				1u,
				subresource.baseArrayLayer,
				subresource.layerCount
			}
		} );
	}

// ---------------------------------------------------

	void IoBuilder::TransferToHost( VkBuffer source, VkDeviceSize offset, VkDeviceSize extent ) {
		VkDeviceSize transferOffset;

		if (!LockRegion<VkDeviceSize>( _readOffset, extent, transferOffset )) {
			return;
		}

		_commandLists[StandardDownload].CopyRegions( _frameTransfers, source, {
			VkBufferCopy {
				transferOffset,
				offset,
				extent
			}
		} );

		_bufferBarriers.Append( VkBufferMemoryBarrier{
			VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER,
			nullptr,
			VK_ACCESS_SHADER_WRITE_BIT,
			VK_ACCESS_TRANSFER_READ_BIT,
			_drawFamily,
			_transferFamily,
			source,
			offset,
			extent
		} );
	}

// ---------------------------------------------------

	VkResult IoBuilder::SubmitCommands( JobExecutor& executor, Gpu& gpu ) {
		constexpr VkPipelineStageFlags	AllCommandsStage	= VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
		constexpr VkPipelineStageFlags	TransferStage		= VK_PIPELINE_STAGE_TRANSFER_BIT;

	//	Wait for previous frame commands to drain to avoid data hazards.
		executor.AwaitCondition( vkWaitForFences( gpu, 1u, &_commandsConsumed, VK_FALSE, 0u ) != VK_TIMEOUT );

	//	Reset the command buffers and record the frame's commands.
		ET_FAIL_UNLESS( vkResetFences( gpu, 1u, &_commandsConsumed ) );
		ET_FAIL_UNLESS( vkResetEvent( gpu, _ioCompleted ) );

		for (CommandList& list : _commandLists) {
			ET_FAIL_UNLESS( list.ResetPool( gpu ) );
			ET_FAIL_UNLESS( list.BeginRecording( VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT ) );

				list.FillBuffer( _frameTransfers, 0u, 0xFFFF, sizeof( uint32 ) );

			ET_FAIL_UNLESS( list.FinishRecording() );
		}

		ET_FAIL_UNLESS( _barrierCommandList.ResetPool( gpu ) );
		ET_FAIL_UNLESS( _barrierCommandList.BeginRecording( VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT ) );

		//	_barrierCommandList.PipelineBarrier( 0u, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT, );
			_barrierCommandList.SetEvent( _ioCompleted, VK_PIPELINE_STAGE_TRANSFER_BIT );

		ET_FAIL_UNLESS( _barrierCommandList.FinishRecording() );

	//	Send the commands to the GPU for execution. Clients may wait on one or more of the upload event objects to ensure that there are no memory hazards.
	/*	Per section 5.5 of the Vulkan spec, it is necessary to split the pre/post bind submit operations into separate batch submission calls in order to
	 *	uphold the queue forward progress guarantee. Note that if this restriction is lifted, it may be possible to group the submits together and rely on the
	 *	semaphore to enforce ordering. The alternative approach has the advantage of reduced CPU submit overhead compared to the split approach. */
		ET_FAIL_UNLESS( gpu.SubmitAsync( QueueConcept::Transfer, VK_NULL_HANDLE, {
			AsSubmitInfo(
				{ _commandLists[StandardDownload].Get()	},	// Commands
				{ _phaseCompleted[StandardDownload]		}	// Signals
			),
			AsSubmitInfo(
				{ _commandLists[StandardUpload].Get()	},	// Commands
				{ _phaseCompleted[StandardUpload]		}	// Signals
			)
		} ) );
		ET_FAIL_UNLESS( gpu.BindAsync( QueueConcept::SparseBinding, VK_NULL_HANDLE, {
			AsBindSparseInfo(
				{ _phaseCompleted[StandardDownload]							},	// Waits
				  0u,							nullptr,						// No buffer binds.
				  _opaqueImageBinds.GetSize(),	_opaqueImageBinds.GetData(),	// Sparse image opaque binds
				  _imageBinds.GetSize(),		_imageBinds.GetData(),			// Image binds
				{ _phaseCompleted[SparseBind]								}	// Signals
			)
		} ) );
		ET_FAIL_UNLESS( gpu.SubmitAsync( QueueConcept::Transfer, VK_NULL_HANDLE, {
			AsSubmitInfo(
				{ _phaseCompleted[SparseBind]		},	// Waits
				{ TransferStage						},	// Wait stages
				{ _commandLists[SparseUpload].Get()	},	// Commands
				{ _phaseCompleted[SparseUpload]		}	// Signals
			)
		} ) );
		ET_FAIL_UNLESS( gpu.SubmitAsync( QueueConcept::Drawing, _commandsConsumed, {
			AsSubmitInfo(
				{ _phaseCompleted[StandardUpload],	_phaseCompleted[SparseUpload]	},	// Waits
				{ AllCommandsStage,					AllCommandsStage				},	// Wait stages
				{ _barrierCommandList.Get()											}	// Commands
			//	No signals-- clients rely on an event set in the command buffer to prevent data hazards.
			)
		} ) );

		return VK_SUCCESS;
	}

// ---------------------------------------------------

	VkResult IoBuilder::BindResources( Gpu& gpu, GpuHeap& heap, VkDeviceSize transferBufferSize ) {
		using ::Eldritch2::Swap;

		TransferBuffer frameTransfers;
		ET_FAIL_UNLESS( frameTransfers.BindResources( heap, transferBufferSize ) );
		ET_AT_SCOPE_EXIT( frameTransfers.FreeResources( heap ) );

		VkSemaphore phaseCompleted[Phase::COUNT];
		ET_AT_SCOPE_EXIT( for (VkSemaphore semaphore : phaseCompleted) vkDestroySemaphore( gpu, semaphore, gpu.GetAllocationCallbacks() ); );
		for (VkSemaphore& semaphore : phaseCompleted) {
			const VkSemaphoreCreateInfo phaseCompleteInfo{
				VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
				nullptr,
				0u
			};
			ET_FAIL_UNLESS( vkCreateSemaphore( gpu, &phaseCompleteInfo, gpu.GetAllocationCallbacks(), &semaphore ) );
		}

		VkFence commandsConsumed( nullptr );
		const VkFenceCreateInfo commandsConsumedInfo{
			VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
			nullptr,
			VK_FENCE_CREATE_SIGNALED_BIT
		};
		ET_FAIL_UNLESS( vkCreateFence( gpu, &commandsConsumedInfo, gpu.GetAllocationCallbacks(), &commandsConsumed ) );
		ET_AT_SCOPE_EXIT( vkDestroyFence( gpu, commandsConsumed, gpu.GetAllocationCallbacks() ) );

		VkEvent ioCompleted( nullptr );
		const VkEventCreateInfo ioCompletedInfo{
			VK_STRUCTURE_TYPE_EVENT_CREATE_INFO,
			nullptr,
			0
		};
		ET_FAIL_UNLESS( vkCreateEvent( gpu, &ioCompletedInfo, gpu.GetAllocationCallbacks(), &ioCompleted ) );
		ET_AT_SCOPE_EXIT( vkDestroyEvent( gpu, ioCompleted, gpu.GetAllocationCallbacks() ) );

		CommandList commandLists[_countof(_commandLists)];
		ET_AT_SCOPE_EXIT( for (CommandList& list : commandLists) list.FreeResources( gpu ); );
		for (CommandList& list : commandLists) {
			ET_FAIL_UNLESS( list.BindResources( gpu, QueueConcept::Transfer, VK_COMMAND_POOL_CREATE_TRANSIENT_BIT ) );
		}
	
		CommandList barrierCommandList;
		ET_FAIL_UNLESS( barrierCommandList.BindResources( gpu, QueueConcept::Drawing, 0u ) );
		ET_AT_SCOPE_EXIT( barrierCommandList.FreeResources( gpu ) );

	//	Swap created resources into the object.
		Swap( _frameTransfers,     frameTransfers );
		Swap( _phaseCompleted,     phaseCompleted );
		Swap( _commandsConsumed,   commandsConsumed );
		Swap( _ioCompleted,        ioCompleted );
		Swap( _commandLists,       commandLists );
		Swap( _barrierCommandList, barrierCommandList );

		return VK_SUCCESS;
	}

// ---------------------------------------------------

	void IoBuilder::FreeResources( Gpu& gpu, GpuHeap& heap ) {
		_barrierCommandList.FreeResources( gpu );

		for (CommandList& list : _commandLists) {
			list.FreeResources( gpu );
		}

		vkDestroyFence( gpu, eastl::exchange( _commandsConsumed, nullptr ), gpu.GetAllocationCallbacks() );

		for (VkSemaphore& semaphore : _phaseCompleted) {
			vkDestroySemaphore( gpu, eastl::exchange( semaphore, nullptr ), gpu.GetAllocationCallbacks() );
		}

		vkDestroyEvent( gpu, eastl::exchange( _ioCompleted, nullptr ), gpu.GetAllocationCallbacks() );
		_frameTransfers.FreeResources( heap );
	}

// ---------------------------------------------------

	void Swap( IoBuilder& builder0, IoBuilder& builder1 ) {
		using ::Eldritch2::Swap;

		Swap( builder0._drawFamily,			builder1._drawFamily );
		Swap( builder0._transferFamily,		builder1._transferFamily );
		Swap( builder0._opaqueImageBinds,	builder1._opaqueImageBinds );
		Swap( builder0._imageBinds,			builder1._imageBinds );
		Swap( builder0._imageBarriers,		builder1._imageBarriers );
		Swap( builder0._bufferBarriers,		builder1._bufferBarriers );
		Swap( builder0._frameTransfers,		builder1._frameTransfers );

	//	Swap read/write offsets.
		VkDeviceSize readOffset( builder1._readOffset.exchange( builder0._readOffset.load( std::memory_order_relaxed ), std::memory_order_relaxed ) );
		VkDeviceSize writeOffset( builder1._writeOffset.exchange( builder0._writeOffset.load( std::memory_order_relaxed ), std::memory_order_relaxed ) );

		builder0._readOffset.store( readOffset, std::memory_order_relaxed );
		builder1._writeOffset.store( writeOffset, std::memory_order_relaxed );

		Swap( builder0._phaseCompleted,		builder1._phaseCompleted );
		Swap( builder0._commandsConsumed,	builder1._commandsConsumed );
		Swap( builder0._ioCompleted,		builder1._ioCompleted );
		Swap( builder0._commandLists,		builder1._commandLists );
		Swap( builder0._barrierCommandList,	builder1._barrierCommandList );
	}

}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2