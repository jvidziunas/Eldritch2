/*==================================================================*\
  IoBuilder.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/GpuResources.hpp>
#include <Graphics/Vulkan/CommandList.hpp>
//------------------------------------------------------------------//
#include <atomic>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Scheduling {
		class	JobExecutor;
	}
}

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {

	class IoBuilder {
	// - TYPE PUBLISHING ---------------------------------

	public:
		enum Phase : uint32 {
			StandardDownload,
			SparseBind,
			StandardUpload,
			SparseUpload,

			COUNT
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Disable copy construction.
		IoBuilder( const IoBuilder& ) = delete;
	//!	Constructs this @ref IoBuilder instance.
		IoBuilder();

		~IoBuilder() = default;

	// ---------------------------------------------------

	public:
		void	TransferToGpu( VkImage target, VkImageSubresourceLayers subresource, VkOffset3D offset, VkExtent3D extent );
		void	TransferToGpu( VkBuffer target, VkDeviceSize offset, VkDeviceSize extent );

		void	TransferToHost( VkImage source, VkImageSubresourceLayers subresource, VkOffset3D offset, VkExtent3D extent );
		void	TransferToHost( VkBuffer source, VkDeviceSize offset, VkDeviceSize extent );

	// ---------------------------------------------------

	public:
		VkResult	SubmitCommands( Scheduling::JobExecutor& executor, Gpu& gpu );

	// ---------------------------------------------------

	public:
		VkResult	BindResources( Gpu& gpu, GpuHeap& heap, VkDeviceSize transferBufferSize );

		void		FreeResources( Gpu& gpu, GpuHeap& heap );

	// ---------------------------------------------------

	//!	Disable copy assignment.
		IoBuilder&	operator=( const IoBuilder& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		uint32_t										_drawFamily;
		uint32_t										_transferFamily;

		ArrayList<VkSparseImageOpaqueMemoryBindInfo>	_opaqueImageBinds;
		ArrayList<VkSparseImageMemoryBindInfo>			_imageBinds;
		ArrayList<VkImageMemoryBarrier>					_imageBarriers;
		ArrayList<VkBufferMemoryBarrier>				_bufferBarriers;

		TransferBuffer									_frameTransfers;
		std::atomic<VkDeviceSize>						_readOffset;
		std::atomic<VkDeviceSize>						_writeOffset;

		VkSemaphore										_phaseCompleted[Phase::COUNT];
		VkFence											_commandsConsumed;
		VkEvent											_ioCompleted;

		CommandList										_commandLists[Phase::COUNT];
		CommandList										_barrierCommandList;

	// ---------------------------------------------------

		friend void Swap( IoBuilder&, IoBuilder& );
	};

}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2