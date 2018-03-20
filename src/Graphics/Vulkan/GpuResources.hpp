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
			class	IoBuilder;
			class	GpuHeap;
		}

		class	GeometrySource;
		class	ImageSource;
	}
}

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {

	class SparsePageManager {
	// - TYPE PUBLISHING ---------------------------------

	public:
		enum : uint32 {
			PageCoordinateBits	= 18u,
			PageMipBits			= 10u,
			MaxImageDimension	= 1u << PageCoordinateBits
		};

	// ---

	public:
		enum : VkDeviceSize {
			InvalidTile = static_cast<VkDeviceSize>(0)
		};

	// ---

	public:
		using PhysicalTile = VkDeviceSize;

	// ---

	public:
		struct Tile {
			uint32_t	x	: PageCoordinateBits;
			uint32_t	y	: PageCoordinateBits;
			uint32_t	z	: PageCoordinateBits;
			uint32_t	mip : PageMipBits;
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Disable copy construction.
		SparsePageManager( const SparsePageManager& ) = delete;
	//!	Constructs this @ref SparsePageManager instance.
		SparsePageManager( SparsePageManager&& );
	//!	Constructs this @ref SparsePageManager instance.
		SparsePageManager();

		~SparsePageManager();

	// ---------------------------------------------------

	public:
		VkOffset3D	GetTexel( Tile tile ) const;

		Tile		GetTile( VkOffset3D texel, uint32_t mip = 0 ) const;

	// ---------------------------------------------------

	public:
		PhysicalTile	ReserveTile();

		void			Free( PhysicalTile tile );

	// ---------------------------------------------------

	public:
		VkExtent3D	GetImageExtentInTexels() const;

		VkExtent3D	GetTileExtentInTexels() const;

	// ---------------------------------------------------

	public:
		const ImageSource*	GetSource() const;

	// ---------------------------------------------------

	public:
		VkResult	BindResources( GpuHeap& heap, VkExtent3D tileExtent, const ImageSource& source );

		void		FreeResources( GpuHeap& heap );

	// ---------------------------------------------------

	//!	Disable copy assignment.
		SparsePageManager&	operator=( const SparsePageManager& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		const ImageSource*		_source;
		ArrayList<PhysicalTile>	_freeTiles;
		VkExtent3D				_logTileExtent;
		VkExtent3D				_imageExtent;

		VmaAllocation			_backing;
	//!	Texture providing the actual texel data that will be sampled during rendering.
		VkImage					_image;

	// ---------------------------------------------------

		friend void	Swap( SparsePageManager&, SparsePageManager& );
	};

// ---

	class TransferBuffer {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Disable copy construction.
		TransferBuffer( const TransferBuffer& ) = delete;
	//!	Constructs this @ref TransferBuffer instance.
		TransferBuffer( TransferBuffer&& );
	//!	Constructs this @ref TransferBuffer instance.
		TransferBuffer();

		~TransferBuffer();

	// ---------------------------------------------------

	public:
		VkResult	BindResources( GpuHeap& heap, VkDeviceSize sizeInBytes );

		void		FreeResources( GpuHeap& heap );

	// ---------------------------------------------------

	public:
		operator VkBuffer();

	// ---------------------------------------------------

	//!	Disable copy assignment.
		TransferBuffer&	operator=( const TransferBuffer& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		VmaAllocation	_backing;
		VkBuffer		_buffer;

	// ---------------------------------------------------

		friend void	Swap( TransferBuffer&, TransferBuffer& );
	};

// ---

	class UniformBuffer {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Disable copy construction.
		UniformBuffer( const UniformBuffer& ) = delete;
	//!	Constructs this @ref UniformBuffer instance.
		UniformBuffer( UniformBuffer&& );
	//!	Constructs this @ref UniformBuffer instance.
		UniformBuffer();

		~UniformBuffer();

	// ---------------------------------------------------

	public:
		VkResult	BindResources( GpuHeap& heap, VkDeviceSize sizeInBytes );

		void		FreeResources( GpuHeap& heap );

	// ---------------------------------------------------

	public:
		operator VkBuffer();

	// ---------------------------------------------------

	//!	Disable copy assignment.
		UniformBuffer&	operator=( const UniformBuffer& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		VmaAllocation	_backing;
		VkBuffer		_buffer;
		void*			_base;

	// ---------------------------------------------------

		friend void	Swap( UniformBuffer&, UniformBuffer& );
	};

// ---

	class ShaderImage {
	// - TYPE PUBLISHING ---------------------------------

	public:
		using ArraySlice	= uint16;
		using MipIndex		= uint16;

	// ---

ET_PUSH_COMPILER_WARNING_STATE()
/*	(4309) MSVC doesn't like the 32 bit -> 16 bit truncation here, but since we're just looking to set all the
 *	bits there's nothing to be afraid of. */
	ET_SET_MSVC_WARNING_STATE( disable : 4309 )
	public:
		enum : MipIndex {
			FirstMipInImage	= 0,
			LastMipInImage	= static_cast<MipIndex>(~0u),
		};

	// ---

	public:
		enum : ArraySlice {
			FirstSliceInImage	= 0,
			LastSliceInImage	= static_cast<ArraySlice>(~0u),
		};

ET_POP_COMPILER_WARNING_STATE()

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Disable copy construction.
		ShaderImage( const ShaderImage& ) = delete;
	//!	Constructs this @ref ShaderImage instance.
		ShaderImage( ShaderImage&& );
	//!	Constructs this @ref ShaderImage instance.
		ShaderImage();

		~ShaderImage();

	// ---------------------------------------------------

	public:
		const ImageSource*	GetSource() const;

	// ---------------------------------------------------

	public:
		VkResult	Upload(
			IoBuilder& ioBuilder,
			MipIndex firstMip = FirstMipInImage,
			MipIndex lastMip = LastMipInImage,
			ArraySlice firstSlice = FirstSliceInImage,
			ArraySlice lastSlice = LastSliceInImage
		);

	// ---------------------------------------------------

	public:
		VkResult	BindResources( GpuHeap& heap, const ImageSource& source );

		void		FreeResources( GpuHeap& heap );

	// ---------------------------------------------------

	//!	Disable copy assignment.
		ShaderImage&	operator=( const ShaderImage& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		const ImageSource*	_source;
		VmaAllocation		_backing;
		VkImage				_image;

	// ---------------------------------------------------

		friend void	Swap( ShaderImage&, ShaderImage& );
	};

// ---

	class Geometry {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Disable copy construction.
		Geometry( const Geometry& ) = delete;
	//!	Constructs this @ref Geometry instance.
		Geometry( Geometry&& );
	//!	Constructs this @ref Geometry instance.
		Geometry();

		~Geometry();

	// ---------------------------------------------------

	public:
		const GeometrySource*	GetSource() const;

	// ---------------------------------------------------

	public:
		VkResult	Upload( IoBuilder& ioBuilder );

	// ---------------------------------------------------

	public:
		VkResult	BindResources( GpuHeap& heap, const GeometrySource& source );

		void		FreeResources( GpuHeap& heap );

	// ---------------------------------------------------

	public:
		operator VkBuffer();

	// ---------------------------------------------------

	//!	Disable copy assignment.
		Geometry&	operator=( const Geometry& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		const GeometrySource*	_source;
		VmaAllocation			_backing;
		VkBuffer				_buffer;

	// ---------------------------------------------------

		friend void	Swap( Geometry&, Geometry& );
	};

// ---

	class SparseShaderImage {
	// - TYPE PUBLISHING ---------------------------------

	public:
		using PhysicalTile	= SparsePageManager::PhysicalTile;
		using CachedTile	= Pair<volatile char*, bool>;
		using Tile			= SparsePageManager::Tile;

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
		const ImageSource*	GetSource() const;

	// ---------------------------------------------------

	public:
		PhysicalTile	FindPhysicalTile();

	// ---------------------------------------------------

	public:
	//!	Streams texel data for image tiles from the @ref ImageSource assigned to the resource to the GPU.
	/*!	@param[in] ioBuilder @ref GpuIoBuilder to perform the host -> GPU copy. */
		VkResult	Upload( IoBuilder& ioBuilder );

		template <typename TileIterator>
		void		MakeResident( TileIterator begin, TileIterator end );

	// ---------------------------------------------------

	public:
		VkResult	BindResources( GpuHeap& heap, VkExtent3D tileExtent, const ImageSource& source );

		void		FreeResources( GpuHeap& heap );

	// ---------------------------------------------------

	//!	Disable copy assignment.
		SparseShaderImage&	operator=( const SparseShaderImage& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
	//!	Collection of tiles resident on the GPU.
		ArrayMap<Tile, PhysicalTile>	_residentTilesByCoordinate;
	//!	Collection of tiles resident on the host.
		ArrayMap<Tile, CachedTile>		_cachedTilesByCoordinate;
	//!	Page manager instance providing the device resources used by this @ref SparseImage.
		SparsePageManager				_pageManager;
	/*!	Staging buffer used as a target for tile decompression. The size of this buffer should be much larger
		than can be transferred in a single frame, as it also serves as a cache for decompressed tiles
		not necessarily resident on the GPU. Contents are write-only on the host, and read-only on the GPU. */
		TransferBuffer					_cache;

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