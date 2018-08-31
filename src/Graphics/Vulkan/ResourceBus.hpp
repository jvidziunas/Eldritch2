/*==================================================================*\
  ResourceBus.hpp
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

namespace Eldritch2 { namespace Graphics {
	namespace Vulkan {
		class Gpu;
	}

	class ImageSource;
	class MeshSource;
}} // namespace Eldritch2::Graphics

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	enum GpuIoPhase : uint32 {
		StandardDownload,
		StandardUpload,
		SparseUpload,

		GpuIoPhases
	};

	// ---

	class ResourceBus {
		// - TYPE PUBLISHING ---------------------------------

	public:
		struct BindPhase {
			using WaitList   = ArrayList<VkSemaphore>;
			using SignalList = ArrayList<VkSemaphore>;

			WaitList                                  waits;
			SignalList                                signals;
			HashMap<VkImage, VkSparseImageMemoryBind> imageBinds;
			HashMap<VkImage, VkSparseMemoryBind>      imageOpaqueBinds;
			HashMap<VkBuffer, VkSparseMemoryBind>     bufferBinds;
		};

		// ---

	public:
		struct Phase {
			using WaitList   = SoArrayList<VkSemaphore, VkPipelineStageFlags>;
			using SignalList = ArrayList<VkSemaphore>;

			WaitList        waits;
			SignalList      signals;
			VkCommandBuffer commands;
		};

		// ---

	public:
		struct Frame {
			VkFence       bindsConsumed;
			VkFence       transfersConsumed;
			VkCommandPool commandPool;
			BindPhase     sparseBinds;
			Phase         phases[GpuIoPhases];

			friend void Swap(Frame&, Frame&);
		};

		// ---

	public:
		template <typename Source, typename Resource>
		using ResidentSet = HashMap<const Source*, Resource>;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		ResourceBus(const ResourceBus&) = delete;
		//!	Constructs this @ref ResourceBus instance.
		ResourceBus(ResourceBus&&);
		//!	Constructs this @ref ResourceBus instance.
		ResourceBus();

		~ResourceBus() = default;

		// ---------------------------------------------------

	public:
		bool GetTransfersConsumed(Gpu& gpu) const ETNoexceptHint;

		bool GetBindsConsumed(Gpu& gpu) const ETNoexceptHint;

		// ---------------------------------------------------

	public:
		VkResult SubmitFrameIo(Gpu& gpu);

		// ---------------------------------------------------

	public:
		void PushToGpu(VkImage target, VkOffset3D offset, VkExtent3D extent, VkImageSubresourceLayers subresource);
		void PushToGpu(VkBuffer target, VkDeviceSize offset, VkDeviceSize extent);

		void PullToHost(VkImage source, VkOffset3D offset, VkExtent3D extent, VkImageSubresourceLayers subresource);
		void PullToHost(VkBuffer source, VkDeviceSize offset, VkDeviceSize extent);

		void BindRegion(VkImage target, const VkSparseImageMemoryBind& bind);
		void BindRegion(VkImage target, const VkSparseMemoryBind& bind);
		void BindRegion(VkBuffer target, const VkSparseMemoryBind& bind);

		// ---------------------------------------------------

	public:
		const ShaderImage* Find(const ImageSource& image) const ETNoexceptHint;
		const Mesh*        Find(const MeshSource& mesh) const ETNoexceptHint;

		// ---------------------------------------------------

	public:
		VkResult Insert(Gpu& gpu, const ImageSource& image, bool andMakeResident = true);
		VkResult Insert(Gpu& gpu, const MeshSource& mesh, bool andMakeResident = true);

		void Erase(Gpu& gpu, const ImageSource& image);
		void Erase(Gpu& gpu, const MeshSource& mesh);

		// ---------------------------------------------------

	public:
		void InsertSignal(uint32 frame, GpuIoPhase phase, VkSemaphore signal);
		void InsertWait(uint32 frame, GpuIoPhase phase, VkSemaphore wait);

		void EraseSignal(uint32 frame, GpuIoPhase phase, VkSemaphore signal);
		void EraseWait(uint32 frame, GpuIoPhase phase, VkSemaphore wait);

		// ---------------------------------------------------

	public:
		VkResult BindResources(Gpu& gpu, VkDeviceSize transferBufferSize, VkDeviceSize vertexCacheSize, VkDeviceSize indexCacheSize);

		void FreeResources(Gpu& gpu);

		// ---------------------------------------------------

	private:
		VkResult MakeResident(ShaderImage& target, const ImageSource& source);
		VkResult MakeResident(Mesh& target, const MeshSource& source);

		// ---------------------------------------------------

		//!	Disable copy assignment.
		ResourceBus& operator=(const ResourceBus&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		Mutex                                 _semaphoresMutex;
		TransferBuffer                        _stagingBuffer;
		VertexCache                           _vertexCache;
		Frame                                 _frames[2];
		ResidentSet<MeshSource, Mesh>         _meshesBySource;
		ResidentSet<ImageSource, ShaderImage> _imagesBySource;

		// ---------------------------------------------------

		friend void Swap(ResourceBus&, ResourceBus&);
	};

}}} // namespace Eldritch2::Graphics::Vulkan

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/Vulkan/ResourceBus.inl>
//------------------------------------------------------------------//
