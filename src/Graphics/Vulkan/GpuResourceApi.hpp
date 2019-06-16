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
#include <Graphics/PipelineBuilder.hpp>
//------------------------------------------------------------------//
#include <vk_mem_alloc.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics {
	namespace Vulkan {
		class Gpu;
	} // namespace Vulkan

	class PipelineBuilder;
	struct ImageDescriptor;
	struct MeshDescriptor;
}} // namespace Eldritch2::Graphics

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	class ETPureAbstractHint GpuBuffer {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
		//!	Disable copy construction.
		GpuBuffer(const GpuBuffer&) = delete;
		//!	Disable move construction.
		GpuBuffer(GpuBuffer&&) = delete;
		//!	Constructs this @ref GpuBuffer instance.
		GpuBuffer() ETNoexceptHint;

		~GpuBuffer();

		// ---------------------------------------------------

	public:
		ETConstexpr operator VkBuffer() const ETNoexceptHint;

		ETConstexpr VkBuffer Get() const ETNoexceptHint;

		ETConstexpr bool IsBound() const ETNoexceptHint;

		// ---------------------------------------------------

	public:
		void FreeResources(Gpu& gpu) ETNoexceptHint;

		// ---------------------------------------------------

	protected:
		VkResult BindResources(Gpu& gpu, const VkBufferCreateInfo& bufferInfo, const VmaAllocationCreateInfo& allocationInfo);
		VkResult BindResources(Gpu& gpu, VkBuffer buffer, VmaAllocation backing) ETNoexceptHint;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		GpuBuffer& operator=(const GpuBuffer&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		VmaAllocation _backing;
		VkBuffer      _buffer;

		// ---------------------------------------------------

		friend void Swap(GpuBuffer&, GpuBuffer&) ETNoexceptHint;
	};

	// ---

	class ETPureAbstractHint GpuImage {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
		//!	Disable copy construction.
		GpuImage(const GpuImage&) = delete;
		//!	Constructs this @ref GpuImage instance.
		ETConstexpr GpuImage() ETNoexceptHint;
		//!	Disable move construction.
		GpuImage(GpuImage&&) = delete;

		~GpuImage();

		// ---------------------------------------------------

	public:
		ETConstexpr operator VkImage() const ETNoexceptHint;

		ETConstexpr VkImage Get() const ETNoexceptHint;

		ETConstexpr bool IsBound() const ETNoexceptHint;

		// ---------------------------------------------------

	public:
		void FreeResources(Gpu& gpu) ETNoexceptHint;

		// ---------------------------------------------------

	protected:
		VkResult BindResources(Gpu& gpu, const VkImageCreateInfo& imageInfo, const VmaAllocationCreateInfo& allocationInfo);
		VkResult BindResources(Gpu& gpu, VkImage image, VmaAllocation backing) ETNoexceptHint;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		GpuImage& operator=(const GpuImage&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		VmaAllocation _backing;
		VkImage       _image;

		// ---------------------------------------------------

		friend void Swap(GpuImage&, GpuImage&) ETNoexceptHint;
	};

	// ---

	class AttributeBuffer : public GpuBuffer {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		AttributeBuffer(const AttributeBuffer&) = delete;
		//!	Constructs this @ref VertexBuffer instance.
		AttributeBuffer(AttributeBuffer&&) ETNoexceptHint;
		//!	Constructs this @ref VertexBuffer instance.
		AttributeBuffer() ETNoexceptHint = default;

		~AttributeBuffer() = default;

		// ---------------------------------------------------

	public:
		VkResult BindResources(Gpu& gpu, VkDeviceSize byteSize);

		// ---------------------------------------------------

		//!	Disable copy assignment.
		AttributeBuffer& operator=(const AttributeBuffer&) = delete;

		// ---------------------------------------------------

		friend void Swap(AttributeBuffer&, AttributeBuffer&) ETNoexceptHint;
	};

	// ---

	class IndexBuffer : public GpuBuffer {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		IndexBuffer(const IndexBuffer&) = delete;
		//!	Constructs this @ref IndexBuffer instance.
		IndexBuffer(IndexBuffer&&) ETNoexceptHint;
		//!	Constructs this @ref IndexBuffer instance.
		IndexBuffer() ETNoexceptHint = default;

		~IndexBuffer() = default;

		// ---------------------------------------------------

	public:
		VkResult BindResources(Gpu& gpu, VkDeviceSize byteSize);

		// ---------------------------------------------------

		//!	Disable copy assignment.
		IndexBuffer& operator=(const IndexBuffer&) = delete;

		// ---------------------------------------------------

		friend void Swap(IndexBuffer&, IndexBuffer&) ETNoexceptHint;
	};

	// ---

	class UploadBuffer : public GpuBuffer {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		UploadBuffer(const UploadBuffer&) = delete;
		//!	Constructs this @ref UploadBuffer instance.
		UploadBuffer(UploadBuffer&&) ETNoexceptHint;
		//!	Constructs this @ref UploadBuffer instance.
		UploadBuffer() ETNoexceptHint = default;

		~UploadBuffer() = default;

		// ---------------------------------------------------

	public:
		VkResult BindResources(Gpu& gpu, VkDeviceSize byteSize);

		// ---------------------------------------------------

		//!	Disable copy assignment.
		UploadBuffer& operator=(const UploadBuffer&) = delete;

		// ---------------------------------------------------

		friend void Swap(UploadBuffer&, UploadBuffer&) ETNoexceptHint;
	};

	// ---

	class UniformBuffer : public GpuBuffer {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		UniformBuffer(const UniformBuffer&) = delete;
		//!	Constructs this @ref UniformBuffer instance.
		UniformBuffer(UniformBuffer&&) ETNoexceptHint;
		//!	Constructs this @ref UniformBuffer instance.
		UniformBuffer() ETNoexceptHint = default;

		~UniformBuffer() = default;

		// ---------------------------------------------------

	public:
		VkResult BindResources(Gpu& gpu, VkDeviceSize byteSize);

		// ---------------------------------------------------

		//!	Disable copy assignment.
		UniformBuffer& operator=(const UniformBuffer&) = delete;

		// ---------------------------------------------------

		friend void Swap(UniformBuffer&, UniformBuffer&) ETNoexceptHint;
	};

	// ---

	class GeometryCache {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		GeometryCache(const GeometryCache&) = delete;
		//! Constructs this @ref GeometryCache instance.
		GeometryCache() ETNoexceptHint;

		~GeometryCache() = default;

		// ---------------------------------------------------

	public:
		VkResult AllocateVertices(VkDeviceSize& outOffset, VkDeviceSize byteSize);
		VkResult AllocateIndices(VkDeviceSize& outOffset, VkDeviceSize byteSize);

		void DeallocateVertices(VkDeviceSize offset, VkDeviceSize byteSize);
		void DeallocateIndices(VkDeviceSize offset, VkDeviceSize byteSize);

		// ---------------------------------------------------

	public:
		ETConstexpr VkBuffer GetVertexBuffer() const ETNoexceptHint;

		ETConstexpr VkBuffer GetIndexBuffer() const ETNoexceptHint;

		// ---------------------------------------------------

	public:
		VkResult BindResources(Gpu& gpu, VkDeviceSize vertexCacheByteSize, VkDeviceSize indexCacheByteSize);

		void FreeResources(Gpu& gpu) ETNoexceptHint;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		GeometryCache& operator=(const GeometryCache&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		AttributeBuffer              _vertices;
		IdentifierPool<VkDeviceSize> _vertexRanges;
		IndexBuffer                  _indices;
		IdentifierPool<VkDeviceSize> _indexRanges;

		// ---------------------------------------------------

		friend void Swap(GeometryCache&, GeometryCache&) ETNoexceptHint;
	};

	// ---

	class Mesh {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		Mesh(const Mesh&) = delete;
		//!	Constructs this @ref Mesh instance.
		Mesh(Mesh&&) ETNoexceptHint;
		//!	Constructs this @ref Mesh instance.
		Mesh() ETNoexceptHint;

		~Mesh() = default;

		// ---------------------------------------------------

	public:
		ETConstexpr VkBuffer GetVertices() const ETNoexceptHint;

		ETConstexpr VkBuffer GetIndices() const ETNoexceptHint;

		ETConstexpr VkDeviceSize GetVertexByteOffset() const ETNoexceptHint;

		ETConstexpr VkDeviceSize GetIndexByteOffset() const ETNoexceptHint;

		ETConstexpr VkDeviceSize GetVerticesByteSize() const ETNoexceptHint;

		ETConstexpr VkDeviceSize GetIndicesByteSize() const ETNoexceptHint;

		// ---------------------------------------------------

	public:
		VkResult BindResources(GeometryCache& cache, const MeshDescriptor& description);

		void FreeResources(GeometryCache& cache);

		// ---------------------------------------------------

		//!	Disable copy assignment.
		Mesh& operator=(const Mesh&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		VkBuffer     _vertices;
		VkBuffer     _indices;
		VkDeviceSize _indicesOffset;
		VkDeviceSize _verticesOffset;
		VkDeviceSize _indicesSize;
		VkDeviceSize _verticesSize;

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
		ShaderImage(ShaderImage&&) ETNoexceptHint;
		//!	Constructs this @ref ShaderImage instance.
		ShaderImage() ETNoexceptHint = default;

		~ShaderImage() = default;

		// ---------------------------------------------------

	public:
		VkResult BindResources(Gpu& gpu, const ImageDescriptor& description);

		// ---------------------------------------------------

	private:
		//!	Disable copy assignment.
		ShaderImage& operator=(const ShaderImage&) = delete;

		// ---------------------------------------------------

		friend void Swap(ShaderImage&, ShaderImage&) ETNoexceptHint;
	};

	// ---

	class TileManager : public Cache<ImageTile, VkDeviceSize> {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		TileManager(const TileManager&) = delete;
		//!	Constructs this @ref TileManager instance.
		TileManager(TileManager&&) ETNoexceptHint;
		//!	Constructs this @ref TileManager instance.
		TileManager() ETNoexceptHint;

		~TileManager() = default;

		// ---------------------------------------------------

	public:
		ETConstexpr VkExtent3D GetTileExtent() const ETNoexceptHint;

		// ---------------------------------------------------

	public:
		VkResult BindResources(VkExtent3D imageExtent, VkExtent3D tileExtent);

		void FreeResources() ETNoexceptHint;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		TileManager& operator=(const TileManager&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		VkExtent3D _tileExtent;

		// ---------------------------------------------------

		friend void Swap(TileManager&, TileManager&) ETNoexceptHint;
	};

	// ---

	class HostSparseTileCache : public GpuBuffer {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		HostSparseTileCache(const HostSparseTileCache&) = delete;
		//!	Constructs this @ref HostSparseTileCache instance.
		HostSparseTileCache(HostSparseTileCache&&) ETNoexceptHint;
		//!	Constructs this @ref HostSparseTileCache instance.
		HostSparseTileCache() ETNoexceptHint;

		~HostSparseTileCache() = default;

		// ---------------------------------------------------

	public:
		bool IsLoading(ImageTile tile) const ETNoexceptHint;

		bool ShouldDecode(VkDeviceSize& outOffset, ImageTile tile) const ETNoexceptHint;

		void NotifyCached(VkDeviceSize offset, ImageTile tile);

		// ---------------------------------------------------

	public:
		VkResult BindResources(Gpu& gpu, const TileManager& client, uint32 cachedTiles);

		void FreeResources(Gpu& gpu);

		// ---------------------------------------------------

		//! Disable copy assignment.
		HostSparseTileCache& operator=(const HostSparseTileCache&) = delete;

		// ---------------------------------------------------

	private:
		HashSet<ImageTile> _loadingTiles;

		// ---------------------------------------------------

		friend void Swap(HostSparseTileCache&, HostSparseTileCache&) ETNoexceptHint;
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
		SparseShaderImage(SparseShaderImage&&) ETNoexceptHint;
		//!	Constructs this @ref SparseShaderImage instance.
		SparseShaderImage() ETNoexceptHint = default;

		~SparseShaderImage() = default;

		// ---------------------------------------------------

	public:
		ETConstexpr const TileManager& GetTileManager() const ETNoexceptHint;

		// ---------------------------------------------------

	public:
		CacheResult MakeResident(const HostSparseTileCache& cache, ImageTile tile);

		// ---------------------------------------------------

	public:
		VkResult BindResources(Gpu& gpu, const ImageDescriptor& description);

		void FreeResources(Gpu& gpu);

		// ---------------------------------------------------

		//!	Disable copy assignment.
		SparseShaderImage& operator=(const SparseShaderImage&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		TileManager _tileManager;

		// ---------------------------------------------------

		friend void Swap(SparseShaderImage&, SparseShaderImage&) ETNoexceptHint;
	};

	// ---

	class DescriptorTable {
		// - TYPE PUBLISHING ---------------------------------

	public:
		enum : size_t {
			SourceImages,
			ImageViews
		};

		// ---

		using ImageList     = SoaList<const GpuImage* /*sourceImages*/, VkImageView /*imageViews*/>;
		using ResourceIndex = ImageList::SizeType;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		DescriptorTable(const DescriptorTable&) = delete;
		//!	Constructs this @ref DescriptorTable instance.
		DescriptorTable(DescriptorTable&&) ETNoexceptHint;
		//!	Constructs this @ref DescriptorTable instance.
		DescriptorTable() ETNoexceptHint;

		~DescriptorTable();

		// ---------------------------------------------------

	public:
		ETConstexpr VkDescriptorSet GetDescriptorSet() const ETNoexceptHint;

		// ---------------------------------------------------

	public:
		template <typename InputIterator>
		ResourceIndex ReserveSlots(InputIterator begin, InputIterator end);

		// ---------------------------------------------------

	public:
		VkResult BindResourceViews(Gpu& gpu, ResourceIndex begin, ResourceIndex end);

		VkResult PushToGpu(Gpu& gpu, ResourceIndex begin, ResourceIndex end);

		// ---------------------------------------------------

	public:
		VkResult BindResources(Gpu& gpu, VkDescriptorSetLayout layout, InitializerList<VkDescriptorPoolSize> pools);

		void FreeResources(Gpu& gpu) ETNoexceptHint;

		// ---------------------------------------------------

	public:
		//!	Disable copy assignment.
		DescriptorTable& operator=(const DescriptorTable&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		ResourceIndex    _lastCurrent;
		ImageList        _images;
		VkDescriptorPool _pool;
		VkDescriptorSet  _descriptors;

		// ---------------------------------------------------

		friend void Swap(DescriptorTable&, DescriptorTable&) ETNoexceptHint;
	};

	// ---

	class Framebuffer {
		// - TYPE PUBLISHING ---------------------------------

	public:
		class Image : public GpuImage {
			// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Disable copy construction.
			Image(const Image&) = delete;
			//! Constructs this @ref Image instance.
			Image(Image&&) ETNoexceptHint;
			//! Constructs this @ref Image instance.
			Image() ETNoexceptHint = default;

			~Image() = default;

			// ---------------------------------------------------

		public:
			VkResult BindResources(Gpu& gpu, const PipelineAttachment& description, VkExtent2D dimensions, uint32 slices);
			VkResult BindResources(Gpu& gpu, VkImage externalImage) ETNoexceptHint;

			// ---------------------------------------------------

			//!	Disable copy assignment.
			Image& operator=(const Image&) = delete;

			// ---------------------------------------------------

			friend void Swap(Image&, Image&) ETNoexceptHint;
		};

		// ---

	public:
		class StageImageSet {
			// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Disable copy construction.
			StageImageSet(const StageImageSet&) = delete;
			//!	Constructs this @ref ImageSet instance.
			StageImageSet(StageImageSet&&) ETNoexceptHint;
			//!	Constructs this @ref ImageSet instance.
			ETConstexpr StageImageSet() ETNoexceptHint;

			~StageImageSet();

			// ---------------------------------------------------

		public:
			VkResult BindResources(Gpu& gpu, const StageBuilder& stage, const PipelineAttachment attachments[], const Image images[]);

			void FreeResources(Gpu& gpu) ETNoexceptHint;

			// ---------------------------------------------------

			//!	Disable copy assignment.
			StageImageSet& operator=(const StageImageSet&) = delete;

			// - DATA MEMBERS ------------------------------------

		public:
			VkImageView views[ETCountOf(StageBuilder::colorAttachments) + ETCountOf(StageBuilder::inputAttachments) + 1];

			// ---------------------------------------------------

			friend void Swap(StageImageSet&, StageImageSet&) ETNoexceptHint;
		};

		// ---

	public:
		class StageFramebuffer {
			// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Disable copy construction.
			StageFramebuffer(const StageFramebuffer&) = delete;
			//!	Constructs this @ref StageFramebuffer instance.
			StageFramebuffer(StageFramebuffer&&) ETNoexceptHint;
			//!	Constructs this @ref StageFramebuffer instance.
			StageFramebuffer() ETNoexceptHint;

			~StageFramebuffer();

			// ---------------------------------------------------

		public:
			ETConstexpr VkExtent2D GetRenderExtent() const ETNoexceptHint;

			// ---------------------------------------------------

		public:
			VkResult BindResources(Gpu& gpu, const StageBuilder& pass, VkExtent2D dimensions, uint32 slices, const StageImageSet& images);

			void FreeResources(Gpu& gpu) ETNoexceptHint;

			// ---------------------------------------------------

			//!	Disable copy assignment.
			StageFramebuffer& operator=(const StageFramebuffer&) = delete;

			// - DATA MEMBERS ------------------------------------

		public:
			VkRenderPass     renderPass;
			VkFramebuffer    framebuffer;
			ResolutionScaler scaler;
			VkExtent2D       granularity;

			// ---------------------------------------------------

			friend void Swap(StageFramebuffer&, StageFramebuffer&) ETNoexceptHint;
		};

		// ---

	public:
		struct ExternalImage {
			uint32  attachmentIndex;
			VkImage image;
		};

		// ---

	public:
		using StageResourceList = SoaList<StageImageSet, StageFramebuffer>;
		using ImageList         = ArrayList<Image>;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		Framebuffer(const Framebuffer&) = delete;
		//!	Constructs this @ref Framebuffer instance.
		Framebuffer(Framebuffer&&) ETNoexceptHint;
		//!	Constructs this @ref Framebuffer instance.
		Framebuffer() ETNoexceptHint;

		~Framebuffer();

		// ---------------------------------------------------

	public:
		ETConstexpr const DescriptorTable& GetShaderResources() const ETNoexceptHint;

		Span<const StageFramebuffer*> GetStages() const ETNoexceptHint;

		ETConstexpr VkQueryPool GetTimingPool() const ETNoexceptHint;

		// ---------------------------------------------------

	public:
		VkResult UpdateScaling(Gpu& gpu) ETNoexceptHint;

		// ---------------------------------------------------

	public:
		VkResult BindResources(Gpu& gpu, const PipelineBuilder& pipeline, VkExtent2D dimensions, uint32 slices, InitializerList<ExternalImage> externalImages = {});

		void FreeResources(Gpu& gpu) ETNoexceptHint;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		Framebuffer& operator=(const Framebuffer&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		VkQueryPool       _timingPool;
		ImageList         _images;
		StageResourceList _stageResources;
		DescriptorTable   _shaderResources;

		// ---------------------------------------------------

		friend void Swap(Framebuffer&, Framebuffer&) ETNoexceptHint;
	};

}}} // namespace Eldritch2::Graphics::Vulkan

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/Vulkan/GpuResourceApi.inl>
//------------------------------------------------------------------//
