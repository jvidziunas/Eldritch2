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
#include <Graphics/GraphicsPipelineApi.hpp>
//------------------------------------------------------------------//
#include <vulkan/vulkan_core.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	class PipelineAttachmentDescription {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref PipelineAttachmentDescription instance.
		PipelineAttachmentDescription(VkFormat format, uint32 mipLevels, bool persistent) ETNoexceptHint;
		//! Constructs this @ref PipelineAttachmentDescription instance.
		PipelineAttachmentDescription(const PipelineAttachmentDescription&) ETNoexceptHint = default;

		~PipelineAttachmentDescription() = default;

		// ---------------------------------------------------

	public:
		ETConstexpr VkAttachmentStoreOp GetStoreOp(uint32 thisPass) const ETNoexceptHint;

		ETConstexpr VkAttachmentLoadOp GetLoadOp(uint32 thisPass) const ETNoexceptHint;

		ETConstexpr bool IsUsed() const ETNoexceptHint;

		// ---------------------------------------------------

	public:
		bool TryMarkWritten(VkImageUsageFlags usage, uint32 pass, const PipelineFramebufferDescription& framebuffer) ETNoexceptHint;

		bool TryMarkRead(VkImageUsageFlags usage, uint32 pass, const PipelineFramebufferDescription& framebuffer) ETNoexceptHint;

		bool TryMarkUsed(VkImageUsageFlags usage, uint32 pass, const PipelineFramebufferDescription& framebuffer) ETNoexceptHint;

		// - DATA MEMBERS ------------------------------------

	public:
		VkFormat               format;
		uint32                 samplesPerPixel;
		VkImageCreateFlags     flags;
		VkImageUsageFlags      usages;
		VkImageViewCreateFlags viewFlags;
		uint32                 layers;
		uint32                 mipLevels;
		uint32                 firstRead;
		uint32                 lastRead;
		uint32                 firstWrite;
		uint32                 lastWrite;
		AttachmentDimensions   resolution;
	};

	// ---

	class PipelineBufferDescription {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref PipelineBufferDescription instance.
		PipelineBufferDescription(const PipelineBufferDescription&) ETNoexceptHint = default;
		//! Constructs this @ref PipelineBufferDescription instance.
		PipelineBufferDescription(VkDeviceSize byteSize) ETNoexceptHint;

		~PipelineBufferDescription() = default;

		// - DATA MEMBERS ------------------------------------

	public:
		VkDeviceSize        byteSize;
		VkBufferCreateFlags flags;
		VkBufferUsageFlags  usages;
		uint32              firstRead;
		uint32              lastRead;
		uint32              firstWrite;
		uint32              lastWrite;
	};

	// ---

	class GraphicsPipelineBuilder {
		// - TYPE PUBLISHING ---------------------------------

	public:
		struct BatchConstants {
			uint32 firstDescriptorBinding;
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
		Range<const PipelineAttachmentDescription*> GetAttachments() const ETNoexceptHint;

		Range<const PipelineBufferDescription*> GetBuffers() const ETNoexceptHint;

		Range<const PipelinePassDescription*> GetPasses() const ETNoexceptHint;

		// ---------------------------------------------------

	public:
		uint32 DefineAttachment(VkFormat format, uint32 mips, bool isPersistent);

		void BeginPass(PassType type, PassDimensions dimensions, uint32 samplesPerPixel, StringView name);

		bool AppendInput(uint32 globalId);

		bool AppendColorOutput(uint32 globalId);

		bool SetDepthStencilBuffer(uint32 globalId);

		// ---------------------------------------------------

	public:
		template <typename Pass, typename... AdditionalPasses>
		GraphicsPipelineBuilder& Optimize(Pass pass, AdditionalPasses... passes);
		GraphicsPipelineBuilder& Optimize();

		// ---------------------------------------------------

		//!	Disable copy construction.
		GraphicsPipelineBuilder& operator=(const GraphicsPipelineBuilder&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		ArrayList<PipelineAttachmentDescription> _attachments;
		ArrayList<PipelineBufferDescription>     _buffers;
		ArrayList<PipelinePassDescription>       _passes;

		// ---------------------------------------------------

		friend void Swap(GraphicsPipelineBuilder&, GraphicsPipelineBuilder&);
	};

}}} // namespace Eldritch2::Graphics::Vulkan

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/Vulkan/GraphicsPipelineBuilder.inl>
//------------------------------------------------------------------//
