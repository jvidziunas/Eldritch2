/*==================================================================*\
  ObjectBuilders.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/Vulkan/DeviceProperties.hpp>
#include <Renderer/Vulkan/VulkanResult.hpp>
#include <Utility/Containers/ArraySet.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Platform {
		class	PlatformWindow;
	}
}

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {

	struct MipRegion {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref MipRegion instance.
		MipRegion( uint32_t firstMip, uint32_t mipCount = VK_REMAINING_MIP_LEVELS );
	//!	Constructs this @ref MipRegion instance.
		MipRegion( const MipRegion& ) = default;

		~MipRegion() = default;

	// - DATA MEMBERS ------------------------------------

	public:
		uint32_t	firstMip;
		uint32_t	mipCount;
	};

// ---

	struct ArrayRegion {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref ArrayRegion instance.
		ArrayRegion( uint32_t firstSlice, uint32_t sliceCount = VK_REMAINING_ARRAY_LAYERS );
	//!	Constructs this @ref ArrayRegion instance.
		ArrayRegion( const ArrayRegion& ) = default;

		~ArrayRegion() = default;

	// - DATA MEMBERS ------------------------------------

	public:
		uint32_t	firstSlice;
		uint32_t	sliceCount;
	};

// ---

	struct CubeRegion : public ArrayRegion {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref CubeRegion instance.
		CubeRegion( uint32_t firstCube, uint32_t cubeCount = VK_REMAINING_ARRAY_LAYERS );
	//!	Constructs this @ref CubeRegion instance.
		CubeRegion( const CubeRegion& ) = default;

		~CubeRegion() = default;
	};

// ---------------------------------------------------

	extern const ArrayRegion	AllArraySlices;
	extern const CubeRegion		AllCubes;
	extern const MipRegion		AllMips;

// ---------------------------------------------------

	class InstanceBuilder {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref InstanceBuilder instance.
		InstanceBuilder( Eldritch2::Allocator& allocator, uint32_t vulkanApiVersion = VK_API_VERSION_1_0 );
	//!	Disable copying.
		InstanceBuilder( const InstanceBuilder& ) = delete;

		~InstanceBuilder() = default;

	// ---------------------------------------------------

	public:
		InstanceBuilder&	EnableExtension( const char* const name );

		InstanceBuilder&	DisableExtension( const char* const name );

	// ---------------------------------------------------

	public:
		InstanceBuilder&	EnableLayer( const char* const name );

		InstanceBuilder&	DisableLayer( const char* const name );

	// ---------------------------------------------------

	public:
		Vulkan::VulkanResult<::VkInstance>	Create( const VkAllocationCallbacks* callbacks = nullptr );

	// - DATA MEMBERS ------------------------------------

	private:
		Eldritch2::ArraySet<const char*>	_enabledExtensions;
		Eldritch2::ArraySet<const char*>	_enabledLayers;

		VkApplicationInfo					_applicationInfo;
		VkInstanceCreateInfo				_createInfo;
	};

// ---

	class LogicalDeviceBuilder {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref LogicalDeviceBuilder instance.
		LogicalDeviceBuilder( Eldritch2::Allocator& allocator );
	//!	Disable copying.
		LogicalDeviceBuilder( const LogicalDeviceBuilder& ) = delete;

		~LogicalDeviceBuilder() = default;

	// ---------------------------------------------------

	public:
		LogicalDeviceBuilder&	EnableExtension( const char* const name );

		LogicalDeviceBuilder&	DisableExtension( const char* const name );

	// ---------------------------------------------------

	public:
		LogicalDeviceBuilder&	EnableLayer( const char* const name );

		LogicalDeviceBuilder&	DisableLayer( const char* const name );

	// ---------------------------------------------------

	public:
		Vulkan::VulkanResult<VkDevice>	Create( VkPhysicalDevice physicalDevice, const VkAllocationCallbacks* callbacks = nullptr );

	// - DATA MEMBERS ------------------------------------

	private:
		Eldritch2::ArraySet<const char*>	_enabledExtensions;
		Eldritch2::ArraySet<const char*>	_enabledLayers;

		VkDeviceCreateInfo					_createInfo;
	};

// ---

	class ImageBuilder {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref ImageBuilder instance.
		ImageBuilder();
	//!	Disable copying.
		ImageBuilder( const ImageBuilder& ) = delete;

		~ImageBuilder() = default;

	// ---------------------------------------------------

	public:
		ImageBuilder&	SetTiling( VkImageTiling tiling );

		ImageBuilder&	SetFormat( VkFormat format );

		ImageBuilder&	SetMipCount( uint32_t mipCount );
			
		ImageBuilder&	SetExtent( uint32_t extent );

		ImageBuilder&	SetExtent( uint32_t extent, uint32_t arraySliceCount );

		ImageBuilder&	SetExtent( VkExtent2D extent );

		ImageBuilder&	SetExtent( VkExtent2D extent, uint32_t arraySliceCount );

		ImageBuilder&	SetExtent( VkExtent3D extent );

		ImageBuilder&	SetCubeExtent( VkExtent2D extent );

		ImageBuilder&	SetCubeExtent( VkExtent2D extent, uint32_t cubeCount );

	// ---------------------------------------------------

	public:
		Vulkan::VulkanResult<VkImage>	Create( VkDevice device, const VkAllocationCallbacks* callbacks = nullptr );

	// - DATA MEMBERS ------------------------------------

	private:
		VkImageCreateInfo	_createInfo;
	};

// ---

	class ImageViewBuilder {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref ImageViewBuilder instance.
		ImageViewBuilder();
	//!	Disable copying.
		ImageViewBuilder( const ImageViewBuilder& ) = delete;

		~ImageViewBuilder() = default;

	// ---------------------------------------------------

	public:
		ImageViewBuilder&	SetIdentitySwizzle();

		ImageViewBuilder&	SetSwizzle( VkComponentSwizzle r = VkComponentSwizzle::VK_COMPONENT_SWIZZLE_IDENTITY,
										VkComponentSwizzle g = VkComponentSwizzle::VK_COMPONENT_SWIZZLE_IDENTITY,
										VkComponentSwizzle b = VkComponentSwizzle::VK_COMPONENT_SWIZZLE_IDENTITY,
										VkComponentSwizzle a = VkComponentSwizzle::VK_COMPONENT_SWIZZLE_IDENTITY );

		ImageViewBuilder&	SetFormat( VkFormat format );

		ImageViewBuilder&	SetMipRegion( MipRegion region );

		ImageViewBuilder&	SetArrayRegion( ArrayRegion region );
			
		ImageViewBuilder&	SetImage1D( MipRegion mipRegion = AllMips, uint32_t sliceInArray = 0u );

		ImageViewBuilder&	SetImage1DArray( MipRegion mipRegion = AllMips, ArrayRegion arrayRegion = AllArraySlices );

		ImageViewBuilder&	SetImage2D( MipRegion mipRegion = AllMips, uint32_t sliceInArray = 0u );

		ImageViewBuilder&	SetImage2DArray( MipRegion mipRegion = AllMips, ArrayRegion arrayRegion = AllArraySlices );

		ImageViewBuilder&	SetImage3D( MipRegion mipRegion = AllMips );

		ImageViewBuilder&	SetImageCube( MipRegion mipRegion = AllMips, uint32_t firstCubemap = 0u );

		ImageViewBuilder&	SetImageCubeArray( MipRegion mipRegion = AllMips, CubeRegion cubeRegion = AllCubes );

	// ---------------------------------------------------

	public:
		Vulkan::VulkanResult<VkImageView>	Create( VkDevice device, VkImage image, const VkAllocationCallbacks* callbacks = nullptr );

	// - DATA MEMBERS ------------------------------------

	private:
		VkImageViewCreateInfo	_createInfo;
	};

// ---

	class ShaderModuleBuilder {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref ShaderModuleBuilder instance.
		ShaderModuleBuilder();
	//!	Disable copying.
		ShaderModuleBuilder( const ShaderModuleBuilder& ) = delete;

		~ShaderModuleBuilder() = default;

	// ---------------------------------------------------

	public:
		ShaderModuleBuilder&	SetBytecode( const void* bytecodeBegin, size_t bytecodeLengthInBytes );

	// ---------------------------------------------------

	public:
		Vulkan::VulkanResult<VkShaderModule>	Create( VkDevice device, const VkAllocationCallbacks* callbacks = nullptr );

	// - DATA MEMBERS ------------------------------------

	private:
		VkShaderModuleCreateInfo	_createInfo;
	};

// ---

#if defined VK_KHR_surface
	class SurfaceBuilder {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref SurfaceBuilder instance.
		SurfaceBuilder();
	//!	Disable copying.
		SurfaceBuilder( const SurfaceBuilder& ) = delete;

		~SurfaceBuilder() = default;

	// ---------------------------------------------------

		static void	EnableExtensions( Vulkan::LogicalDeviceBuilder& builder );
		static void	EnableExtensions( Vulkan::InstanceBuilder& builder );

	// ---------------------------------------------------

		Vulkan::VulkanResult<VkSurfaceKHR>	Create( VkInstance instance, const Platform::PlatformWindow& window, const VkAllocationCallbacks* callbacks = nullptr );
	};

#if defined VK_KHR_swapchain
	class SwapChainBuilder {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref SwapChainBuilder instance.
		SwapChainBuilder();
	//!	Disable copying.
		SwapChainBuilder( const SwapChainBuilder& ) = delete;

		~SwapChainBuilder() = default;

	// ---------------------------------------------------

	public:
		static void	EnableExtensions( Vulkan::LogicalDeviceBuilder& builder );
		static void	EnableExtensions( Vulkan::InstanceBuilder& builder );

	// ---------------------------------------------------

	public:
		SwapChainBuilder&	SetOldSwapchain( VkSwapchainKHR oldSwapchain );

		SwapChainBuilder&	SetExtent( VkExtent2D extent );

		SwapChainBuilder&	SetUsage( VkImageUsageFlags usageFlags );

		SwapChainBuilder&	SetImageCount( uint32_t bufferCount );

		SwapChainBuilder&	SetSlicesPerImage( uint32_t sliceCount );

		SwapChainBuilder&	SetFormatAndColorSpace( VkFormat imageFormat, VkColorSpaceKHR imageColorSpace );

		SwapChainBuilder&	SetDevicePreferredFormatAndColorSpace( VkPhysicalDevice device, VkSurfaceKHR surface );

	// ---------------------------------------------------

	public:
		bool	IsUsableOnDevice( VkPhysicalDevice device, VkSurfaceKHR surface ) const;

	// ---------------------------------------------------

	public:
		Vulkan::VulkanResult<VkSwapchainKHR>	Create( VkDevice device, VkSurfaceKHR surface, const VkAllocationCallbacks* callbacks = nullptr );

	// - DATA MEMBERS ------------------------------------

	private:
		VkSwapchainCreateInfoKHR	_createInfo;
	};
#endif	// defined VK_KHR_swapchain
#endif	// defined VK_KHR_surface

}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Renderer/Vulkan/ObjectBuilders.inl>
//------------------------------------------------------------------//