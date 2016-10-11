/*==================================================================*\
  SwapChain.hpp
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
#include <Utility/Mpl/IntTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Renderer {
		namespace Vulkan {
			class	SwapChainBuilder;
			class	ImageViewBuilder;
		}
	}
}

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {

	class SwapChain {
	// - TYPE PUBLISHING ---------------------------------

	public:
		enum : Eldritch2::uint32 {
			MaximumQueuedImageCount = 3u,
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref SwapChain instance.
	/*!	@param[in] surface Vulkan surface the underlying swap chain will present to. */
		SwapChain( Vulkan::UniquePointer<VkSurfaceKHR> surface );
	//!	Disable copying.
		SwapChain( const SwapChain& ) = delete;

		~SwapChain() = default;

	// ---------------------------------------------------

	public:
		Eldritch2::uint32	GetCurrentImageIndex() const;

	// ---------------------------------------------------

	public:
		explicit operator	VkSwapchainKHR() const;
		explicit operator	VkImageView() const;
		explicit operator	VkImage() const;

	// ---------------------------------------------------

	public:
		VkResult	OnFrameBegin( VkDevice deviceContext, VkSemaphore semaphore, Eldritch2::uint64 acquisitionTimeoutPeriodNanoseconds = 0u, const VkAllocationCallbacks* allocationCallbacks = nullptr );

		VkResult	AcquireNextImage( VkDevice deviceContext, VkSemaphore semaphore, Eldritch2::uint64 timeoutPeriodNanoseconds = 0u );

		VkResult	Rebuild( VkDevice deviceContext, const VkAllocationCallbacks* allocationCallbacks = nullptr );

	// ---------------------------------------------------

	protected:
		virtual void	ConfigureForRebuild( Vulkan::SwapChainBuilder& swapChainBuilder, Vulkan::ImageViewBuilder& imageViewBuilder ) abstract;

	// ---------------------------------------------------

	//!	Disable assignment.
		SwapChain&	operator=( const SwapChain& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		Vulkan::UniquePointer<VkSurfaceKHR>		_surface;
		Vulkan::UniquePointer<VkSwapchainKHR>	_swapChain;
	//!	Index of the active image in the _images and _views arrays. Always in the range 0 < _currentImageIndex < MaximumQueuedImageCount.
		Eldritch2::uint32						_currentImageIndex;

	//!	Note that these are deliberately not unique/owned pointer objects as the Vulkan implementation is defined as being responsible for their deletion.
		VkImage									_images[MaximumQueuedImageCount];
	//!	VkImageViews the devices can use for rendering. Unlike the objects in the @ref _images array, these are considered to be owned by the application.
		Vulkan::UniquePointer<VkImageView>		_views[MaximumQueuedImageCount];
	};

}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Renderer/Vulkan/SwapChain.inl>
//------------------------------------------------------------------//