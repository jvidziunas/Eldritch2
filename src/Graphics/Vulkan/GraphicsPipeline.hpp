/*==================================================================*\
  GraphicsPipeline.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/GraphicsScene.hpp>
//------------------------------------------------------------------//
#include <vk_mem_alloc.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {
	class GraphicsPipelineBuilder;
	class Gpu;
}}} // namespace Eldritch2::Graphics::Vulkan

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	class GraphicsPipeline {
		// - TYPE PUBLISHING ---------------------------------

	public:
		class Pass {
			// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref Pass instance.
			Pass(GeometryType source);
			//!	Constructs this @ref Pass instance.
			Pass(const Pass&) = default;

			~Pass();

			// ---------------------------------------------------

		public:
			VkResult BindResources(Gpu& gpu, const GraphicsPipelineBuilder& builder);

			void FreeResources(Gpu& gpu);

			// - DATA MEMBERS ------------------------------------

		public:
			VkRenderPass renderPass;
			GeometryType source;
		};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		GraphicsPipeline(const GraphicsPipeline&) = delete;
		//!	Constructs this @ref GraphicsPipeline instance.
		GraphicsPipeline(GraphicsPipeline&&);
		//!	Constructs this @ref GraphicsPipeline instance.
		GraphicsPipeline();

		~GraphicsPipeline();

		// ---------------------------------------------------

	public:
		const Pass& operator[](uint32 subpass) const;

		uint32 GetPassCount() const;

		// ---------------------------------------------------

	public:
		VkDescriptorSetLayout GetDescriptorLayout() const;

		VkQueryPool GetTimingPool() const;

		// ---------------------------------------------------

	public:
		VkResult BindResources(Gpu& gpu, const GraphicsPipelineBuilder& builder);

		void FreeResources(Gpu& gpu);

		// ---------------------------------------------------

		//!	Disable copy assignment.
		GraphicsPipeline& operator=(const GraphicsPipeline&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		VkDescriptorSetLayout _descriptorLayout;
		VkQueryPool           _timingPool;
		ArrayList<Pass>       _passes;

		// ---------------------------------------------------

		friend void Swap(GraphicsPipeline&, GraphicsPipeline&);
	};

	// ---

	class Framebuffer {
		// - TYPE PUBLISHING ---------------------------------

	public:
		class Attachment {
			// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Disable copy construction.
			Attachment(const Attachment&) = delete;
			//! Constructs this @ref Attachment instance.
			Attachment(Attachment&&);
			//! Constructs this @ref Attachment instance.
			Attachment();

			~Attachment();

			// ---------------------------------------------------

		public:
			VkResult BindResources(Gpu& gpu, VkFormat format, VkImageCreateFlags flags, VkImageUsageFlags usage, VkSampleCountFlags msaa, VkExtent3D extent, uint32_t arrayLayers);
			VkResult BindResources(Gpu& gpu, VkFormat format, VkImageViewCreateFlags flags, VkImage image, VkImageViewType type);

			void FreeResources(Gpu& gpu);

			// - DATA MEMBERS ------------------------------------

		public:
			VkImage     image;
			VkImageView view;

			// ---------------------------------------------------

			friend void Swap(Attachment&, Attachment&);
		};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		Framebuffer(const Framebuffer&) = delete;
		//!	Constructs this @ref Framebuffer instance.
		Framebuffer(Framebuffer&&);
		//!	Constructs this @ref Framebuffer instance.
		Framebuffer();

		~Framebuffer();

		// ---------------------------------------------------

	public:
		VkFramebuffer operator[](uint32_t pass) const;

		// ---------------------------------------------------

	public:
		VkResult BindResources(Gpu& gpu, const GraphicsPipeline& pipeline, const GraphicsPipelineBuilder& pipelineSource, VkExtent2D baseDimensions, uint32 arrayLayers = 1u);

		void FreeResources(Gpu& gpu);

		// ---------------------------------------------------

		//!	Disable copy assignment.
		Framebuffer& operator=(const Framebuffer&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		ArrayList<Attachment>    _combinedAttachments;
		ArrayList<VmaAllocation> _allocations;
		ArrayList<VkFramebuffer> _framebufferByPass;

		// ---------------------------------------------------

		friend void Swap(Framebuffer&, Framebuffer&);
	};

}}} // namespace Eldritch2::Graphics::Vulkan

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/Vulkan/GraphicsPipeline.inl>
//------------------------------------------------------------------//
