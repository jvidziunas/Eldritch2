/*==================================================================*\
  GpuResourceBus.hpp
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
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {
	class Gpu;
}}} // namespace Eldritch2::Graphics::Vulkan

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	class FrameTransferBus {
		// - TYPE PUBLISHING ---------------------------------

	public:
		template <typename TargetResource, typename BindStruct>
		class PendingBind : public BindStruct {
			// - TYPE PUBLISHING ---------------------------------
		public:
			using TargetType = typename TargetResource;
			using BindType   = typename BindStruct;

			// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref PendingBind instance.
			PendingBind(TargetResource target, const BindStruct& bind);
			//!	Constructs this @ref PendingBind instance.
			PendingBind(const PendingBind&) = default;

			~PendingBind() = default;

			// - DATA MEMBERS ------------------------------------

		public:
			TargetResource target;
		};

		// ---

	public:
		enum Phase : uint32 {
			StandardDownload,
			SparseBind,
			StandardUpload,
			SparseUpload,

			COUNT
		};

		// ---

	public:
		using PendingImageBind       = PendingBind<VkImage, VkSparseImageMemoryBind>;
		using PendingImageOpaqueBind = PendingBind<VkImage, VkSparseMemoryBind>;
		using PendingBufferBind      = PendingBind<VkBuffer, VkSparseMemoryBind>;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		FrameTransferBus(const FrameTransferBus&) = delete;
		//!	Constructs this @ref GpuResourceBus instance.
		FrameTransferBus(FrameTransferBus&&);
		//!	Constructs this @ref GpuResourceBus instance.
		FrameTransferBus();

		~FrameTransferBus() = default;

		// ---------------------------------------------------

	public:
		void SendToGpu(VkImage target, VkOffset3D targetOffset, VkExtent3D targetExtent, VkImageSubresourceLayers subresource);
		void SendToGpu(VkBuffer target, VkDeviceSize targetOffset, VkDeviceSize targetExtent);

		void SendToHost(VkImage source, VkOffset3D sourceOffset, VkExtent3D sourceExtent, VkImageSubresourceLayers subresource);
		void SendToHost(VkBuffer source, VkDeviceSize sourceOffset, VkDeviceSize sourceExtent);

		void BindRegion(VkImage target, const VkSparseImageMemoryBind& bind);
		void BindRegion(VkImage target, const VkSparseMemoryBind& bind);

		// ---------------------------------------------------

	public:
		VkResult SubmitCommands(Gpu& gpu);

		bool CheckCommandsConsumed(Gpu& gpu) const;

		// ---------------------------------------------------

	public:
		VkResult InsertSignalSemaphore(Gpu& gpu, VkSemaphore& outSemaphore);

		void EraseSignalSemaphore(Gpu& gpu, VkSemaphore semaphore);

		// ---------------------------------------------------

	public:
		VkResult BindResources(Gpu& gpu, VkDeviceSize transferBufferSize);

		void FreeResources(Gpu& gpu);

		// ---------------------------------------------------

		//!	Disable copy assignment.
		FrameTransferBus& operator=(const FrameTransferBus&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		ArrayList<PendingImageOpaqueBind> _pendingImageOpaqueBinds;
		ArrayList<PendingImageBind>       _pendingImageBinds;
		Mutex                             _signalsMutex;
		ArraySet<VkSemaphore>             _signals;
		TransferBuffer                    _stagingBuffer;
		Atomic<VkDeviceSize>              _readOffset;
		Atomic<VkDeviceSize>              _writeOffset;
		VkFence                           _commandsConsumed;
		VkCommandPool                     _commandPool;
		VkCommandBuffer                   _commands[Phase::COUNT];
		VkSemaphore                       _phaseCompleted[Phase::COUNT];

		// ---------------------------------------------------

		friend void Swap(FrameTransferBus&, FrameTransferBus&);
	};

}}} // namespace Eldritch2::Graphics::Vulkan

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/Vulkan/FrameTransferBus.inl>
//------------------------------------------------------------------//
