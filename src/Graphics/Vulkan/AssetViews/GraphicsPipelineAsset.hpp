/*==================================================================*\
  GraphicsPipelineAsset.hpp
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

	class GraphicsPipelineAsset : public Assets::Asset {
	// - TYPE PUBLISHING ---------------------------------

	public:
		enum : size_t {
			MaxInputAttachmentsPerPass	= 8u,
			MaxOutputAttachmentsPerPass	= 8u,
			MaxAttachmentsPerPass		= (MaxInputAttachmentsPerPass + MaxOutputAttachmentsPerPass + 1)
		};

	// ---

	public:
		struct AttachmentAsset {
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
		class StageAsset {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
		//!	Constructs this @ref StageAsset instance.
			StageAsset( const Utf8Char* const shaderUsageName );
		//!	Constructs this @ref StageAsset instance.
			StageAsset( const StageAsset& ) = default;

			~StageAsset() = default;

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
	//!	Constructs this @ref GraphicsPipelineAsset instance.
	/*!	@param[in] filePath Null-terminated, UTF-8-encoded character sequence containing the file system path to the asset this
			@ref GraphicsPipelineAsset is describing. */
		GraphicsPipelineAsset( const Utf8Char* const filePath );
	//!	Disable copy construction.
		GraphicsPipelineAsset( const GraphicsPipelineAsset& ) = delete;

		~GraphicsPipelineAsset() override = default;

	// ---------------------------------------------------

	public:
		const ArrayList<AttachmentAsset>&	GetAttachments() const;

		const ArrayList<StageAsset>&		GetStages() const;

	// ---------------------------------------------------

	public:
		ErrorCode	BindResources( const Builder& asset ) override;

		void		FreeResources() override;

	// ---------------------------------------------------

	//!	Disable copy assignment.
		GraphicsPipelineAsset&	operator=( const GraphicsPipelineAsset& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		ArrayList<AttachmentAsset>	_attachments;
		ArrayList<StageAsset>		_stages;
	};

}	// namespace AssetViews
}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/Vulkan/AssetViews/GraphicsPipelineAsset.inl>
//------------------------------------------------------------------//