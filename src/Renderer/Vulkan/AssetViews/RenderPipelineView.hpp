/*==================================================================*\
  RenderPipelineView.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/ResizableArray.hpp>
#include <Renderer/Vulkan/SmartPointers.hpp>
#include <Utility/Containers/ArraySet.hpp>
#include <Utility/UniquePointer.hpp>
#include <Utility/StringLiteral.hpp>
#include <Assets/AssetView.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Assets {
		class	AssetLibrary;
	}

	namespace Renderer {
		namespace Vulkan {
			namespace AssetViews {
				class	ImageView;
			}

			template <typename>
			class	VulkanResult;
			class	Device;
		}
	}

	template <typename CreatedObject>
	class	Result;
	template <typename Iterator>
	class	Range;
}

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {
namespace AssetViews {

	class RenderPipelineView : public Assets::AssetView {
	// - TYPE PUBLISHING ---------------------------------

	public:
		class Pass {
		// - TYPE PUBLISHING ---------------------------------

		public:
			enum : size_t {
			//!	Maximum number of color attachments that may be attached to a framebuffer per pass.
				MaximumColorAttachmentsPerPass	= 8u,
			//!	Maximum number of attachments (color and depth) that may be attached to a framebuffer per pass.
				MaximumAttachmentsPerPass		= (MaximumColorAttachmentsPerPass + 1)
			};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
		//!	Constructs this @ref Pass instance.
			Pass( Eldritch2::Allocator& allocator );
		//!	Constructs this @ref Pass instance.
			Pass( const Pass& ) = default;

			~Pass() = default;

		// ---------------------------------------------------

		public:
			Vulkan::VulkanResult<VkRenderPass>	CreateRenderPass( Vulkan::Device& deviceContext ) const;

		// - DATA MEMBERS ------------------------------------

		private:
			Eldritch2::ArraySet<const AssetViews::ImageView*>	_attachments;
			VkClearValue										_clearValues[MaximumAttachmentsPerPass];
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref MeshView instance.
		RenderPipelineView( const Eldritch2::Utf8Char* const name, Eldritch2::ResizableArray<Pass>&& passes );
	//!	Disable copying.
		RenderPipelineView( const RenderPipelineView& ) = delete;

		~RenderPipelineView() = default;

	// ---------------------------------------------------

	public:
		template <typename Visitor>
		void	VisitPasses( Visitor&& visitor ) const;

	// ---------------------------------------------------

	public:
		static Eldritch2::Result<Eldritch2::UniquePointer<Assets::AssetView>>	CreateView( Eldritch2::Allocator& allocator, const Assets::AssetLibrary& library, const Eldritch2::Utf8Char* const name, Eldritch2::Range<const char*> assetBytes );

		static ETPureFunctionHint Eldritch2::Utf8Literal						GetExtension();

	// ---------------------------------------------------

	//!	Disable assignment.
		RenderPipelineView&	operator=( const RenderPipelineView& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		Eldritch2::ResizableArray<Pass>	_passes;
	};

}	// namespace AssetViews
}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2