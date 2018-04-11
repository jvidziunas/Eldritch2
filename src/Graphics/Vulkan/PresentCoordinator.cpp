/*==================================================================*\
  PresentCoordinator.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/PresentCoordinator.hpp>
#include <Graphics/Vulkan/VulkanTools.hpp>
#include <Graphics/Vulkan/Gpu.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {

	PresentCoordinator::PresentCoordinator(
	) : _outputsByName( MallocAllocator( "Present Coordinator Output by Name Map Allocator" ) ),
		_presentableSwapchains( MallocAllocator( "Present Coordinator Presentable Swapchain List Allocator" ) ),
		_presentableImageIndices( MallocAllocator( "Present Coordinator Presentable Image Index List Allocator" ) ) {
	}

// ---------------------------------------------------

	VkResult PresentCoordinator::Present( Gpu& gpu ) {
		Lock	_( _outputsMutex );

	//	Vulkan spec for VkQueuePresentKHR() requires there be at least one image to be submitted, so we must therefore verify we have an image to present.
		if (_presentableSwapchains) {
			ET_ASSERT( _presentableSwapchains.GetSize() == _presentableImageIndices.GetSize(), "Swapchains and image indices must have 1:1 correspondence!" );

			gpu.PresentAsync( QueueConcept::Presentation, VkPresentInfoKHR{
				VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
				nullptr,		// No extension data.
				0u, nullptr,	// No wait semaphores.
				uint32_t( _presentableSwapchains.GetSize() ),
				_presentableSwapchains.GetData(),
				_presentableImageIndices.GetData(),
				nullptr
			} );

			_presentableSwapchains.Clear();
			_presentableImageIndices.Clear();
		}

		_presentableSwapchains.Reserve( _outputsByName.GetSize() );
		_presentableImageIndices.Reserve( _outputsByName.GetSize() );

		for (NameMap<OutputWindow>::Iterator output( _outputsByName.Begin() ); output != _outputsByName.End(); ) {
			OutputWindow&	window( output->second );

		//	Garbage collect swapchains.
			if (window.IsReferenced( std::memory_order_consume ) == false) {
				window.FreeResources( *_vulkan, gpu );

				output = _outputsByName.Erase( output );
				continue;
			}

			if (Succeeded( window.AcquireImage( gpu ) )) {
				_presentableSwapchains.Append( window.GetSwapchain() );
				_presentableImageIndices.Append( window.GetPresentableImageIndex() );
			}

			++output;
		}

		return VK_SUCCESS;
	}

// ---------------------------------------------------

	CountedPointer<OutputWindow> PresentCoordinator::FindWindowByName( const Utf8Char* name ) {
		Lock	_( _outputsMutex );

		NameMap<OutputWindow>::Iterator	candidate( _outputsByName.Find( name, Hash<decltype(name)>(), EqualTo<decltype(name)>() ) );

		if (candidate == _outputsByName.End()) {
			candidate = _outputsByName.Emplace( String<>( name, MallocAllocator( "Output Name Allocator" ) ) ).first;
		}

		return { &candidate->second };
	}

// ---------------------------------------------------

	VkResult PresentCoordinator::BindResources( Vulkan& vulkan ) {
		_vulkan = eastl::addressof( vulkan );

		return VK_SUCCESS;
	}

// ---------------------------------------------------

	void PresentCoordinator::FreeResources( Vulkan& /*vulkan*/ ) {
		_vulkan = nullptr;
	}

}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2