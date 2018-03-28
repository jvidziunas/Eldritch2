/*==================================================================*\
  ResidencyCoordinator.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/ResidencyCoordinator.hpp>
#include <Graphics/Vulkan/VulkanTools.hpp>
#include <Scheduling/JobExecutor.hpp>
#include <Graphics/ImageSource.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {

	using namespace ::Eldritch2::Scheduling;

	ResidencyCoordinator::ResidencyCoordinator(
	) : _allocator( "Residency Manager Root Allocator" ),
		_meshesBySource( MallocAllocator( "Geometry By Source Collection Allocator" ) ),
		_imagesBySource( MallocAllocator( "Image By Source Collection Allocator" ) ) {
	}

// ---------------------------------------------------

	VkResult ResidencyCoordinator::SubmitFrameIo( JobExecutor& executor, Gpu& gpu ) {
		executor.AwaitCondition( _ioBuilder.CheckCommandsConsumed( gpu ) );

		return _ioBuilder.SubmitCommands( gpu );
	}

// ---------------------------------------------------

	VkResult ResidencyCoordinator::BindResources( Gpu& gpu, VkDeviceSize transferBufferSize ) {
		using Eldritch2::Swap;

		GpuHeap heap;
		ET_FAIL_UNLESS( heap.BindResources( gpu, GpuHeapBlockSize ) );
		ET_AT_SCOPE_EXIT( heap.FreeResources( gpu ) );

		IoBuilder ioBuilder;
		ET_FAIL_UNLESS( ioBuilder.BindResources( gpu, heap, transferBufferSize ) );
		ET_AT_SCOPE_EXIT( ioBuilder.FreeResources( gpu, heap ) );

		Swap( _heap,      heap );
		Swap( _ioBuilder, ioBuilder );

		return VK_SUCCESS;
	}

// ---------------------------------------------------

	void ResidencyCoordinator::FreeResources( Gpu& gpu ) {
		_imagesBySource.Clear();
		_meshesBySource.Clear();

		_heap.FreeResources( gpu );
	}

// ---------------------------------------------------

	VkResult ResidencyCoordinator::Insert( const MeshSource& source, bool andMakeResident ) {
	//	const MeshSource::Dimensions dimensions( source.GetDimensions() );

		Mesh mesh;
		if (andMakeResident) {
			ET_FAIL_UNLESS( MakeResident( mesh._indices, source ) );
			ET_FAIL_UNLESS( MakeResident( mesh._vertices, source ) );
		}

		_meshesBySource.Emplace( &source, eastl::move( mesh ) );

		return VK_SUCCESS;
	}

// ---------------------------------------------------

	VkResult ResidencyCoordinator::Insert( const ImageSource& source, bool andMakeResident ) {
		const ImageSource::Dimensions dimensions( source.GetDimensions() );

		ShaderImage	image;
		ET_FAIL_UNLESS( image.BindResources( _heap, VkExtent3D{ dimensions.widthInTexels, dimensions.heightInTexels, dimensions.depthInTexels }, 0u ) );
		ET_AT_SCOPE_EXIT( image.FreeResources( _heap ) );
		
		if (andMakeResident) {
			ET_FAIL_UNLESS( MakeResident( image, source ) );
		}

		_imagesBySource.Emplace( &source, eastl::move( image ) );

		return VK_SUCCESS;
	}

// ---------------------------------------------------

	void ResidencyCoordinator::Erase( const MeshSource& source ) {
		const auto candidate( _meshesBySource.Find( &source ) );

		if (candidate != _meshesBySource.End()) {
			candidate->second.FreeResources( _heap );

			_meshesBySource.Erase( candidate );
		}
	}

// ---------------------------------------------------

	void ResidencyCoordinator::Erase( const ImageSource& source ) {
		const auto candidate( _imagesBySource.Find( &source ) );

		if (candidate != _imagesBySource.End()) {
			candidate->second.FreeResources( _heap );

			_imagesBySource.Erase( candidate );
		}
	}

// ---------------------------------------------------

	VkResult ResidencyCoordinator::MakeResident( VertexBuffer& /*target*/, const MeshSource& /*source*/ ) {
		return VK_SUCCESS;
	}

// ---------------------------------------------------

	VkResult ResidencyCoordinator::MakeResident( IndexBuffer& /*target*/, const MeshSource& /*source*/ ) {
		return VK_SUCCESS;
	}

// ---------------------------------------------------

	VkResult ResidencyCoordinator::MakeResident( ShaderImage& /*target*/, const ImageSource& /*source*/ ) {
		return VK_SUCCESS;
	}

}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2