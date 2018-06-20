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

	class GpuResourceBus {
		// - TYPE PUBLISHING ---------------------------------

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

			TargetResource target;
		};

		// ---

	public:
		using PendingImageBind       = PendingBind<VkImage, VkSparseImageMemoryBind>;
		using PendingImageOpaqueBind = PendingBind<VkImage, VkSparseMemoryBind>;
		using PendingBufferBind      = PendingBind<VkBuffer, VkSparseMemoryBind>;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		GpuResourceBus(const GpuResourceBus&) = delete;
		//!	Constructs this @ref GpuResourceBus instance.
		GpuResourceBus(GpuResourceBus&&);
		//!	Constructs this @ref GpuResourceBus instance.
		GpuResourceBus();

		~GpuResourceBus() = default;

		// ---------------------------------------------------

	public:
		void TransferToGpu(VkImage target, VkImageSubresourceLayers subresource, VkOffset3D targetOffset, VkExtent3D targetExtent);
		void TransferToGpu(VkBuffer target, VkDeviceSize targetOffset, VkDeviceSize targetExtent);

		void TransferToHost(VkImage source, VkImageSubresourceLayers subresource, VkOffset3D sourceOffset, VkExtent3D sourceExtent);
		void TransferToHost(VkBuffer source, VkDeviceSize sourceOffset, VkDeviceSize sourceExtent);

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
		GpuResourceBus& operator=(const GpuResourceBus&) = delete;

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

		friend void Swap(GpuResourceBus&, GpuResourceBus&);
	};

}}} // namespace Eldritch2::Graphics::Vulkan

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/Vulkan/GpuResourceBus.inl>
//------------------------------------------------------------------//
