/*==================================================================*\
  GraphicsPipelineBuilder.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <vulkan/vulkan_core.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	class GraphicsPipelineBuilder {
		// - TYPE PUBLISHING ---------------------------------

	public:
		struct AttachmentReference {
			uint32        index;
			VkImageLayout layout;
		};

		// ---

	public:
		class Attachment {
			// - CONSTRUCTOR/DESTRUCTOR --------------------------
		public:
			//! Constructs this @ref Attachment instance.
			Attachment(VkFormat format, VkSampleCountFlags quality, float32 widthScale = 1.0f, float32 heightScale = 1.0f, float32 depthScale = 1.0f);
			//! Constructs this @ref Attachment instance.
			Attachment(VkFormat format, VkSampleCountFlags quality, VkExtent3D dimensions);
			//! Constructs this @ref Attachment instance.
			Attachment(const Attachment&) = default;

			~Attachment() = default;

			// ---------------------------------------------------

		public:
			bool ShouldPreserveInPass(uint32 pass) const;

			bool IsReferenced() const;

			void MarkUsed(uint32 pass, VkImageUsageFlags usage);

			// - DATA MEMBERS ------------------------------------

		public:
			VkFormat           format;
			VkImageCreateFlags flags;
			VkImageUsageFlags  usages;
			uint32             firstPass : 10;
			uint32             lastPass : 10;
			uint32             msaaQuality : 8;
			uint32             staticResolution : 1;
			union {
				float32    scale[3];
				VkExtent3D dimensions;
			};
		};

		// ---

	public:
		class Pass {
			// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref Pass instance.
			Pass(const Utf8Char* const name);
			//!	Constructs this @ref Pass instance.
			Pass(const Pass&) = default;

			~Pass() = default;

			// - DATA MEMBERS ------------------------------------

		public:
			Utf8Char            name[64u];
			uint32              attachmentCount;
			uint32              inputAttachmentCount;
			AttachmentReference attachments[4u];
			AttachmentReference inputAttachments[4u];
			AttachmentReference depthStencilAttachment;
		};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		GraphicsPipelineBuilder(const GraphicsPipelineBuilder&) = delete;
		//!	Constructs this @ref GraphicsPipelineBuilder instance.
		GraphicsPipelineBuilder(GraphicsPipelineBuilder&&);
		//!	Constructs this @ref GraphicsPipelineBuilder instance.
		GraphicsPipelineBuilder();

		~GraphicsPipelineBuilder() = default;

		// ---------------------------------------------------

	public:
		const Pass& operator[](uint32 pass) const;

		uint32 GetPassCount() const;

		// ---------------------------------------------------

	public:
		const Attachment& GetAttachment(uint32 attachment) const;

		uint32 GetAttachmentCount() const;

		// ---------------------------------------------------

	public:
		template <typename... Args>
		void DefineAttachment(VkFormat format, VkSampleCountFlags quality, VkImageLayout initial, VkImageLayout final, Args&&... args);

		void Begin(VkPipelineBindPoint bindPoint, const Utf8Char* name);

		void BeginPass(VkPipelineBindPoint bindPoint, const Utf8Char* name);

		bool AttachInput(uint32 index, VkImageLayout layout);

		bool AttachColorOutput(uint32 index, VkImageLayout layout);

		bool AttachDepthStencilBuffer(uint32 index, VkImageLayout layout);

		void Finish();

		// ---------------------------------------------------

		//!	Disable copy construction.
		GraphicsPipelineBuilder& operator=(const GraphicsPipelineBuilder&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		ArrayList<Attachment> _attachments;
		ArrayList<Pass>       _passes;

		// ---------------------------------------------------

		friend void Swap(GraphicsPipelineBuilder&, GraphicsPipelineBuilder&);
	};

}}} // namespace Eldritch2::Graphics::Vulkan

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/Vulkan/GraphicsPipelineBuilder.inl>
//------------------------------------------------------------------//
