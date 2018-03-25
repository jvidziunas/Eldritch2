/*==================================================================*\
  RenderPipelineAsset.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Assets/Asset.hpp>
//------------------------------------------------------------------//
#include <vulkan/vulkan_core.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Graphics {
		namespace AssetViews {
			class	ImageAsset;
		}
	}
}

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {
namespace AssetViews {

	class RenderPipelineAsset : public Assets::Asset {
	// - TYPE PUBLISHING ---------------------------------

	public:
		enum : size_t {
			MaxInputAttachmentsPerPass	= 8u,
			MaxOutputAttachmentsPerPass	= 8u,
			MaxAttachmentsPerPass		= (MaxInputAttachmentsPerPass + MaxOutputAttachmentsPerPass + 1)
		};

	// ---

	public:
		struct Attachment {
			Utf8Char	name[MaxPathLength];
			VkFormat	format;
			float32		widthScalar;
			float32		heightScalar;
		};

	// ---

	public:
		using AttachmentId = uint32_t;

	// ---

	public:
		class Stage {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
		//!	Constructs this @ref Stage instance.
			Stage( const Utf8Char* const shaderUsageName );
		//!	Constructs this @ref Stage instance.
			Stage( const Stage& ) = default;

			~Stage() = default;

		// ---------------------------------------------------

		public:
			const Utf8Char* GetShaderUsageName() const;

		// - DATA MEMBERS ------------------------------------

		private:
			Utf8Char		_shaderUsageName[MaxPathLength];
			AttachmentId	_inputAttachments[MaxInputAttachmentsPerPass];
			AttachmentId	_outputAttachments[MaxOutputAttachmentsPerPass];
			AttachmentId	_depth;
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref RenderPipelineAsset instance.
	/*!	@param[in] filePath Null-terminated, UTF-8-encoded character sequence containing the file system path to the asset this
			@ref RenderPipelineAsset is describing. */
		RenderPipelineAsset( const Utf8Char* const filePath );
	//!	Disable copy construction.
		RenderPipelineAsset( const RenderPipelineAsset& ) = delete;

		~RenderPipelineAsset() override = default;

	// ---------------------------------------------------

	public:
		const ArrayList<Attachment>&	GetAttachments() const;

		const ArrayList<Stage>&			GetStages() const;

	// ---------------------------------------------------

	public:
		ErrorCode	BindResources( const Builder& builder ) override;

		void		FreeResources() override;

	// ---------------------------------------------------

	//!	Disable copy assignment.
		RenderPipelineAsset&	operator=( const RenderPipelineAsset& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		ArrayList<Attachment>	_attachments;
		ArrayList<Stage>		_stages;
	};

}	// namespace AssetViews
}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/Vulkan/AssetViews/RenderPipelineAsset.inl>
//------------------------------------------------------------------//