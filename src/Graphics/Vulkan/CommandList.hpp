/*==================================================================*\
  CommandList.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/Gpu.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics {
	namespace Vulkan {
		class GraphicsPipeline;
		class Framebuffer;
	} // namespace Vulkan

	class RenderView;
}} // namespace Eldritch2::Graphics

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	class AbstractCommandList {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		AbstractCommandList(const AbstractCommandList&) = delete;
		//!	Constructs this @ref AbstractCommandList instance.
		AbstractCommandList(AbstractCommandList&&) ETNoexceptHint;
		//!	Constructs this @ref AbstractCommandList instance.
		ETConstexpr AbstractCommandList() ETNoexceptHint;

		~AbstractCommandList();

		// ---------------------------------------------------

	public:
		VkResult BindResources(Gpu& gpu, QueueClass queue);

		void FreeResources(Gpu& gpu) ETNoexceptHint;

		// ---------------------------------------------------

	protected:
		VkResult Submit(Gpu& gpu, VkFence consumed, uint32 submitCount, const VkSubmitInfo submits[]) ETNoexceptHint;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		AbstractCommandList& operator=(const AbstractCommandList&) = delete;

		// - DATA MEMBERS ------------------------------------

	protected:
		VkCommandPool   _pool;
		VkCommandBuffer _commands;
	};

	// ---

	class GraphicsCommandList : public AbstractCommandList {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref GraphicsCommandList instance.
		GraphicsCommandList(GraphicsCommandList&&) ETNoexceptHint = default;
		//!	Disable copy construction.
		GraphicsCommandList(const GraphicsCommandList&) = delete;
		//!	Constructs this @ref GraphicsCommandList instance.
		GraphicsCommandList() ETNoexceptHint = default;

		~GraphicsCommandList() = default;

		// ---------------------------------------------------

	public:
		void ExecutePipeline(Framebuffer& target, const GraphicsPipeline& pipeline, Span<const RenderView*> views) ETNoexceptHint;

		// ---------------------------------------------------

	public:
		VkResult Submit(Gpu& gpu, VkFence consumed, uint32 waitCount, const VkSemaphore waits[], const VkPipelineStageFlags stages[]) ETNoexceptHint;
		VkResult Submit(Gpu& gpu, VkFence consumed = VK_NULL_HANDLE) ETNoexceptHint;
	};

	// ---

	class TransferCommandList : public AbstractCommandList {
		// - TYPE PUBLISHING ---------------------------------

	public:
		enum : size_t {
			BindInfos,
			BindLists
		};

		// ---

	public:
		using MemoryBindList      = ArrayList<VkSparseMemoryBind>;
		using ImageMemoryBindList = ArrayList<VkSparseImageMemoryBind>;
		using BufferBindList      = SoaList<VkSparseBufferMemoryBindInfo, MemoryBindList>;
		using ImageBindList       = SoaList<VkSparseImageMemoryBindInfo, ImageMemoryBindList>;
		using ImageOpaqueBindList = SoaList<VkSparseImageOpaqueMemoryBindInfo, MemoryBindList>;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		TransferCommandList(const TransferCommandList&) = delete;
		//!	Constructs this @ref TransferCommandList instance.
		TransferCommandList(TransferCommandList&&) ETNoexceptHint = default;
		//!	Constructs this @ref TransferCommandList instance.
		TransferCommandList() ETNoexceptHint;

		~TransferCommandList() = default;

		// ---------------------------------------------------

	public:
		void QueuePush(VkImage target, VkBuffer source, VkOffset3D offset, VkExtent3D extent, VkImageSubresourceLayers layers);
		void QueuePush(VkBuffer target, VkBuffer source, VkDeviceSize offset, VkDeviceSize extent);

		void QueuePull(VkBuffer target, VkImage source, VkOffset3D offset, VkExtent3D extent, VkImageSubresourceLayers layers);
		void QueuePull(VkBuffer target, VkBuffer source, VkDeviceSize offset, VkDeviceSize extent);

		void QueueBind(VkImage target, const VkSparseImageMemoryBind& bind);
		void QueueBind(VkBuffer target, const VkSparseMemoryBind& bind);
		void QueueBind(VkImage target, const VkSparseMemoryBind& bind);

		// ---------------------------------------------------

	public:
		VkResult Submit(Gpu& gpu, VkFence consumed, uint32 waitCount, const VkSemaphore waits[], const VkPipelineStageFlags stages[]) ETNoexceptHint;
		VkResult Submit(Gpu& gpu, VkFence consumed = VK_NULL_HANDLE) ETNoexceptHint;

		// ---------------------------------------------------

	private:
		static ETConstexpr VkSparseBufferMemoryBindInfo CreateBindInfo(VkBuffer buffer) ETNoexceptHint;
		static ETConstexpr VkSparseImageMemoryBindInfo CreateBindInfo(VkImage image) ETNoexceptHint;

		static ETConstexpr VkSparseImageOpaqueMemoryBindInfo CreateOpaqueBindInfo(VkImage image) ETNoexceptHint;

		// - DATA MEMBERS ------------------------------------

	private:
		BufferBindList      _bufferBinds;
		ImageBindList       _imageBinds;
		ImageOpaqueBindList _imageOpaqueBinds;
	};

}}} // namespace Eldritch2::Graphics::Vulkan

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/Vulkan/CommandList.inl>
//------------------------------------------------------------------//
