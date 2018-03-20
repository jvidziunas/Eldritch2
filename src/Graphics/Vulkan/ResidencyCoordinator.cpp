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
#include <Graphics/Vulkan/Gpu.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {

	using namespace ::Eldritch2::Scheduling;

	ResidencyCoordinator::ResidencyCoordinator(
	) : _allocator( "Residency Manager Root Allocator" ),
		_sparseShaderImages( MallocAllocator( "Sparse Image By Source Collection Allocator" ) ),
		_geometry( MallocAllocator( "Geometry By Source Collection Allocator" ) ),
		_shaderImages( MallocAllocator( "Image By Source Collection Allocator" ) ) {
	}

// ---------------------------------------------------

	ResidencyCoordinator::~ResidencyCoordinator() {
		ET_ASSERT( _sparseShaderImages.IsEmpty(), "Leaking Vulkan sparse shader images!" );
		ET_ASSERT( _geometry.IsEmpty(),           "Leaking Vulkan geometry!" );
		ET_ASSERT( _shaderImages.IsEmpty(),       "Leaking Vulkan shader images!" );
	}

// ---------------------------------------------------

	VkResult ResidencyCoordinator::SubmitFrameIo( JobExecutor& executor, Gpu& gpu ) {
		for (const SparseShaderImage& image : _sparseShaderImages) {
		/*	Use of const_cast is gross, but this is one of the suggested solutions in C++ Defect Report 103.
		 *	See http://www.open-std.org/jtc1/sc22/wg21/docs/lwg-defects.html#103 */
			const_cast<SparseShaderImage&>(image).Upload( _ioBuilder );
		}

		return _ioBuilder.SubmitCommands( executor, gpu );
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
		_sparseShaderImages.Clear();
		_shaderImages.Clear();
		_geometry.Clear();

		_heap.FreeResources( gpu );
	}

// ---------------------------------------------------

	VkResult ResidencyCoordinator::Insert( const GeometrySource& source, bool andUpload, bool allowSwap ) {
		ResidentSet<Geometry>::ValueType geometry;
		ET_FAIL_UNLESS( geometry.BindResources( _heap, source ) );
		ET_AT_SCOPE_EXIT( geometry.FreeResources( _heap ) );

		if (andUpload) {
			ET_FAIL_UNLESS( geometry.Upload( _ioBuilder ) );
		}

		const auto insertResult( _geometry.Insert( eastl::move( geometry ) ) );
		if (allowSwap && !insertResult.second) {
			Swap( geometry, const_cast<Geometry&>(*insertResult.first) );
		}

		return VK_SUCCESS;
	}

// ---------------------------------------------------

	VkResult ResidencyCoordinator::Insert( const ImageSource& source, bool andUpload, bool allowSwap ) {
		ResidentSet<ShaderImage>::ValueType	image;
		ET_FAIL_UNLESS( image.BindResources( _heap, source ) );
		ET_AT_SCOPE_EXIT( image.FreeResources( _heap ) );
		
		if (andUpload) {
			ET_FAIL_UNLESS( image.Upload( _ioBuilder ) );
		}

		const auto insertResult( _shaderImages.Insert( eastl::move( image ) ) );
		if (allowSwap && !insertResult.second) {
			Swap( image, const_cast<ShaderImage&>(*insertResult.first) );
		}

		return VK_SUCCESS;
	}

// ---------------------------------------------------

	void ResidencyCoordinator::Erase( const GeometrySource& source ) {
		const auto candidate( _geometry.Find( &source, _geometry.GetHashPredicate(), _geometry.GetEqualityPredicate() ) );

		if (candidate != _geometry.End()) {
		/*	Use of const_cast is gross, but this is one of the suggested solutions in C++ Defect Report 103.
		 *	See http://www.open-std.org/jtc1/sc22/wg21/docs/lwg-defects.html#103 */
			const_cast<Geometry&>(*candidate).FreeResources( _heap );

			_geometry.Erase( candidate );
		}
	}

// ---------------------------------------------------

	void ResidencyCoordinator::Erase( const ImageSource& source ) {
		const auto candidate( _shaderImages.Find( &source, _shaderImages.GetHashPredicate(), _shaderImages.GetEqualityPredicate() ) );

		if (candidate != _shaderImages.End()) {
		/*	Use of const_cast is gross, but this is one of the suggested solutions in C++ Defect Report 103.
		 *	See http://www.open-std.org/jtc1/sc22/wg21/docs/lwg-defects.html#103 */
			const_cast<ShaderImage&>(*candidate).FreeResources( _heap );

			_shaderImages.Erase( candidate );
		}
	}

}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2