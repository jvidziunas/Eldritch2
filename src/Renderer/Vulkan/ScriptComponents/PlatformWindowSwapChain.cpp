/*==================================================================*\
  PlatformWindowSwapChain.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/Vulkan/ScriptComponents/PlatformWindowSwapChain.hpp>
#include <Renderer/Vulkan/ObjectBuilders.hpp>
#include <Utility/Assert.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Platform;
using namespace ::Eldritch2::Core;

#if defined( CreateSemaphore )
#	pragma push_macro( "CreateSemaphore" )
#	undef CreateSemaphore
#	define CLEANUP_CREATESEMAPHORE 1
#endif	// defined( CreateSemaphore )

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {
namespace ScriptComponents {
namespace {

	static ETInlineHint Vulkan::UniquePointer<VkSurfaceKHR> CreateSurface( VkInstance instance, const PlatformWindow& window, const VkAllocationCallbacks* allocator ) {
		auto	createSurfaceResult( SurfaceBuilder().Create( instance, window, allocator ) );

		ETRuntimeAssert( createSurfaceResult );

		return eastl::move( *createSurfaceResult );
	}

}	// anonymous namespace

// ---------------------------------------------------

	PlatformWindowSwapChain::PlatformWindowSwapChain( VkInstance instance ) : SwapChain( CreateSurface( instance, *this, nullptr ) ) {}

// ---------------------------------------------------

	void PlatformWindowSwapChain::ConfigureForRebuild( SwapChainBuilder& swapChainBuilder, ImageViewBuilder& imageViewBuilder ) {
		const auto	presentationParameters( PlatformWindow::GetPresentationParameters() );

		swapChainBuilder
			.SetExtent( { presentationParameters.widthInPixels, presentationParameters.heightInPixels } )
			.SetFormatAndColorSpace( VkFormat::VK_FORMAT_A8B8G8R8_SRGB_PACK32, VkColorSpaceKHR::VK_COLORSPACE_SRGB_NONLINEAR_KHR )
			.SetSlicesPerImage( 1u );

		imageViewBuilder
			.SetImage2D( { presentationParameters.widthInPixels, presentationParameters.heightInPixels } )
			.SetFormat( VkFormat::VK_FORMAT_A8B8G8R8_SRGB_PACK32 );
	}

}	// namespace ScriptComponents
}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2
#if defined( CLEANUP_CREATESEMAPHORE )
#	pragma pop_macro( "CreateSemaphore" )
#	undef CLEANUP_CREATESEMAPHORE
#endif	// defined( CLEANUP_CREATESEMAPHORE )