/*==================================================================*\
  OutputWindow.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <vulkan/vulkan_core.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Graphics {
		namespace Vulkan {
			class	Vulkan;
			class	Gpu;
		}
	}
}

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {

	class OutputWindow {
	// - TYPE PUBLISHING ---------------------------------

	public:
		enum : uint32_t { MaxQueueDepth = 3u };

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Disable copy construction.
		OutputWindow( const OutputWindow& ) = delete;
	//!	Constructs this @ref OutputWindow instance.
		OutputWindow( OutputWindow&& );
	//!	Constructs this @ref OutputWindow instance.
		OutputWindow();

		~OutputWindow();

	// - REFERENCE MANAGEMENT ----------------------------

	public:
		size_t	AddReference( std::memory_order order = std::memory_order_relaxed ) const;

		size_t	ReleaseReference( std::memory_order order = std::memory_order_release ) const;

		bool	IsReferenced( std::memory_order order = std::memory_order_consume ) const;

	// ---------------------------------------------------

	public:
		VkSwapchainKHR	GetSwapchain() const;

		uint32_t		GetPresentableImageIndex() const;

	// ---------------------------------------------------

	public:
		VkResult	AcquireImage( Gpu& gpu );

	// ---------------------------------------------------

	public:
		VkResult	BindResources( Vulkan& vulkan, Gpu& gpu );

		void		FreeResources( Vulkan& vulkan, Gpu& gpu );

	// ---------------------------------------------------

	//!	Disable copy assignment.
		OutputWindow&	operator=( const OutputWindow& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		mutable Atomic<uint32_t>	_referenceCount;
		Window						_window;
		VkSurfaceKHR				_surface;
		VkSwapchainKHR				_swapchain;
		uint32_t					_imageCount;
		uint32_t					_presentableIndex;
		VkImage						_images[MaxQueueDepth];
		VkFence						_imageAvailable;
		VkEvent						_canComposite;

	// ---------------------------------------------------

		friend void	Swap( OutputWindow&, OutputWindow& );
	};

}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/Vulkan/OutputWindow.inl>
//------------------------------------------------------------------//