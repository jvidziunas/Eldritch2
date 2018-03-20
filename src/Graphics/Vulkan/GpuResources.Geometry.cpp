/*==================================================================*\
  GpuResources.Geometry.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/GpuResources.hpp>
#include <Graphics/Vulkan/VulkanTools.hpp>
#include <Graphics/Vulkan/IoBuilder.hpp>
#include <Graphics/Vulkan/GpuHeap.hpp>
#include <Graphics/GeometrySource.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {
namespace {

	static ETInlineHint ETPureFunctionHint VkBufferCreateInfo GetCreateInfo( const GeometrySource::Dimensions& source ) {
		return VkBufferCreateInfo{
			VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
			nullptr,		// No extension data.
			0u,				// Default flags.
			source.vertexCount * 32u,
			VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_SHARING_MODE_EXCLUSIVE,
			0u, nullptr,	// Exclusive sharing.
		};
	}

}	// anonymous namespace

	Geometry::Geometry() : _source( nullptr ), _backing( nullptr ), _buffer( nullptr ) {}

// ---------------------------------------------------

	Geometry::Geometry(
		Geometry&& geometry
	) : _source( eastl::exchange( geometry._source, nullptr ) ),
		_backing( eastl::exchange( geometry._backing, nullptr ) ),
		_buffer( eastl::exchange( geometry._buffer, nullptr ) ) {
	}

// ---------------------------------------------------

	Geometry::~Geometry() {
		ET_ASSERT( _backing == nullptr, "Leaking Vulkan buffer!" );
		ET_ASSERT( _buffer == nullptr,  "Leaking Vulkan memory allocation!" );
	}

// ---------------------------------------------------

	VkResult Geometry::Upload( IoBuilder& ioBuilder ) {
		ioBuilder.TransferToGpu( _buffer, 0u, 1u );

		return VK_SUCCESS;
	}

// ---------------------------------------------------

	VkResult Geometry::BindResources( GpuHeap& heap, const GeometrySource& source ) {
		enum : VkMemoryPropertyFlags { InferMemoryPropertiesFromUsage = 0u };
		
		using ::Eldritch2::Swap;

		const VkBufferCreateInfo		bufferInfo( GetCreateInfo( source.GetDimensions() ) );
		const VmaAllocationCreateInfo	allocationInfo{
			0u,
			VMA_MEMORY_USAGE_GPU_ONLY,
			InferMemoryPropertiesFromUsage,
			InferMemoryPropertiesFromUsage,
			VK_NULL_HANDLE,	// Use default pool scheme.
			nullptr			// No user data.
		};

		VmaAllocation	backing;
		VkBuffer		buffer;

		ET_FAIL_UNLESS( vmaCreateBuffer( heap, &bufferInfo, &allocationInfo, &buffer, &backing, nullptr ) );
		ET_AT_SCOPE_EXIT( vmaDestroyBuffer( heap, buffer, backing ) );

		Swap( _backing, backing );
		Swap( _buffer,	buffer );
		_source = &source;

		return VK_SUCCESS;
	}

// ---------------------------------------------------

	void Geometry::FreeResources( GpuHeap& heap ) {
		if (eastl::exchange( _source, nullptr )) {
			heap.AddGarbage( { eastl::exchange( _buffer, nullptr ), eastl::exchange( _backing, nullptr ) } );
		}
	}

// ---------------------------------------------------

	void Swap( Geometry& geometry0, Geometry& geometry1 ) {
		using ::Eldritch2::Swap;

		Swap( geometry0._backing,	geometry1._backing );
		Swap( geometry0._buffer,	geometry1._buffer );
		Swap( geometry0._source,	geometry1._source );
	}

}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2