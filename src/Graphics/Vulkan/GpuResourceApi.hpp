/*==================================================================*\
  GpuResourceApi.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/GraphicsPipelineApi.hpp>
//------------------------------------------------------------------//
#include <vk_mem_alloc.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {
	class PipelineAttachmentDescription;
	class GraphicsPipelineBuilder;
	class Gpu;
}}} // namespace Eldritch2::Graphics::Vulkan

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	class ETPureAbstractHint GpuBuffer {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
		//!	Disable copy construction.
		GpuBuffer(const GpuBuffer&) = delete;
		//!	Disable move construction; clients should implement themselves via Swap().
		GpuBuffer(GpuBuffer&&) = delete;
		//!	Constructs this @ref Buffer instance.
		GpuBuffer();

		~GpuBuffer();

		// ---------------------------------------------------

	public:
		ETConstexpr operator VkBuffer() const ETNoexceptHint;

		ETConstexpr VkBuffer Get() const ETNoexceptHint;

		// ---------------------------------------------------

	public:
		void FreeResources(Gpu& gpu);

		// ---------------------------------------------------

	protected:
		VkResult BindResources(Gpu& gpu, const VkBufferCreateInfo& bufferInfo, const VmaAllocationCreateInfo& allocationInfo);

		// ---------------------------------------------------

		//!	Disable copy assignment.
		GpuBuffer& operator=(const GpuBuffer&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		VmaAllocation _backing;
		VkBuffer      _buffer;

		// ---------------------------------------------------

		friend void Swap(GpuBuffer&, GpuBuffer&);
	};

	// ---

	class ETPureAbstractHint GpuImage {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
		//!	Disable copy construction.
		GpuImage(const GpuImage&) = delete;
		//!	Disable move construction; clients should implement themselves via Swap().
		GpuImage(GpuImage&&) = delete;
		//!	Constructs this @ref Image instance.
		GpuImage();

		~GpuImage();

		// ---------------------------------------------------

	public:
		ETConstexpr VkImage Get() const ETNoexceptHint;

		ETConstexpr operator VkImage() const ETNoexceptHint;

		// ---------------------------------------------------

	public:
		void FreeResources(Gpu& gpu);

		// ---------------------------------------------------

	protected:
		VkResult BindResources(Gpu& gpu, const VkImageCreateInfo& imageInfo, const VmaAllocationCreateInfo& allocationInfo);

		// ---------------------------------------------------

		//!	Disable copy assignment.
		GpuImage& operator=(const GpuImage&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		VmaAllocation _backing;
		VkImage       _image;

		// ---------------------------------------------------

		friend void Swap(GpuImage&, GpuImage&);
	};

	// ---

	class VertexBuffer : public GpuBuffer {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		VertexBuffer(const VertexBuffer&) = delete;
		//!	Constructs this @ref VertexBuffer instance.
		VertexBuffer(VertexBuffer&&);
		//!	Constructs this @ref VertexBuffer instance.
		VertexBuffer() = default;

		~VertexBuffer() = default;

		// ---------------------------------------------------

	public:
		VkResult BindResources(Gpu& gpu, VkDeviceSize sizeInBytes);

		// ---------------------------------------------------

		//!	Disable copy assignment.
		VertexBuffer& operator=(const VertexBuffer&) = delete;

		// ---------------------------------------------------

		friend void Swap(VertexBuffer&, VertexBuffer&);
	};

	// ---

	class IndexBuffer : public GpuBuffer {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		IndexBuffer(const IndexBuffer&) = delete;
		//!	Constructs this @ref IndexBuffer instance.
		IndexBuffer(IndexBuffer&&);
		//!	Constructs this @ref IndexBuffer instance.
		IndexBuffer() = default;

		~IndexBuffer() = default;

		// ---------------------------------------------------

	public:
		VkResult BindResources(Gpu& gpu, VkDeviceSize sizeInBytes);

		// ---------------------------------------------------

		//!	Disable copy assignment.
		IndexBuffer& operator=(const IndexBuffer&) = delete;

		// ---------------------------------------------------

		friend void Swap(IndexBuffer&, IndexBuffer&);
	};

	// ---

	class UploadBuffer : public GpuBuffer {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		UploadBuffer(const UploadBuffer&) = delete;
		//!	Constructs this @ref TransferBuffer instance.
		UploadBuffer(UploadBuffer&&);
		//!	Constructs this @ref TransferBuffer instance.
		UploadBuffer() = default;

		~UploadBuffer() = default;

		// ---------------------------------------------------

	public:
		VkResult BindResources(Gpu& gpu, VkDeviceSize sizeInBytes);

		// ---------------------------------------------------

		//!	Disable copy assignment.
		UploadBuffer& operator=(const UploadBuffer&) = delete;

		// ---------------------------------------------------

		friend void Swap(UploadBuffer&, UploadBuffer&);
	};

	// ---

	class UniformBuffer : public GpuBuffer {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		UniformBuffer(const UniformBuffer&) = delete;
		//!	Constructs this @ref UniformBuffer instance.
		UniformBuffer(UniformBuffer&&);
		//!	Constructs this @ref UniformBuffer instance.
		UniformBuffer() = default;

		~UniformBuffer() = default;

		// ---------------------------------------------------

	public:
		VkResult BindResources(Gpu& gpu, VkDeviceSize sizeInBytes);

		// ---------------------------------------------------

		//!	Disable copy assignment.
		UniformBuffer& operator=(const UniformBuffer&) = delete;

		// ---------------------------------------------------

		friend void Swap(UniformBuffer&, UniformBuffer&);
	};

	// ---

	class VertexCache {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		VertexCache(const VertexCache&) = delete;
		//! Constructs this @ref VertexCache instance.
		VertexCache();

		~VertexCache() = default;

		// ---------------------------------------------------

	public:
		VkResult AllocateVertices(VkDeviceSize& outOffset, VkDeviceSize size);
		VkResult AllocateIndices(VkDeviceSize& outOffset, VkDeviceSize size);

		void DeallocateVertices(VkDeviceSize offset, VkDeviceSize size);
		void DeallocateIndices(VkDeviceSize offset, VkDeviceSize size);

		// ---------------------------------------------------

	public:
		ETConstexpr VkBuffer GetVertexBuffer() const ETNoexceptHint;

		ETConstexpr VkBuffer GetIndexBuffer() const ETNoexceptHint;

		// ---------------------------------------------------

	public:
		VkResult BindResources(Gpu& gpu, VkDeviceSize vertexCacheSize, VkDeviceSize indexCacheSize);

		void FreeResources(Gpu& gpu);

		// ---------------------------------------------------

		//!	Disable copy assignment.
		VertexCache& operator=(const VertexCache&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		VertexBuffer                 _vertices;
		IdentifierPool<VkDeviceSize> _vertexRanges;
		IndexBuffer                  _indices;
		IdentifierPool<VkDeviceSize> _indexRanges;

		// ---------------------------------------------------

		friend void Swap(VertexCache&, VertexCache&);
	};

	// ---

	class Mesh {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		Mesh(const Mesh&) = delete;
		//!	Constructs this @ref Mesh instance.
		Mesh(Mesh&&);
		//!	Constructs this @ref Mesh instance.
		Mesh() ETNoexceptHint;

		~Mesh() = default;

		// ---------------------------------------------------

	public:
		ETConstexpr VkBuffer GetVertices() const ETNoexceptHint;

		ETConstexpr VkBuffer GetIndices() const ETNoexceptHint;

		ETConstexpr VkDeviceSize GetVertexOffset() const ETNoexceptHint;

		ETConstexpr VkDeviceSize GetIndexOffset() const ETNoexceptHint;

		ETConstexpr VkDeviceSize GetVerticesSize() const ETNoexceptHint;

		ETConstexpr VkDeviceSize GetIndicesSize() const ETNoexceptHint;

		// ---------------------------------------------------

	public:
		VkResult BindResources(VertexCache& cache, VkDeviceSize verticesSize, VkDeviceSize indicesSize);

		void FreeResources(VertexCache& cache);

		// ---------------------------------------------------

		//!	Disable copy assignment.
		Mesh& operator=(const Mesh&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		VkBuffer     _vertices;
		VkBuffer     _indices;
		VkDeviceSize _verticesOffset;
		VkDeviceSize _indicesOffset;
		VkDeviceSize _verticesSize;
		VkDeviceSize _indicesSize;

		// ---------------------------------------------------

		friend void Swap(Mesh&, Mesh&) ETNoexceptHint;
	};

	// ---

	class ShaderImage : public GpuImage {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		ShaderImage(const ShaderImage&) = delete;
		//!	Constructs this @ref ShaderImage instance.
		ShaderImage(ShaderImage&&);
		//!	Constructs this @ref ShaderImage instance.
		ShaderImage() = default;

		~ShaderImage() = default;

		// ---------------------------------------------------

	public:
		VkResult BindResources(Gpu& gpu, VkFormat format, VkExtent3D extent, uint32_t mips, uint32_t layers);

		// ---------------------------------------------------

		//!	Disable copy assignment.
		ShaderImage& operator=(const ShaderImage&) = delete;

		// ---------------------------------------------------

		friend void Swap(ShaderImage&, ShaderImage&);
	};

	// ---

	class TileManager {
		// - TYPE PUBLISHING ---------------------------------

	public:
		enum : uint32 {
			PageCoordinateBits  = 18u,
			MaxImageDimension   = 1u << PageCoordinateBits,
			PageSubresourceBits = 10u
		};

		// ---

	public:
		struct Tile {
			uint64 x : PageCoordinateBits;
			uint64 y : PageCoordinateBits;
			uint64 z : PageCoordinateBits;
			uint64 subresource : PageSubresourceBits;
		};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		TileManager(const TileManager&) = delete;
		//!	Constructs this @ref TileManager instance.
		TileManager(TileManager&&);
		//!	Constructs this @ref TileManager instance.
		TileManager();

		~TileManager() = default;

		// ---------------------------------------------------

	public:
		ETConstexpr VkExtent3D GetTileExtent() const ETNoexceptHint;

		// ---------------------------------------------------

	public:
		bool Touch(Tile tile) ETNoexceptHint;

		void ReplaceLru(Tile tile) ETNoexceptHint;

		// ---------------------------------------------------

	public:
		VkResult BindResources(Gpu& gpu, VkExtent3D imageExtent, VkExtent3D tileExtent);

		void FreeResources(Gpu& gpu);

		// ---------------------------------------------------

		//!	Disable copy assignment.
		TileManager& operator=(const TileManager&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		HashSet<Tile> _residentTiles;
		VkExtent3D    _tileExtent;

		// ---------------------------------------------------

		friend void Swap(TileManager&, TileManager&);
	};

	// ---

	class HostSparseTileCache : public GpuBuffer {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		HostSparseTileCache(const HostSparseTileCache&) = delete;
		//!	Constructs this @ref SparseTileCache instance.
		HostSparseTileCache(HostSparseTileCache&&);
		//!	Constructs this @ref SparseTileCache instance.
		HostSparseTileCache();

		~HostSparseTileCache() = default;

		// ---------------------------------------------------

	public:
		bool IsCached(VkDeviceSize& outOffset, TileManager::Tile tile) const ETNoexceptHint;

		bool IsLoading(TileManager::Tile tile) const ETNoexceptHint;

		void NotifyCached(VkDeviceSize offset, TileManager::Tile tile);

		// ---------------------------------------------------

	public:
		VkResult BindResources(Gpu& gpu, const TileManager& client, uint32 cachedTiles);

		void FreeResources(Gpu& gpu);

		// ---------------------------------------------------

		//! Disable copy assignment.
		HostSparseTileCache& operator=(const HostSparseTileCache&) = delete;

		// ---------------------------------------------------

	private:
		HashMap<TileManager::Tile, VkDeviceSize> _cachedPagesByTile;
		HashSet<TileManager::Tile>               _loadingTiles;

		// ---------------------------------------------------

		friend void Swap(HostSparseTileCache&, HostSparseTileCache&);
	};

	// ---

	class SparseShaderImage : public GpuImage {
		// - TYPE PUBLISHING ---------------------------------

	public:
		enum class CacheResult {
			Hit,
			L0Miss,
			L1Miss
		};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		SparseShaderImage(const SparseShaderImage&) = delete;
		//!	Constructs this @ref SparseShaderImage instance.
		SparseShaderImage(SparseShaderImage&&);
		//!	Constructs this @ref SparseShaderImage instance.
		SparseShaderImage() = default;

		~SparseShaderImage() = default;

		// ---------------------------------------------------

	public:
		ETConstexpr const TileManager& GetTileManager() const ETNoexceptHint;

		// ---------------------------------------------------

	public:
		CacheResult MakeResident(const HostSparseTileCache& cache, TileManager::Tile tile);

		// ---------------------------------------------------

	public:
		VkResult BindResources(Gpu& gpu, VkFormat format, VkExtent3D extent, uint32 mips, uint32 layers);

		void FreeResources(Gpu& gpu);

		// ---------------------------------------------------

		//!	Disable copy assignment.
		SparseShaderImage& operator=(const SparseShaderImage&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		TileManager _tileManager;

		// ---------------------------------------------------

		friend void Swap(SparseShaderImage&, SparseShaderImage&);
	};

	// ---

	class Framebuffer {
		// - TYPE PUBLISHING ---------------------------------

	public:
		using ExternalImageMap = ArrayMap<size_t /*index*/, VkImage /*image*/>;

		// ---

		class Image {
			// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Disable copy construction.
			Image(const Image&) = delete;
			//! Constructs this @ref Image instance.
			Image(Image&&) ETNoexceptHint;
			//! Constructs this @ref Image instance.
			Image() ETNoexceptHint;

			~Image();

			// ---------------------------------------------------

		public:
			ETConstexpr operator VkImage() const ETNoexceptHint;

			// ---------------------------------------------------

		public:
			VkResult BindResources(Gpu& gpu, const PipelineAttachmentDescription& description, VkExtent2D baseDimensions, uint32 baseLayers);
			VkResult BindResources(Gpu& gpu, const PipelineAttachmentDescription& description, VkImage externalImage);

			void FreeResources(Gpu& gpu);

			// ---------------------------------------------------

			//!	Disable copy assignment.
			Image& operator=(const Image&) = delete;

			// - DATA MEMBERS ------------------------------------

		private:
			VkImage       _image;
			VmaAllocation _backing;
			bool          _ownsImage;

			// ---------------------------------------------------

			friend void Swap(Image&, Image&) ETNoexceptHint;
		};

		// ---

	public:
		class PassImages {
			// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Disable copy construction.
			PassImages(const PassImages&) = delete;
			//!	Constructs this @ref PassImages instance.
			PassImages(PassImages&&) ETNoexceptHint;
			//!	Constructs this @ref PassImages instance.
			PassImages() ETNoexceptHint;

			~PassImages();

			// ---------------------------------------------------

		public:
			VkResult BindResources(Gpu& gpu, const PipelinePassDescription& pass, const PipelineAttachmentDescription descriptions[], const Image images[]);

			void FreeResources(Gpu& gpu);

			// ---------------------------------------------------

			//!	Disable copy assignment.
			PassImages& operator=(const PassImages&) = delete;

			// - DATA MEMBERS ------------------------------------

		public:
			VkImageView views[ETCountOf(PipelineFramebufferDescription::colorAttachments) + ETCountOf(PipelineFramebufferDescription::inputAttachments) + 1];

			// ---------------------------------------------------

			friend void Swap(PassImages&, PassImages&) ETNoexceptHint;
		};

		// ---

		class Pass {
			// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref Pass instance.
			Pass(Pass&&) ETNoexceptHint;
			//!	Disable copy construction.
			Pass(const Pass&) = delete;
			//!	Constructs this @ref Pass instance.
			Pass() ETNoexceptHint;

			~Pass();

			// ---------------------------------------------------

		public:
			ETConstexpr VkExtent2D GetRenderExtent() const ETNoexceptHint;

			// ---------------------------------------------------

		public:
			VkResult BindResources(Gpu& gpu, const PipelinePassDescription& pass, VkExtent2D dimensions, uint32 layers, const PassImages& images);

			void FreeResources(Gpu& gpu);

			// ---------------------------------------------------

			//!	Disable copy assignment.
			Pass& operator=(const Pass&) = delete;

			// - DATA MEMBERS ------------------------------------

		public:
			VkRenderPass     renderPass;
			VkFramebuffer    framebuffer;
			ResolutionScaler scaler;
			VkExtent2D       granularity;

			// ---------------------------------------------------

			friend void Swap(Pass&, Pass&) ETNoexceptHint;
		};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		Framebuffer(const Framebuffer&) = delete;
		//!	Constructs this @ref Framebuffer instance.
		Framebuffer(Framebuffer&&);
		//!	Constructs this @ref Framebuffer instance.
		Framebuffer();

		~Framebuffer();

		// ---------------------------------------------------

	public:
		ETConstexpr VkQueryPool GetTimingPool() const ETNoexceptHint;

		Range<const Pass*> GetPasses() const ETNoexceptHint;

		// ---------------------------------------------------

	public:
		VkResult UpdateScaling(Gpu& gpu);

		// ---------------------------------------------------

	public:
		VkResult BindResources(Gpu& gpu, const GraphicsPipelineBuilder& pipeline, VkExtent2D dimensions, uint32 layers, ExternalImageMap externalImages = ExternalImageMap {});

		void FreeResources(Gpu& gpu);

		// ---------------------------------------------------

		//!	Disable copy assignment.
		Framebuffer& operator=(const Framebuffer&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		VkQueryPool                   _timingPool;
		ArrayList<Image>              _images;
		SoArrayList<PassImages, Pass> _passes;

		// ---------------------------------------------------

		friend void Swap(Framebuffer&, Framebuffer&) ETNoexceptHint;
	};

	// ---

	class Viewport : public Framebuffer {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Construct this @ref Viewport instance.
		Viewport(Viewport&&) ETNoexceptHint;
		//!	Disable copy construction.
		Viewport(const Viewport&) = delete;
		//!	Construct this @ref Viewport instance.
		Viewport();

		~Viewport() = default;

		// ---------------------------------------------------

	public:
		VkResult Resize(Gpu& gpu, VkExtent2D dimensions, uint32 layers, ExternalImageMap externalImages = ExternalImageMap {});

		// ---------------------------------------------------

	public:
		bool TryAcquire(const GraphicsPipelineBuilder& generator) ETNoexceptHint;

		void Release() ETNoexceptHint;

		// ---------------------------------------------------

	private:
		Atomic<const GraphicsPipelineBuilder*> _generator;

		// ---------------------------------------------------

		friend void Swap(Viewport&, Viewport&) ETNoexceptHint;
	};

	// ---

	struct ViewportDisposer {
		ETPureFunctionHint void operator()(Viewport* viewport) const ETNoexceptHint;
	};

}}} // namespace Eldritch2::Graphics::Vulkan

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/Vulkan/GpuResourceApi.inl>
//------------------------------------------------------------------//
