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
#include <Graphics/Vulkan/GpuResourceApi.hpp>
#include <Graphics/GpuAbi.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics {
	namespace Vulkan {
		class Gpu;
	}

	class ImageSource;
	template <typename Vertex>
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
		class BindPhase {
			// - TYPE PUBLISHING ---------------------------------

		public:
			using WaitList   = ArrayList<VkSemaphore>;
			using SignalList = ArrayList<VkSemaphore>;

			// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Disable copy construction.
			BindPhase(const BindPhase&) = delete;
			//!	Constructs this @ref BindPhase instance.
			BindPhase(BindPhase&&);
			//!	Constructs this @ref BindPhase instance.
			BindPhase();

			~BindPhase();

			// - DATA MEMBERS ------------------------------------

		public:
			VkSemaphore                                           complete;
			WaitList                                              waits;
			SignalList                                            signals;
			ArrayMap<VkImage, ArrayList<VkSparseImageMemoryBind>> imageBinds;
			ArrayMap<VkImage, ArrayList<VkSparseMemoryBind>>      imageOpaqueBinds;
			ArrayMap<VkBuffer, ArrayList<VkSparseMemoryBind>>     bufferBinds;

			// ---------------------------------------------------

			friend void Swap(BindPhase&, BindPhase&);
		};

		// ---

	public:
		class TransferPhase {
			// - TYPE PUBLISHING ---------------------------------

		public:
			using WaitList   = SoArrayList<VkSemaphore, VkPipelineStageFlags>;
			using SignalList = ArrayList<VkSemaphore>;

			// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//! Disable copy construction.
			TransferPhase(const TransferPhase&) = delete;
			//! Constructs this @ref Phase instance.
			TransferPhase(TransferPhase&&);
			//! Constructs this @ref Phase instance.
			TransferPhase();

			~TransferPhase();

			// - DATA MEMBERS ------------------------------------

		public:
			WaitList        waits;
			SignalList      signals;
			VkCommandBuffer commands[1];

			// ---------------------------------------------------

			friend void Swap(TransferPhase&, TransferPhase&);
		};

		// ---

	public:
		class Frame {
			// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Disable copy construction.
			Frame(const Frame&) = delete;
			//!	Constructs this @ref Frame instance.
			Frame();

			~Frame();

			// ---------------------------------------------------

		public:
			bool GetTransfersConsumed(Gpu& gpu) const ETNoexceptHint;

			bool GetBindsConsumed(Gpu& gpu) const ETNoexceptHint;

			// ---------------------------------------------------

		public:
			void PushToGpu(VkImage target, VkOffset3D offset, VkExtent3D extent, VkImageSubresourceLayers layers);
			void PushToGpu(VkBuffer target, VkDeviceSize offset, VkDeviceSize extent);

			void PullToHost(VkImage source, VkOffset3D offset, VkExtent3D extent, VkImageSubresourceLayers layers);
			void PullToHost(VkBuffer source, VkDeviceSize offset, VkDeviceSize extent);

			void Bind(VkImage target, const VkSparseImageMemoryBind& bind);
			void Bind(VkImage target, const VkSparseMemoryBind& bind);
			void Bind(VkBuffer target, const VkSparseMemoryBind& bind);

			// ---------------------------------------------------

		public:
			VkResult SubmitIo(Gpu& gpu);

			// ---------------------------------------------------

		public:
			VkResult MakeResident(Mesh& target, const MeshSource<SkinnedVertex>& source);
			VkResult MakeResident(ShaderImage& target, const ImageSource& source);

			// ---------------------------------------------------

		public:
			void InsertSignal(GpuIoPhase phase, VkSemaphore signal);

			void InsertWait(GpuIoPhase phase, VkSemaphore wait);

			void EraseSignal(GpuIoPhase phase, VkSemaphore signal);

			void EraseWait(GpuIoPhase phase, VkSemaphore wait);

			// ---------------------------------------------------

		public:
			VkResult BindResources(Gpu& gpu);

			void FreeResources(Gpu& gpu);

			// - DATA MEMBERS ------------------------------------

		private:
			VkBuffer      _stagingBuffer;
			VkFence       _bindsConsumed;
			VkFence       _transfersConsumed;
			VkCommandPool _commandPool;
			Mutex         _bindsMutex;
			BindPhase     _sparseBinds;
			Mutex         _semaphoresMutex;
			TransferPhase _phases[GpuIoPhases];

			// ---------------------------------------------------

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
		ETConstexpr auto GetFrames() const ETNoexceptHint -> const Frame (&)[2];
		ETConstexpr auto GetFrames() ETNoexceptHint -> Frame (&)[2];

		// ---------------------------------------------------

	public:
		const Mesh*        Find(const MeshSource<SkinnedVertex>& mesh) const ETNoexceptHint;
		const ShaderImage* Find(const ImageSource& image) const ETNoexceptHint;

		// ---------------------------------------------------

	public:
		VkResult Insert(Gpu& gpu, const MeshSource<SkinnedVertex>& mesh, bool andMakeResident = true);
		VkResult Insert(Gpu& gpu, const ImageSource& image, bool andMakeResident = true);

		void Erase(Gpu& gpu, const ImageSource& image);
		void Erase(Gpu& gpu, const MeshSource<SkinnedVertex>& mesh);

		// ---------------------------------------------------

	public:
		VkResult BindResources(Gpu& gpu, VkDeviceSize transferBufferSize, VkDeviceSize vertexCacheSize, VkDeviceSize indexCacheSize);

		void FreeResources(Gpu& gpu);

		// ---------------------------------------------------

		//!	Disable copy assignment.
		ResourceBus& operator=(const ResourceBus&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		VertexCache                                  _vertexCache;
		UploadBuffer                                 _uploadBuffer;
		ResidentSet<MeshSource<SkinnedVertex>, Mesh> _meshesBySource;
		ResidentSet<ImageSource, ShaderImage>        _imagesBySource;
		Frame                                        _frames[2];

		// ---------------------------------------------------

		friend void Swap(ResourceBus&, ResourceBus&);
	};

}}} // namespace Eldritch2::Graphics::Vulkan

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/Vulkan/ResourceBus.inl>
//------------------------------------------------------------------//
