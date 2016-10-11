/*==================================================================*\
  MeshView.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/Vulkan/AssetViews/SpirVShaderPipelineView.hpp>
#include <Renderer/Vulkan/AssetViews/MeshView.hpp>
#include <Renderer/Vulkan/VulkanResult.hpp>
#include <Utility/Containers/Range.hpp>
#include <Renderer/Vulkan/Device.hpp>
#include <Assets/AssetLibrary.hpp>
#include <Utility/ErrorCode.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Logging;
using namespace ::Eldritch2::Assets;

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {
namespace AssetViews {
namespace {

	struct VertexInputStateCreateInfo : public VkPipelineVertexInputStateCreateInfo {
		VertexInputStateCreateInfo() {
			sType	= VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
			pNext	= nullptr;
			flags	= 0;
			uint32_t                                    vertexBindingDescriptionCount;
			const VkVertexInputBindingDescription*      pVertexBindingDescriptions;
			uint32_t                                    vertexAttributeDescriptionCount;
			const VkVertexInputAttributeDescription*    pVertexAttributeDescriptions;
		}
	};

// ---

	struct PipelineInputAssemblyStateCreateInfo : public VkPipelineInputAssemblyStateCreateInfo {
	public:
		PipelineInputAssemblyStateCreateInfo( int ) : PipelineInputAssemblyStateCreateInfo() {
			topology				= VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			primitiveRestartEnable	= VK_FALSE;
		}

	protected:
		PipelineInputAssemblyStateCreateInfo() {
			sType	= VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
			pNext	= nullptr;
			flags	= 0;
		}
	};

// ---------------------------------------------------

	static ETInlineHint Eldritch2::UniquePointer<VkVertexInputAttributeDescription[]> CreateInputAttributeDescriptions( Allocator& /*allocator*/ ) {
		return nullptr;
	}

// ---------------------------------------------------

	static ETInlineHint Eldritch2::UniquePointer<VkVertexInputBindingDescription[]> CreateVertexInputBindings( Allocator& /*allocator*/ ) {
		return nullptr;
	}

// ---------------------------------------------------

	static ETInlineHint Eldritch2::UniquePointer<MeshView::Submesh[]> CreateSubmeshes( Allocator& /*allocator*/ ) {
		return nullptr;
	}

}	// anonymous namespace

	MeshView::MeshView( const Utf8Char* const											name,
						UniquePointer<VkBuffer>											buffer,
						DeviceMemoryPool::Allocation									allocation,
						Eldritch2::UniquePointer<VkVertexInputAttributeDescription[]>	aggregateInputAttributeDescriptions,
						Eldritch2::UniquePointer<VkVertexInputBindingDescription[]>		aggregateVertexInputBindings,
						Eldritch2::UniquePointer<Submesh[]>								submeshes ) : AssetView( name ),
																									  _buffer( eastl::move( buffer ) ),
																									  _allocation( eastl::move( allocation ) ),
																									  _aggregatedAttributes( eastl::move( aggregateInputAttributeDescriptions ) ),
																									  _aggregatedBindings( eastl::move( aggregateVertexInputBindings ) ),
																									  _submeshes( eastl::move( submeshes ) ) {}

// ---------------------------------------------------

	Result<Eldritch2::UniquePointer<AssetView>>	MeshView::CreateView( Allocator& allocator, Device& device, DeviceMemoryPool& pool, const AssetLibrary& library, const Utf8Char* const name, Range<const char*> rawBytes ) {
		Eldritch2::UniquePointer<VkVertexInputAttributeDescription[]>	attributes( CreateInputAttributeDescriptions( allocator ) );
		Eldritch2::UniquePointer<VkVertexInputBindingDescription[]>		bindings( CreateVertexInputBindings( allocator ) );
		Eldritch2::UniquePointer<Submesh[]>								submeshes( CreateSubmeshes( allocator ) );
		UniquePointer<VkBuffer>											buffer;

		VkMemoryRequirements	memoryRequirements;
		vkGetBufferMemoryRequirements( device, buffer.Get(), &memoryRequirements );

		Result<DeviceMemoryPool::Allocation>	allocateMemoryResult( pool.TryAllocateRegion( memoryRequirements.size ) );
		if( !allocateMemoryResult ) {
			library.GetLog()( MessageSeverity::Error, "Unable to allocate device memory for mesh '{}'! (Size: {} bytes)" ET_UTF8_NEWLINE_LITERAL, name, memoryRequirements.size );
			return allocateMemoryResult.GetErrorCode();
		}

		const VkResult	bindMemoryResult( vkBindBufferMemory( device, buffer.Get(), pool.GetDeviceMemory(), allocateMemoryResult->GetOffsetIntoPoolInBytes() ) );
		if( VkResult::VK_SUCCESS != bindMemoryResult ) {
			library.GetLog()( MessageSeverity::Error, "Unable to bind device memory for mesh '{}'!" ET_UTF8_NEWLINE_LITERAL, name );
		}

		auto	result( MakeUnique<MeshView>( allocator, name, eastl::move( buffer ), eastl::move( *allocateMemoryResult ), eastl::move( attributes ), eastl::move( bindings ), eastl::move( submeshes ) ) );
		if( !result ) {
			return Error::OutOfMemory;
		}

		return eastl::move( result );
	}

// ---------------------------------------------------

	ETPureFunctionHint Utf8Literal MeshView::GetExtension() {
		return ".E2Mesh";
	}

}	// namespace AssetViews
}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2

