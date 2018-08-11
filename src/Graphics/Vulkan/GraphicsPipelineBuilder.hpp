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
			AttachmentDescription(VkFormat format, VkSampleCountFlags quality);
			//! Constructs this @ref AttachmentDescription instance.
			AttachmentDescription(const AttachmentDescription&) = default;

			~AttachmentDescription() = default;

			// ---------------------------------------------------

		public:
			bool SupportsResolution(float32 widthScale, float32 heightScale) const;
			bool SupportsResolution(VkExtent3D dimensions) const;

			// ---------------------------------------------------

		public:
			bool ShouldPreserveInPass(uint32 pass) const;

			bool IsReferenced() const;

			bool IsWritten() const;

			bool IsRead() const;

			// ---------------------------------------------------

		public:
			void MarkWritten(uint32 pass, VkImageUsageFlags usage);

			void MarkRead(uint32 pass, VkImageUsageFlags usage);

			void MarkUsed(uint32 pass, VkImageUsageFlags usage);

			// - DATA MEMBERS ------------------------------------

		public:
			VkFormat           format;
			VkImageCreateFlags flags;
			VkImageUsageFlags  usages;
			uint32             firstRead;
			uint32             lastRead;
			uint32             firstWrite;
			uint32             lastWrite;
			uint32             msaaQuality : 8;
			uint32             isExport : 1;
			uint32             isPersistent : 1;
			uint32             staticDimensions : 1;
			union {
				VkExtent3D dimensions;
				float32    scales[2];
			};
		};

		// ---

	public:
		class BufferDescription {
			// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//! Constructs this @ref BufferDescription instance.
			BufferDescription(VkDeviceSize sizeInBytes);
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
			PassDescription(float32 widthScale, float32 heightScale, const Utf8Char* const name);
			//!	Constructs this @ref PassDescription instance.
			PassDescription(VkExtent3D dimensions, const Utf8Char* const name);
			//!	Constructs this @ref PassDescription instance.
			PassDescription(const PassDescription&) = default;

			~PassDescription() = default;

			// - DATA MEMBERS ------------------------------------

		public:
			Utf8Char name[64u];
			uint32   staticDimensions : 1;
			union {
				VkExtent3D dimensions;
				struct {
					float32 widthScale;
					float32 heightScale;
				};
			};
			AttachmentReference colorAttachments[MaxAttachmentsPerPass];
			AttachmentReference inputAttachments[MaxAttachmentsPerPass];
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
		const PassDescription& operator[](uint32 pass) const;

		uint32 GetPassCount() const;

		// ---------------------------------------------------

	public:
		const AttachmentDescription& GetAttachment(uint32 attachment) const;

		uint32 GetAttachmentCount() const;

		// ---------------------------------------------------

	public:
		template <typename... Args, class = eastl::enable_if_t<eastl::is_constructible<AttachmentDescription, VkFormat, VkSampleCountFlags, Args...>::value>>
		uint32 DefineAttachment(VkFormat format, VkSampleCountFlags quality, Args&&... args);

		void BeginPass(VkPipelineBindPoint bindPoint, float32 width, float32 height, const Utf8Char* name);
		void BeginPass(VkPipelineBindPoint bindPoint, VkExtent3D resolution, const Utf8Char* name);

		bool AppendInput(uint32 attachment, VkImageLayout layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

		bool AppendColorOutput(uint32 attachment, VkImageLayout layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);

		bool SetDepthStencilBuffer(uint32 attachment, VkImageLayout layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

		void Finish(bool andOptimize = true);

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
