/*==================================================================*\
  GpuResources.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <vk_mem_alloc.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {
	class GraphicsPipeline;
	class Gpu;
}}} // namespace Eldritch2::Graphics::Vulkan

namespace Eldritch2 { namespace Graphics { namespace Vulkan {
	namespace Detail {

		class ETPureAbstractHint AbstractBuffer {
			// - CONSTRUCTOR/DESTRUCTOR --------------------------

		protected:
			//!	Disable copy construction.
			AbstractBuffer(const AbstractBuffer&) = delete;
			//!	Disable move construction; clients should implement themselves via Swap().
			AbstractBuffer(AbstractBuffer&&) = delete;
			//!	Constructs this @ref AbstractBuffer instance.
			AbstractBuffer();

			~AbstractBuffer();

			// ---------------------------------------------------

		public:
			VkBuffer Get() ETNoexceptHint;

			operator VkBuffer() ETNoexceptHint;

			// ---------------------------------------------------

		public:
			void FreeResources(Gpu& gpu);

			// ---------------------------------------------------

		protected:
			VkResult BindResources(Gpu& gpu, const VkBufferCreateInfo& bufferInfo, const VmaAllocationCreateInfo& allocationInfo);

			void GetAllocationInfo(Gpu& gpu, VmaAllocationInfo& info);

			VkResult MapHostPointer(Gpu& gpu, void*& outBase) const;

			void UnmapHostPointer(Gpu& gpu) const;

			// ---------------------------------------------------

			//!	Disable copy assignment.
			AbstractBuffer& operator=(const AbstractBuffer&) = delete;

			// - DATA MEMBERS ------------------------------------

		private:
			VmaAllocation _backing;
			VkBuffer      _buffer;

			// ---------------------------------------------------

			friend void Swap(AbstractBuffer&, AbstractBuffer&);
		};

		// ---

		class ETPureAbstractHint AbstractImage {
			// - CONSTRUCTOR/DESTRUCTOR --------------------------

		protected:
			//!	Disable copy construction.
			AbstractImage(const AbstractImage&) = delete;
			//!	Disable move construction; clients should implement themselves via Swap().
			AbstractImage(AbstractImage&&) = delete;
			//!	Constructs this @ref AbstractImage instance.
			AbstractImage();

			~AbstractImage();

			// ---------------------------------------------------

		public:
			VkImage Get() ETNoexceptHint;

			operator VkImage() ETNoexceptHint;

			// ---------------------------------------------------

		public:
			void FreeResources(Gpu& gpu);

			// ---------------------------------------------------

		protected:
			VkResult BindResources(Gpu& gpu, const VkImageCreateInfo& imageInfo, const VmaAllocationCreateInfo& allocationInfo);

			// ---------------------------------------------------

			//!	Disable copy assignment.
			AbstractImage& operator=(const AbstractImage&) = delete;

			// - DATA MEMBERS ------------------------------------

		private:
			VmaAllocation _backing;
			VkImage       _image;

			// ---------------------------------------------------

			friend void Swap(AbstractImage&, AbstractImage&);
		};

	} // namespace Detail

	class VertexBuffer : public Detail::AbstractBuffer {
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

	class IndexBuffer : public Detail::AbstractBuffer {
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

	class TransferBuffer : public Detail::AbstractBuffer {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		TransferBuffer(const TransferBuffer&) = delete;
		//!	Constructs this @ref TransferBuffer instance.
		TransferBuffer(TransferBuffer&&);
		//!	Constructs this @ref TransferBuffer instance.
		TransferBuffer() = default;

		~TransferBuffer() = default;

		// ---------------------------------------------------

	public:
		using Detail::AbstractBuffer::MapHostPointer;
		using Detail::AbstractBuffer::UnmapHostPointer;

		// ---------------------------------------------------

	public:
		VkResult BindResources(Gpu& gpu, VkDeviceSize sizeInBytes);

		// ---------------------------------------------------

		//!	Disable copy assignment.
		TransferBuffer& operator=(const TransferBuffer&) = delete;

		// ---------------------------------------------------

		friend void Swap(TransferBuffer&, TransferBuffer&);
	};

	// ---

	class UniformBuffer : public Detail::AbstractBuffer {
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
		using Detail::AbstractBuffer::MapHostPointer;
		using Detail::AbstractBuffer::UnmapHostPointer;

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

	class Mesh {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		Mesh(const Mesh&) = delete;
		//!	Constructs this @ref Mesh instance.
		Mesh(Mesh&&);
		//!	Constructs this @ref Mesh instance.
		Mesh() = default;

		~Mesh() = default;

		// ---------------------------------------------------

	public:
		const VertexBuffer& GetVertices() const;
		VertexBuffer&       GetVertices();

		const IndexBuffer& GetIndices() const;
		IndexBuffer&       GetIndices();

		// ---------------------------------------------------

	public:
		VkResult BindResources(Gpu& gpu, uint32_t vertexCount, uint32_t indexCount);

		void FreeResources(Gpu& gpu);

		// ---------------------------------------------------

		//!	Disable copy assignment.
		Mesh& operator=(const Mesh&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		VertexBuffer _vertices;
		IndexBuffer  _indices;

		// ---------------------------------------------------

		friend void Swap(Mesh&, Mesh&);
	};

	// ---

	class SparseTileCache : public Detail::AbstractBuffer {
		// - TYPE PUBLISHING ---------------------------------

	public:
		enum : uint32_t {
			PageCoordinateBits = 18u,
			MaxImageDimension  = 1u << PageCoordinateBits,
			PageMipBits        = 10u
		};

		// ---

	public:
		struct Tile {
			uint64_t x : PageCoordinateBits;
			uint64_t y : PageCoordinateBits;
			uint64_t z : PageCoordinateBits;
			uint64_t mip : PageMipBits;
		};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		SparseTileCache(const SparseTileCache&) = delete;
		//!	Constructs this @ref SparseTileCache instance.
		SparseTileCache(SparseTileCache&&);
		//!	Constructs this @ref SparseTileCache instance.
		SparseTileCache();

		~SparseTileCache() = default;

		// ---------------------------------------------------

	public:
		using Detail::AbstractBuffer::MapHostPointer;
		using Detail::AbstractBuffer::UnmapHostPointer;

		// ---------------------------------------------------

	public:
		bool ShouldCacheTile(Tile tile, VkDeviceSize& reservedOffset);

		void NotifyCached(Tile tile, VkDeviceSize offset);

		// ---------------------------------------------------

	public:
		bool IsCached(Tile tile, VkDeviceSize& offset) const ETNoexceptHint;

		bool IsLoading(Tile tile) const ETNoexceptHint;

		// ---------------------------------------------------

	public:
		VkResult BindResources(Gpu& gpu, VkDeviceSize tileSizeInBytes, uint32_t cachedTileLimit);

		void FreeResources(Gpu& gpu);

		// ---------------------------------------------------

		//!	 Disable copy assignment.
		SparseTileCache& operator=(const SparseTileCache&) = delete;

		// ---------------------------------------------------

	private:
		HashMap<Tile, VkDeviceSize> _cachedPagesByTile;
		HashSet<Tile>               _loadingTiles;

		// ---------------------------------------------------

		friend void Swap(SparseTileCache&, SparseTileCache&);
	};

	// ---

	class ShaderImage : public Detail::AbstractImage {
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
		VkResult BindResources(Gpu& gpu, VkFormat format, VkExtent3D extent, uint32_t mips, uint32_t arrayLayers = 1u);

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
		using Tile = SparseTileCache::Tile;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref TileManager instance.
		TileManager(VkFormat format, VkExtent3D tileExtent, VkExtent3D imageExtent);
		//!	Disable copy construction.
		TileManager(const TileManager&) = delete;
		//!	Constructs this @ref TileManager instance.
		TileManager(TileManager&&);
		//!	Constructs this @ref TileManager instance.
		TileManager();

		~TileManager() = default;

		// ---------------------------------------------------

	public:
		VkDeviceSize GetTileSize() const ETNoexceptHint;

		VkOffset3D GetTexel(Tile tile) const ETNoexceptHint;

		Tile GetTile(VkOffset3D texel, uint32_t mip = 0) const ETNoexceptHint;

		// ---------------------------------------------------

	public:
		VkExtent3D GetImageExtentInTexels() const ETNoexceptHint;

		VkExtent3D GetTileExtentInTexels() const ETNoexceptHint;

		// ---------------------------------------------------

	public:
		bool TouchTile(Tile tile) ETNoexceptHint;

		Tile EvictTile(Tile tile) ETNoexceptHint;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		TileManager& operator=(const TileManager&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		HashSet<Tile> _residentTiles;
		VkDeviceSize  _tileSize;
		VkExtent3D    _logTileExtent;
		VkExtent3D    _imageExtent;

		// ---------------------------------------------------

		friend void Swap(TileManager&, TileManager&);
	};

	// ---

	class SparseShaderImage : public Detail::AbstractImage {
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
		const TileManager& GetTileManager() const;

		// ---------------------------------------------------

	public:
		CacheResult MakeResident(const SparseTileCache& cache, VkOffset3D texel, uint32_t mip);
		CacheResult MakeResident(const SparseTileCache& cache, TileManager::Tile tile);

		// ---------------------------------------------------

	public:
		VkResult BindResources(Gpu& gpu, VkFormat format, VkExtent3D tileExtent, VkExtent3D extent, uint32_t mips);

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

}}} // namespace Eldritch2::Graphics::Vulkan

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/Vulkan/GpuResources.inl>
//------------------------------------------------------------------//
