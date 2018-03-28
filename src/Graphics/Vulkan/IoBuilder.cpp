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
#include <Graphics/Vulkan/Gpu.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {
namespace {

	enum : uint32_t {
		TightPack	= 0u
	};

// ---------------------------------------------------

	template <typename Offset>
	ETInlineHint bool LockRegion( Atomic<Offset>& tracker, Offset extent, Offset& offset ) {
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
		_readOffset( 0u ),
		_writeOffset( 0u ),
		_phaseCompleted{ nullptr },
		_commandsConsumed( nullptr ),
		_ioCompleted( nullptr ) {
	}

// ---------------------------------------------------

	void IoBuilder::TransferToGpu( VkImage target, VkImageSubresourceLayers subresource, VkOffset3D targetOffset, VkExtent3D targetExtent ) {
		VkDeviceSize	extent( targetExtent.width * targetExtent.height * targetExtent.depth * subresource.layerCount );
		VkDeviceSize	stagingOffset;

		if (!LockRegion<VkDeviceSize>( _readOffset, extent, stagingOffset )) {
			return;
		}

		_commandLists[StandardUpload].Copy( target, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, _stagingBuffer, {
			VkBufferImageCopy{
				stagingOffset,
				TightPack,
				TightPack,
				subresource,
				targetOffset,
				targetExtent,
			}
		} );
	}

// ---------------------------------------------------

	void IoBuilder::TransferToGpu( VkBuffer target, VkDeviceSize targetOffset, VkDeviceSize targetExtent ) {
		VkDeviceSize	stagingOffset;

		if (!LockRegion<VkDeviceSize>( _readOffset, targetExtent, stagingOffset )) {
			return;
		}

		_commandLists[StandardUpload].Copy( target, _stagingBuffer, {
			VkBufferCopy {
				stagingOffset,
				targetOffset,
				targetExtent
			}
		} );
	}

// ---------------------------------------------------

	void IoBuilder::TransferToHost( VkImage source, VkImageSubresourceLayers subresource, VkOffset3D sourceOffset, VkExtent3D sourceExtent ) {
		VkDeviceSize	stagingOffset;
		VkDeviceSize	extent( sourceExtent.width * sourceExtent.height * sourceExtent.depth * subresource.layerCount );

		if (!LockRegion<VkDeviceSize>( _readOffset, extent, stagingOffset )) {
			return;
		}

		_commandLists[StandardDownload].Copy( _stagingBuffer, source, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, {
			VkBufferImageCopy{
				stagingOffset,
				TightPack,
				TightPack,
				subresource,
				sourceOffset,
				sourceExtent,
			}
		} );
	}

// ---------------------------------------------------

	void IoBuilder::TransferToHost( VkBuffer source, VkDeviceSize sourceOffset, VkDeviceSize sourceExtent ) {
		VkDeviceSize stagingOffset;

		if (!LockRegion<VkDeviceSize>( _readOffset, sourceExtent, stagingOffset )) {
			return;
		}

		_commandLists[StandardDownload].Copy( _stagingBuffer, source, {
			VkBufferCopy{ stagingOffset, sourceOffset, sourceExtent }
		} );
	}

// ---------------------------------------------------

	bool IoBuilder::CheckCommandsConsumed( Gpu& gpu ) const {
		return vkWaitForFences( gpu, 1u, &_commandsConsumed, VK_FALSE, 0u ) != VK_TIMEOUT;
	}

// ---------------------------------------------------

	VkResult IoBuilder::SubmitCommands( Gpu& gpu ) {
		constexpr VkPipelineStageFlags	AllCommandsStage	= VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
		constexpr VkPipelineStageFlags	TransferStage		= VK_PIPELINE_STAGE_TRANSFER_BIT;

		VkMemoryBarrier barrier{
			VK_STRUCTURE_TYPE_MEMORY_BARRIER,
			nullptr,	// No extension structures.
		};

		ET_ASSERT( CheckCommandsConsumed( gpu ), "Call CheckCommandsConsumed() to wait for GPU to finish processing commands!" );

	//	Reset the command buffers and record the frame's commands.
		ET_FAIL_UNLESS( vkResetFences( gpu, 1u, &_commandsConsumed ) );
		ET_FAIL_UNLESS( vkResetEvent( gpu, _ioCompleted ) );

		for (CommandList& list : _commandLists) {
			ET_FAIL_UNLESS( list.ResetPool( gpu ) );
			ET_FAIL_UNLESS( list.BeginRecording( VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT ) );
		}

		barrier.srcAccessMask = VK_ACCESS_MEMORY_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
		_commandLists[StandardDownload].PipelineBarrier( 0u, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 1u, &barrier, 0u, nullptr, 0u, nullptr );

		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
		_commandLists[StandardUpload].PipelineBarrier( 0u, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 1u, &barrier, 0u, nullptr, 0u, nullptr );
		_commandLists[SparseUpload].PipelineBarrier( 0u, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 1u, &barrier, 0u, nullptr, 0u, nullptr );

		for (CommandList& list : _commandLists) {
			ET_FAIL_UNLESS( list.FinishRecording() );
		}

		ET_FAIL_UNLESS( _finalizeCommandList.ResetPool( gpu ) );
		ET_FAIL_UNLESS( _finalizeCommandList.BeginRecording( VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT ) );

			_finalizeCommandList.SetEvent( _ioCompleted, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT );

		ET_FAIL_UNLESS( _finalizeCommandList.FinishRecording() );

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
				{ _finalizeCommandList.Get()										}	// Commands
			//	No signals-- clients rely on an event set in the command buffer to prevent data hazards.
			)
		} ) );

		return VK_SUCCESS;
	}

// ---------------------------------------------------

	VkResult IoBuilder::BindResources( Gpu& gpu, GpuHeap& heap, VkDeviceSize transferBufferSize ) {
		using ::Eldritch2::Swap;

		TransferBuffer stagingBuffer;
		ET_FAIL_UNLESS( stagingBuffer.BindResources( heap, transferBufferSize ) );
		ET_AT_SCOPE_EXIT( stagingBuffer.FreeResources( heap ) );

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
	
		CommandList finalizeCommandList;
		ET_FAIL_UNLESS( finalizeCommandList.BindResources( gpu, QueueConcept::Drawing, 0u ) );
		ET_AT_SCOPE_EXIT( finalizeCommandList.FreeResources( gpu ) );

	//	Swap created resources into the object.
		Swap( _stagingBuffer,       stagingBuffer );
		Swap( _phaseCompleted,      phaseCompleted );
		Swap( _commandsConsumed,    commandsConsumed );
		Swap( _ioCompleted,         ioCompleted );
		Swap( _commandLists,        commandLists );
		Swap( _finalizeCommandList, finalizeCommandList );

		return VK_SUCCESS;
	}

// ---------------------------------------------------

	void IoBuilder::FreeResources( Gpu& gpu, GpuHeap& heap ) {
		_finalizeCommandList.FreeResources( gpu );

		for (CommandList& list : _commandLists) {
			list.FreeResources( gpu );
		}

		vkDestroyFence( gpu, eastl::exchange( _commandsConsumed, nullptr ), gpu.GetAllocationCallbacks() );

		for (VkSemaphore& semaphore : _phaseCompleted) {
			vkDestroySemaphore( gpu, eastl::exchange( semaphore, nullptr ), gpu.GetAllocationCallbacks() );
		}

		vkDestroyEvent( gpu, eastl::exchange( _ioCompleted, nullptr ), gpu.GetAllocationCallbacks() );
		_stagingBuffer.FreeResources( heap );
	}

// ---------------------------------------------------

	void Swap( IoBuilder& builder0, IoBuilder& builder1 ) {
		using ::Eldritch2::Swap;

		Swap( builder0._opaqueImageBinds, builder1._opaqueImageBinds );
		Swap( builder0._imageBinds,       builder1._imageBinds );
		Swap( builder0._stagingBuffer,    builder1._stagingBuffer );

	//	Swap read/write offsets.
		VkDeviceSize readOffset( builder1._readOffset.exchange( builder0._readOffset.load( std::memory_order_relaxed ), std::memory_order_relaxed ) );
		VkDeviceSize writeOffset( builder1._writeOffset.exchange( builder0._writeOffset.load( std::memory_order_relaxed ), std::memory_order_relaxed ) );

		builder0._readOffset.store( readOffset, std::memory_order_relaxed );
		builder1._writeOffset.store( writeOffset, std::memory_order_relaxed );

		Swap( builder0._phaseCompleted,      builder1._phaseCompleted );
		Swap( builder0._commandsConsumed,    builder1._commandsConsumed );
		Swap( builder0._ioCompleted,         builder1._ioCompleted );
		Swap( builder0._commandLists,        builder1._commandLists );
		Swap( builder0._finalizeCommandList, builder1._finalizeCommandList );
	}

}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2