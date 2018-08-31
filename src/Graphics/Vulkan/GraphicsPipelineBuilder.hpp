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

	struct GraphicsPassScale {
		float32 width;
		float32 height;
	};

	// ---

	class GraphicsPipelineBuilder {
		// - TYPE PUBLISHING ---------------------------------

	public:
		enum : uint32 {
			MaxAttachmentsPerPass = 4u
		};

		// ---

	public:
		struct AttachmentReference {
			uint32        globalIndex;
			VkImageLayout layout;
		};

		// ---

	public:
		class AttachmentDescription {
			// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//! Constructs this @ref AttachmentDescription instance.
			AttachmentDescription(const AttachmentDescription&) = default;
			//! Constructs this @ref AttachmentDescription instance.
			AttachmentDescription(VkFormat format) ETNoexceptHint;

			~AttachmentDescription() = default;

			// ---------------------------------------------------

		public:
			bool SupportsResolution(GraphicsPassScale scale) const ETNoexceptHint;
			bool SupportsResolution(VkExtent3D resolution) const ETNoexceptHint;

			// ---------------------------------------------------

		public:
			bool ShouldPreserve(uint32 thisPass) const ETNoexceptHint;

			bool IsWritten() const ETNoexceptHint;

			bool IsRead() const ETNoexceptHint;

			bool IsUsed() const ETNoexceptHint;

			// ---------------------------------------------------

		public:
			void MarkWritten(uint32 pass, VkSampleCountFlagBits sampleRate, VkImageUsageFlags usage) ETNoexceptHint;

			void MarkRead(uint32 pass, VkSampleCountFlagBits sampleRate, VkImageUsageFlags usage) ETNoexceptHint;

			void MarkUsed(uint32 pass, VkSampleCountFlagBits sampleRate, VkImageUsageFlags usage) ETNoexceptHint;

			// - DATA MEMBERS ------------------------------------

		public:
			VkFormat           format;
			VkSampleCountFlags sampleRates;
			VkImageCreateFlags flags;
			VkImageUsageFlags  usages;
			uint32             firstRead;
			uint32             lastRead;
			uint32             firstWrite;
			uint32             lastWrite;
			uint32             isExport : 1;
			uint32             isPersistent : 1;
			uint32             staticResolution : 1;
			union {
				VkExtent3D        resolution;
				GraphicsPassScale scale;
			};
		};

		// ---

	public:
		class BufferDescription {
			// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//! Constructs this @ref BufferDescription instance.
			BufferDescription(VkDeviceSize sizeInBytes) ETNoexceptHint;
			//! Constructs this @ref BufferDescription instance.
			BufferDescription(const BufferDescription&) = default;

			~BufferDescription() = default;

			// - DATA MEMBERS ------------------------------------

		public:
			VkDeviceSize        sizeInBytes;
			VkBufferCreateFlags flags;
			VkBufferUsageFlags  usages;
			uint32              firstRead;
			uint32              lastRead;
			uint32              firstWrite;
			uint32              lastWrite;
		};

		// ---

	public:
		class PassDescription {
			// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref PassDescription instance.
			PassDescription(GraphicsPassScale scale, VkSampleCountFlagBits samples, StringView name) ETNoexceptHint;
			//!	Constructs this @ref PassDescription instance.
			PassDescription(VkExtent3D resolution, VkSampleCountFlagBits samples, StringView name) ETNoexceptHint;
			//!	Constructs this @ref PassDescription instance.
			PassDescription(const PassDescription&) ETNoexceptHint = default;

			~PassDescription() = default;

			// - DATA MEMBERS ------------------------------------

		public:
			Utf8Char              name[64u];
			AttachmentReference   colorAttachments[MaxAttachmentsPerPass];
			AttachmentReference   inputAttachments[MaxAttachmentsPerPass];
			AttachmentReference   depthStencilAttachment;
			VkSampleCountFlagBits samples;
			uint32                staticResolution : 1;
			union {
				VkExtent3D        resolution;
				GraphicsPassScale scale;
			};
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
		const PassDescription& operator[](uint32 pass) const ETNoexceptHint;

		uint32 GetPassCount() const ETNoexceptHint;

		// ---------------------------------------------------

	public:
		const AttachmentDescription& GetAttachment(uint32 attachment) const ETNoexceptHint;

		uint32 GetAttachmentCount() const ETNoexceptHint;

		// ---------------------------------------------------

	public:
		uint32 DefineAttachment(VkFormat format);

		void BeginPass(VkPipelineBindPoint target, VkExtent3D resolution, VkSampleCountFlagBits sampleRate, StringView name);
		void BeginPass(VkPipelineBindPoint target, GraphicsPassScale scale, VkSampleCountFlagBits sampleRate, StringView name);

		bool AppendInput(uint32 globalId, VkImageLayout layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

		bool AppendColorOutput(uint32 globalId, VkImageLayout layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);

		bool SetDepthStencilBuffer(uint32 globalId, VkImageLayout layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

		VkResult Finish(bool andOptimize = true);

		// ---------------------------------------------------

		//!	Disable copy construction.
		GraphicsPipelineBuilder& operator=(const GraphicsPipelineBuilder&) = delete;

		// ---------------------------------------------------

	private:
		void StripUnusedResources();

		// - DATA MEMBERS ------------------------------------

	private:
		ArrayList<AttachmentDescription> _attachments;
		ArrayList<BufferDescription>     _buffers;
		ArrayList<PassDescription>       _passes;

		// ---------------------------------------------------

		friend void Swap(GraphicsPipelineBuilder&, GraphicsPipelineBuilder&);
	};

}}} // namespace Eldritch2::Graphics::Vulkan

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/Vulkan/GraphicsPipelineBuilder.inl>
//------------------------------------------------------------------//
