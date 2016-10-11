/*==================================================================*\
  SparseImage.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/Vulkan/SmartPointers.hpp>
#include <Utility/Containers/HashMap.hpp>
#include <Utility/IdentifierPool.hpp>
#include <Scheduling/JobFiber.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {

	struct SparseImageVirtualCoordinate {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref SparseImageVirtualCoordinate instance.
		SparseImageVirtualCoordinate( VkOffset3D index, uint32_t subresourceIndex );
	//!	Constructs this @ref SparseImageVirtualCoordinate instance.
		SparseImageVirtualCoordinate( const SparseImageVirtualCoordinate& ) = default;

		~SparseImageVirtualCoordinate() = default;

	// - DATA MEMBERS ------------------------------------

		VkOffset3D	index;
		uint32_t	subresourceIndex;
	};

// ---

	using SparseImagePhysicalCoordinate = VkDeviceSize;

// ---

	class SparseImagePageManager {
	// - TYPE PUBLISHING ---------------------------------

	public:
		enum : SparseImagePhysicalCoordinate {
			InvalidPhysicalCoordinate = static_cast<SparseImagePhysicalCoordinate>( 0 )
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref SparseImagePageManager instance.
		SparseImagePageManager( VkExtent3D tileExtent, VkDeviceSize tileSizeInBytes );
	//!	Constructs this @ref SparseImagePageManager instance.
		SparseImagePageManager( const SparseImagePageManager& ) = default;

		virtual ~SparseImagePageManager() = default;

	// ---------------------------------------------------

	public:
		virtual SparseImagePhysicalCoordinate	Reserve( SparseImageVirtualCoordinate coordinate ) abstract;

		virtual void							Release( SparseImagePhysicalCoordinate coordinate ) abstract;

	// ---------------------------------------------------

	public:
		static VkExtent3D	CalculateExtent3D( VkPhysicalDevice device, VkDeviceSize granularityInBytes, VkFormat format );

		static VkExtent3D	CalculateExtent2D( VkPhysicalDevice device, VkDeviceSize granularityInBytes, VkFormat format );

		static VkExtent3D	CalculateExtent1D( VkPhysicalDevice device, VkDeviceSize granularityInBytes, VkFormat format );

	// ---------------------------------------------------

	public:
		SparseImageVirtualCoordinate	GetTile( VkOffset3D texel, uint32_t	subresourceIndex = 0 ) const;

		VkExtent3D						GetTileExtentInTexels() const;

		VkDeviceSize					GetTileSizeInBytes() const;

	// - DATA MEMBERS ------------------------------------

	private:
		VkExtent3D		_tileExtent;
		VkDeviceSize	_tileSizeInBytes;
	};

// ---

	class HardwareSparseImagePageManager : public SparseImagePageManager {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref HardwareSparseImagePageManager instance.
		HardwareSparseImagePageManager( VkExtent3D							tileExtent,
										Eldritch2::uint32					maximumResidentTileCount,
										Vulkan::UniquePointer<VkImage>		image,
										Vulkan::UniquePointer<VkImageView>	imageView,
										Eldritch2::Allocator&				allocator );
	//!	Constructs this @ref HardwareSparseImagePageManager instance.
		HardwareSparseImagePageManager( HardwareSparseImagePageManager&& );

		~HardwareSparseImagePageManager() = default;

	// ---------------------------------------------------

	public:
		SparseImagePhysicalCoordinate	Reserve( SparseImageVirtualCoordinate coordinate ) override;

		void							Release( SparseImagePhysicalCoordinate coordinate ) override;

	// - DATA MEMBERS ------------------------------------

	private:
		Vulkan::UniquePointer<VkDeviceMemory>						_memory;
		Vulkan::UniquePointer<VkImage>								_image;
		Vulkan::UniquePointer<VkImageView>							_imageView;

		Eldritch2::IdentifierPool<SparseImagePhysicalCoordinate>	_bindPool;
	};

// ---------------------------------------------------

	class SoftwareSparseImagePageManager : public SparseImagePageManager {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref SoftwareSparseImagePageManager instance.
		SoftwareSparseImagePageManager( VkExtent3D								tileExtent,
										Vulkan::UniquePointer<VkDeviceMemory>	memory,
										Vulkan::UniquePointer<VkImage>			backingImage,
										Vulkan::UniquePointer<VkImageView>		backingImageView,
										Vulkan::UniquePointer<VkImage>			indirectionImage,
										Vulkan::UniquePointer<VkImageView>		indirectionImageView,
										Eldritch2::Allocator&					allocator );
	//!	Constructs this @ref SoftwareSparseImagePageManager instance.
		SoftwareSparseImagePageManager( SoftwareSparseImagePageManager&& );

		~SoftwareSparseImagePageManager() = default;

	// ---------------------------------------------------

	public:
		SparseImagePhysicalCoordinate	Reserve( SparseImageVirtualCoordinate coordinate ) override;

		void							Release( SparseImagePhysicalCoordinate coordinate ) override;

	// - DATA MEMBERS ------------------------------------

	private:
		Vulkan::UniquePointer<VkDeviceMemory>						_memory;
		Vulkan::UniquePointer<VkImage>								_backingImage;
		Vulkan::UniquePointer<VkImageView>							_backingImageView;

		Vulkan::UniquePointer<VkImage>								_indirectionImage;
		Vulkan::UniquePointer<VkImageView>							_indirectionImageView;

		Eldritch2::IdentifierPool<SparseImagePhysicalCoordinate>	_bindPool;
	};

// ---------------------------------------------------

	class SparseImage {
	// - TYPE PUBLISHING ---------------------------------

	public:
		class CommittedTile {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
		//!	Constructs this @ref CommittedTile instance.
			CommittedTile( SparseImagePhysicalCoordinate coordinate );
		//!	Constructs this @ref CommittedTile instance.
			CommittedTile( const CommittedTile& ) = default;

			~CommittedTile() = default;

		// - DATA MEMBERS ------------------------------------

		public:
			SparseImagePhysicalCoordinate	coordinate;
			size_t							referenceCount;
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref SparseImage instance.
		SparseImage( Eldritch2::UniquePointer<SparseImagePageManager> pageManager, Eldritch2::Allocator& allocator );
	//!	This overload is provided only for signature compatibility and should not be used in practice.
		SparseImage( const SparseImage& );
	//!	Constructs this @ref SparseImage instance.
		SparseImage( SparseImage&& );

		~SparseImage() = default;

	// ---------------------------------------------------

	public:
		bool	IsResident( SparseImageVirtualCoordinate tileCoordinate ) const;

	// ---------------------------------------------------

	public:
		bool	MakeResident( VkExtent3D offsetInTexels, VkExtent3D regionInTexels );
		bool	MakeResident( SparseImageVirtualCoordinate tileCoordinate );

		void	Evict( VkExtent3D offsetInTexels, VkExtent3D regionInTexels );
		void	Evict( SparseImageVirtualCoordinate tileCoordinate );

	// ---------------------------------------------------

	//!	Disable assignment.
		SparseImage&	operator=( const SparseImage& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
	//!	Polymorphic page manager instance providing the device resources used by this @ref SparseImage.
		Eldritch2::UniquePointer<SparseImagePageManager>				_pageManager;
		Eldritch2::HashMap<SparseImageVirtualCoordinate, CommittedTile>	_translationTable;
	};

}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Renderer/Vulkan/SparseImage.inl>
//------------------------------------------------------------------//