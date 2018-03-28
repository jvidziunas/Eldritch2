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

namespace Eldritch2 {
	namespace Graphics {
		namespace Vulkan {
			class	GpuHeap;
		}
	}
}

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {
namespace Detail {

	class ETPureAbstractHint AbstractBuffer {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
	//!	Disable copy construction.
		AbstractBuffer( const AbstractBuffer& ) = delete;
	//!	Constructs this @ref AbstractBuffer instance.
		AbstractBuffer( AbstractBuffer&& );
	//!	Constructs this @ref AbstractBuffer instance.
		AbstractBuffer();

		~AbstractBuffer();

	// ---------------------------------------------------

	public:
		VkBuffer	Get() ETNoexceptHint;

		operator	VkBuffer() ETNoexceptHint;

	// ---------------------------------------------------

	public:
		void	FreeResources( GpuHeap& heap );

	// ---------------------------------------------------

	protected:
		VkResult	BindResources( GpuHeap& heap, const VkBufferCreateInfo& bufferInfo, const VmaAllocationCreateInfo& allocationInfo );

		void		GetAllocationInfo( GpuHeap& heap, VmaAllocationInfo& info );

	// ---------------------------------------------------

	//!	Disable copy assignment.
		AbstractBuffer&	operator=( const AbstractBuffer& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		VmaAllocation	_backing;
		VkBuffer		_buffer;

	// ---------------------------------------------------

		friend void	Swap( AbstractBuffer&, AbstractBuffer& );
	};

// ---

	class ETPureAbstractHint AbstractImage {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
	//!	Disable copy construction.
		AbstractImage( const AbstractImage& ) = delete;
	//!	Constructs this @ref AbstractImage instance.
		AbstractImage( AbstractImage&& );
	//!	Constructs this @ref AbstractImage instance.
		AbstractImage();

		~AbstractImage();

	// ---------------------------------------------------

	public:
		VkImage	Get() ETNoexceptHint;

		operator VkImage() ETNoexceptHint;

	// ---------------------------------------------------

	public:
		void	FreeResources( GpuHeap& heap );

	// ---------------------------------------------------

	protected:
		VkResult	BindResources( GpuHeap& heap, const VkImageCreateInfo& imageInfo, const VmaAllocationCreateInfo& allocationInfo );

	// ---------------------------------------------------

	//!	Disable copy assignment.
		AbstractImage&	operator=( const AbstractImage& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		VmaAllocation	_backing;
		VkImage			_image;

	// ---------------------------------------------------

		friend void	Swap( AbstractImage&, AbstractImage& );
	};

}	// namespace Detail

	class VertexBuffer : public Detail::AbstractBuffer {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Disable copy construction.
		VertexBuffer( const VertexBuffer& ) = delete;
	//!	Constructs this @ref VertexBuffer instance.
		VertexBuffer( VertexBuffer&& ) = default;
	//!	Constructs this @ref VertexBuffer instance.
		VertexBuffer() = default;

		~VertexBuffer() = default;

	// ---------------------------------------------------

	public:
		VkResult	BindResources( GpuHeap& heap, VkDeviceSize sizeInBytes );

	// ---------------------------------------------------

	//!	Disable copy assignment.
		VertexBuffer&	operator=( const VertexBuffer& ) = delete;

	// ---------------------------------------------------

		friend void	Swap( VertexBuffer&, VertexBuffer& );
	};

// ---

	class IndexBuffer : public Detail::AbstractBuffer {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Disable copy construction.
		IndexBuffer( const IndexBuffer& ) = delete;
	//!	Constructs this @ref IndexBuffer instance.
		IndexBuffer( IndexBuffer&& ) = default;
	//!	Constructs this @ref IndexBuffer instance.
		IndexBuffer() = default;

		~IndexBuffer() = default;

	// ---------------------------------------------------

	public:
		VkResult	BindResources( GpuHeap& heap, VkDeviceSize sizeInBytes );

	// ---------------------------------------------------

	//!	Disable copy assignment.
		IndexBuffer&	operator=( const IndexBuffer& ) = delete;

	// ---------------------------------------------------

		friend void	Swap( IndexBuffer&, IndexBuffer& );
	};

// ---

	class TransferBuffer : public Detail::AbstractBuffer {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Disable copy construction.
		TransferBuffer( const TransferBuffer& ) = delete;
	//!	Constructs this @ref TransferBuffer instance.
		TransferBuffer( TransferBuffer&& ) = default;
	//!	Constructs this @ref TransferBuffer instance.
		TransferBuffer() = default;

		~TransferBuffer() = default;

	// ---------------------------------------------------

	public:
		VkResult	BindResources( GpuHeap& heap, VkDeviceSize sizeInBytes );

		void*		GetHostPointer( GpuHeap& heap );

	// ---------------------------------------------------

	//!	Disable copy assignment.
		TransferBuffer&	operator=( const TransferBuffer& ) = delete;

	// ---------------------------------------------------

		friend void	Swap( TransferBuffer&, TransferBuffer& );
	};

// ---

	class UniformBuffer : public Detail::AbstractBuffer {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Disable copy construction.
		UniformBuffer( const UniformBuffer& ) = delete;
	//!	Constructs this @ref UniformBuffer instance.
		UniformBuffer( UniformBuffer&& ) = default;
	//!	Constructs this @ref UniformBuffer instance.
		UniformBuffer() = default;

		~UniformBuffer() = default;

	// ---------------------------------------------------

	public:
		VkResult	BindResources( GpuHeap& heap, VkDeviceSize sizeInBytes );

		void*		GetHostPointer( GpuHeap& heap );

	// ---------------------------------------------------

	//!	Disable copy assignment.
		UniformBuffer&	operator=( const UniformBuffer& ) = delete;

	// ---------------------------------------------------

		friend void	Swap( UniformBuffer&, UniformBuffer& );
	};

// ---

	class SparsePageManager {
	// - TYPE PUBLISHING ---------------------------------

	public:
		enum : uint64_t { InvalidTile = static_cast<uint64_t>(0u) };
		enum : uint32_t {
			PageCoordinateBits	= 18u,
			PageMipBits			= 10u,
			MaxImageDimension	= 1u << PageCoordinateBits
		};

	// ---

	public:
		struct Tile {
			uint32_t x   : PageCoordinateBits;
			uint32_t y   : PageCoordinateBits;
			uint32_t z   : PageCoordinateBits;
			uint32_t mip : PageMipBits;
		};

	// ---

	public:
		template <typename Value>
		using TileMap		= HashMap<Tile, Value>;
		using CachedTile	= Pair<volatile char*, bool>;
		using PhysicalTile	= VkDeviceSize;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref SparsePageManager instance.
		SparsePageManager( VkExtent3D tileExtent, VkExtent3D extent );
	//!	Constructs this @ref SparsePageManager instance.
		SparsePageManager( const SparsePageManager& ) = delete;
	//!	Constructs this @ref SparsePageManager instance.
		SparsePageManager( SparsePageManager&& );
	//!	Constructs this @ref SparsePageManager instance.
		SparsePageManager();

		~SparsePageManager() = default;

	// ---------------------------------------------------

	public:
		VkOffset3D	GetTexel( Tile tile ) const ETNoexceptHint;

		Tile		GetTile( VkOffset3D texel, uint32_t mip = 0 ) const ETNoexceptHint;

	// ---------------------------------------------------

	public:
		VkExtent3D	GetImageExtentInTexels() const ETNoexceptHint;

		VkExtent3D	GetTileExtentInTexels() const ETNoexceptHint;

	// ---------------------------------------------------

	public:
		bool	MakeResident( SparsePageManager::Tile tile );

	// ---------------------------------------------------

	public:
		SparsePageManager&	operator=( const SparsePageManager& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		VkExtent3D				_logTileExtent;
		VkExtent3D				_extent;
	//!	Collection of tiles resident on the GPU.
		TileMap<PhysicalTile>	_residentTilesByCoordinate;
	//!	Collection of tiles resident on the host.
		TileMap<CachedTile>		_cachedTilesByCoordinate;

	// ---------------------------------------------------

		friend void	Swap( SparsePageManager&, SparsePageManager& );
	};

// ---

	class ShaderImage : public Detail::AbstractImage {
	// - TYPE PUBLISHING ---------------------------------

	public:
		using ArraySlice	= uint16;
		using MipIndex		= uint16;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Disable copy construction.
		ShaderImage( const ShaderImage& ) = delete;
	//!	Constructs this @ref ShaderImage instance.
		ShaderImage( ShaderImage&& ) = default;
	//!	Constructs this @ref ShaderImage instance.
		ShaderImage() = default;

		~ShaderImage() = default;

	// ---------------------------------------------------

	public:
		VkResult	BindResources( GpuHeap& heap, VkExtent3D extent, uint32_t mips, uint32_t arrayLayers = 1u );

	// ---------------------------------------------------

	//!	Disable copy assignment.
		ShaderImage&	operator=( const ShaderImage& ) = delete;

	// ---------------------------------------------------

		friend void	Swap( ShaderImage&, ShaderImage& );
	};

// ---

	class SparseShaderImage : public Detail::AbstractImage {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Disable copy construction.
		SparseShaderImage( const SparseShaderImage& ) = delete;
	//!	Constructs this @ref SparseShaderImage instance.
		SparseShaderImage( SparseShaderImage&& );
	//!	Constructs this @ref SparseShaderImage instance.
		SparseShaderImage();

		~SparseShaderImage() = default;

	// ---------------------------------------------------

	public:
		VkResult	BindResources( GpuHeap& heap, VkExtent3D tileExtent, VkExtent3D extent, uint32_t mips );

		void		FreeResources( GpuHeap& heap );

	// ---------------------------------------------------

	public:
		bool	MakeResident( SparsePageManager::Tile tile );

	// ---------------------------------------------------

	//!	Disable copy assignment.
		SparseShaderImage&	operator=( const SparseShaderImage& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
	//!	Page manager instance providing the device resources used by this @ref SparseShaderImage.
		SparsePageManager	_pageManager;
	/*!	Staging buffer used as a target for tile decompression. The size of this buffer should be much larger
		than can be transferred in a single frame, as it also serves as a cache for decompressed tiles
		not necessarily resident on the GPU. Contents are write-only on the host, and read-only on the GPU. */
		TransferBuffer		_cache;

	// ---------------------------------------------------

		friend void	Swap( SparseShaderImage&, SparseShaderImage& );
	};

}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/Vulkan/GpuResources.inl>
//------------------------------------------------------------------//