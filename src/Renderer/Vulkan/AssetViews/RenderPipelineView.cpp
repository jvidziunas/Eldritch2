/*==================================================================*\
  RenderPipelineView.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/Vulkan/AssetViews/RenderPipelineView.hpp>
#include <Renderer/Vulkan/AssetViews/ImageView.hpp>
#include <Renderer/Vulkan/VulkanResult.hpp>
#include <Utility/Containers/Range.hpp>
#include <Renderer/Vulkan/Device.hpp>
#include <Assets/AssetLibrary.hpp>
#include <Utility/Result.hpp>
//------------------------------------------------------------------//
#include <Flatbuffers/RenderPipeline_generated.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Logging;
using namespace ::Eldritch2::Assets;
using namespace ::flatbuffers;

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {
namespace AssetViews {
namespace {

	static ETInlineHint VulkanResult<VkFramebuffer> CreateFramebuffer( const FlatBuffers::PassDescriptor& pass, VkRenderPass renderPass, const AssetLibrary& library, Device& deviceContext ) {
		VkFramebufferCreateInfo	createInfo;
		VkFramebuffer			result;
		VkImageView				attachments[RenderPipelineView::Pass::MaximumAttachmentsPerPass];
		VkImageView*			out( attachments );

	//	Fill out basic parameters for creating a VkFramebuffer.
		createInfo.sType			= VkStructureType::VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		createInfo.pNext			= nullptr;
		createInfo.flags			= 0u;
		createInfo.renderPass		= renderPass;
		createInfo.pAttachments		= attachments;
		createInfo.width			= 1;
		createInfo.height			= 1;
		createInfo.layers			= 1;

	//	Fill out all the framebuffer attachment information for each of the color attachments used in this render pass.
		for( const auto attachment : *pass.ColorAttachments() ) {
			const ImageView*	assetView( library.ResolveView<ImageView>( attachment->ResourceName()->c_str() ) );

		//	Ensure the asset exists and is able to be bound as a framebuffer attachment.
			if( assetView == nullptr || VK_NULL_HANDLE == assetView->GetFramebufferAttachmentView() ) {
				return VkResult::VK_ERROR_VALIDATION_FAILED_EXT;
			}

		//	Add the VkImageView to the referenced attachment collection and proceed.
			*out++ = assetView->GetFramebufferAttachmentView();
		}

	//	Fill out the framebuffer attachment information for the depth attachment if used in this render pass.
		if( const auto attachment = pass.DepthAttachment() ) {
			const ImageView*	assetView( library.ResolveView<ImageView>( attachment->ResourceName()->c_str() ) );

		//	Ensure the asset exists and is able to be bound as a framebuffer attachment.
			if( assetView == nullptr || VK_NULL_HANDLE == assetView->GetFramebufferAttachmentView() ) {
				return VkResult::VK_ERROR_VALIDATION_FAILED_EXT;
			}

		//	Add the VkImageView to the referenced attachment collection.
			*out++ = assetView->GetFramebufferAttachmentView();
		}

	//	Set the number of attachments to the combined total of referenced color and depth attachments.
		createInfo.attachmentCount = static_cast<int32_t>( eastl::distance( attachments, out ) );

		const VkResult	createFramebufferResult( vkCreateFramebuffer( deviceContext, &createInfo, deviceContext.GetAllocator(), &result ) );
		if( VkResult::VK_SUCCESS != createFramebufferResult ) {
			return createFramebufferResult;
		}
	}

}	// anonymous namespace

	RenderPipelineView::Pass::Pass( Allocator& allocator ) : _attachments( { allocator, "Vulkan Render Pipeline Pass Attachment Collection Allocator" } ) {}

// ---------------------------------------------------

	VulkanResult<VkRenderPass> RenderPipelineView::Pass::CreateRenderPass( Device& deviceContext ) const {
		VkRenderPassCreateInfo	createInfo;
		VkRenderPass			result;

		createInfo.sType			= VkStructureType::VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		createInfo.pNext			= nullptr;
		createInfo.flags			= 0;
		createInfo.attachmentCount	;
		createInfo.pAttachments;
		createInfo.subpassCount;
		createInfo.pSubpasses;
		createInfo.dependencyCount;
		createInfo.pDependencies;

		const auto	createPassResult( vkCreateRenderPass( deviceContext, &createInfo, deviceContext.GetAllocator(), &result ) );
		if( VkResult::VK_SUCCESS != createPassResult ) {
			return createPassResult;
		}

		return UniquePointer<VkRenderPass>( result, { deviceContext, deviceContext.GetAllocator() } );
	}

// ---------------------------------------------------

	RenderPipelineView::RenderPipelineView( const Utf8Char* const name, ResizableArray<Pass>&& passes ) : AssetView( name ), _passes( eastl::move( passes ) ) {}

// ---------------------------------------------------

	Result<Eldritch2::UniquePointer<AssetView>> RenderPipelineView::CreateView( Allocator& allocator, const AssetLibrary& library, const Utf8Char* const name, Range<const char*> rawBytes ) {
		ResizableArray<Pass>	renderPasses( { allocator, "Vulkan Render Pass Collection Allocator" } );
		Verifier				verifier( reinterpret_cast<const uint8_t*>( rawBytes.Begin() ), rawBytes.GetSize() );

	//	Ensure we're working with data that can plausibly represent a pipeline.
		if( !FlatBuffers::RenderPipelineBufferHasIdentifier( rawBytes.Begin() ) || !FlatBuffers::VerifyRenderPipelineBuffer( verifier ) ) {
			library.GetLog()( MessageSeverity::Error, "Render pipeline '{}' is malformed." ET_UTF8_NEWLINE_LITERAL, name );
			return Error::InvalidParameter;
		}

		const FlatBuffers::RenderPipeline&	definition( *FlatBuffers::GetRenderPipeline( rawBytes.Begin() ) );

		auto	result( MakeUnique<RenderPipelineView>( allocator, name, eastl::move( renderPasses ) ) );
		if( !result ) {
			return Error::OutOfMemory;
		}

		return eastl::move( result );
	}

// ---------------------------------------------------

	ETPureFunctionHint Utf8Literal RenderPipelineView::GetExtension() {
		return Utf8Literal( FlatBuffers::RenderPipelineExtension() );
	}

}	// namespace AssetViews
}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2