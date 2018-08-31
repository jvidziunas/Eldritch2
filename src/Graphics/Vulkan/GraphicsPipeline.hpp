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
#include <Graphics/ResolutionScale.hpp>
#include <Graphics/GraphicsScene.hpp>
//------------------------------------------------------------------//
#include <vk_mem_alloc.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {
	namespace AssetViews {
		class SpirVShaderSetAsset;
	}
	class GraphicsPipelineBuilder;
	class GraphicsPipeline;
	class Gpu;
}}} // namespace Eldritch2::Graphics::Vulkan

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	class ShaderPipeline {
		// - TYPE PUBLISHING ---------------------------------

	public:
		class Usage {
			// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Disable copy construction.
			Usage(const Usage&) = delete;
			//!	Constructs this @ref Usage instance.
			Usage(Usage&&) ETNoexceptHint;
			//!	Constructs this @ref Usage instance.
			Usage() ETNoexceptHint;

			~Usage();

			// ---------------------------------------------------

		public:
			VkResult BindResources(Gpu& gpu, const AssetViews::SpirVShaderSetAsset& set, uint32 usageIndex);

			void FreeResources(Gpu& gpu);

			// ---------------------------------------------------

			//!	Disable copy assignment.
			Usage& operator=(const Usage&) = delete;

			// - DATA MEMBERS ------------------------------------

		public:
			VkShaderModule shaders[5];

			// ---------------------------------------------------

			friend void Swap(Usage&, Usage&) ETNoexceptHint;
		};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		ShaderPipeline(const ShaderPipeline&) = delete;
		//!	Constructs this @ref ShaderPipeline instance.
		ShaderPipeline(ShaderPipeline&&);
		//!	Constructs this @ref ShaderPipeline instance.
		ShaderPipeline();

		~ShaderPipeline();

		// ---------------------------------------------------

	public:
		VkResult BindResources(Gpu& gpu, GraphicsPipeline& context, const AssetViews::SpirVShaderSetAsset& asset);

		void FreeResources(Gpu& gpu);

		// ---------------------------------------------------

		//!	Disable copy assignment.
		ShaderPipeline& operator=(const ShaderPipeline&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		VkPipelineLayout      _pipelineLayout;
		ArrayList<VkPipeline> _pipelinesByPass; // To consider: can we flatten this across pipelines?
		ArrayList<Usage>      _usagesByPass;

		// ---------------------------------------------------

		friend void Swap(ShaderPipeline&, ShaderPipeline&);
	};

	// ---

	class GraphicsPipeline {
		// - TYPE PUBLISHING ---------------------------------

	public:
		class AttachmentDescription {
			// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref AttachmentDescription instance.
			AttachmentDescription(const GraphicsPipelineBuilder::AttachmentDescription&) ETNoexceptHint;
			//!	Constructs this @ref AttachmentDescription instance.
			AttachmentDescription(const AttachmentDescription&) ETNoexceptHint = default;
			//!	Constructs this @ref AttachmentDescription instance.
			AttachmentDescription() ETNoexceptHint;

			~AttachmentDescription() = default;

			// ---------------------------------------------------

		public:
			VkImageViewCreateInfo GetImageViewCreateInfo(VkImage image, uint32 mip, void* extensions = nullptr) const ETNoexceptHint;

			VkImageCreateInfo GetImageCreateInfo(VkExtent2D base, uint32 layers, void* extensions = nullptr) const ETNoexceptHint;

			// ---------------------------------------------------

		private:
			VkImageViewType GetViewType() const ETNoexceptHint;

			VkImageType GetType() const ETNoexceptHint;

			VkExtent3D GetResolution(VkExtent2D baseResolution) const ETNoexceptHint;

			uint32 GetLayers(uint32 baseLayers) const ETNoexceptHint;

			// - DATA MEMBERS ------------------------------------

		private:
			VkFormat           _format;
			VkImageCreateFlags _flags;
			VkImageUsageFlags  _usages;
			VkSampleCountFlags _sampleRates;
			uint32             _arraySize;
			bool               _staticResolution;
			union {
				VkExtent3D        _resolution;
				GraphicsPassScale _scale;
			};
		};

		// ---

	public:
		class PassDescription {
			// - TYPE PUBLISHING ---------------------------------

		public:
			enum : uint32 {
				MaxAttachments      = 4u,
				MaxTotalAttachments = MaxAttachments + MaxAttachments + 1u
			};

			// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref PassDescription instance.
			PassDescription(GeometryType source = GeometryType::StaticMeshes) ETNoexceptHint;
			//!	Constructs this @ref PassDescription instance.
			PassDescription(const PassDescription&) ETNoexceptHint = default;

			~PassDescription();

			// ---------------------------------------------------

		public:
			VkResult BindResources(Gpu& gpu, const GraphicsPipelineBuilder& pipeline, uint32 passIndex);

			void FreeResources(Gpu& gpu);

			// - DATA MEMBERS ------------------------------------

		public:
			Utf8Char     name[64u];
			GeometryType source;
			VkRenderPass renderPass;
			float32      maxScale;
			float32      minScale;
			uint32       colorAttachments[MaxAttachments];
			uint32       inputAttachments[MaxAttachments];
			uint32       depthStencilAttachment;
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
		const PassDescription* GetPasses() const ETNoexceptHint;

		uint32 GetPassCount() const ETNoexceptHint;

		const AttachmentDescription* GetFramebufferAttachments() const ETNoexceptHint;

		uint32 GetFramebufferAttachmentCount() const ETNoexceptHint;

		VkDescriptorSetLayout GetDescriptorLayout() const ETNoexceptHint;

		// ---------------------------------------------------

	public:
		VkResult BindResources(Gpu& gpu, const GraphicsPipelineBuilder& builder);

		void FreeResources(Gpu& gpu);

		// ---------------------------------------------------

		//!	Disable copy assignment.
		GraphicsPipeline& operator=(const GraphicsPipeline&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		VkDescriptorSetLayout            _descriptorLayout;
		ArrayList<AttachmentDescription> _attachments;
		ArrayList<PassDescription>       _passes;

		// ---------------------------------------------------

		friend void Swap(GraphicsPipeline&, GraphicsPipeline&);
	};

	// ---

	class Framebuffer {
		// - TYPE PUBLISHING ---------------------------------

	public:
		using ExternalImageMap = ArrayMap<uint32 /*attachmentIndex*/, VkImage /*image*/>;

		// ---

		class Attachment {
			// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Disable copy construction.
			Attachment(const Attachment&) = delete;
			//! Constructs this @ref Attachment instance.
			Attachment(Attachment&&) ETNoexceptHint;
			//! Constructs this @ref Attachment instance.
			Attachment() ETNoexceptHint;

			~Attachment();

			// ---------------------------------------------------

		public:
			bool OwnsImage() const ETNoexceptHint;

			// ---------------------------------------------------

		public:
			VkResult BindResources(Gpu& gpu, const GraphicsPipeline& pipeline, uint32 attachmentIndex, VkExtent2D baseDimensions, uint32 layers);
			VkResult BindResources(Gpu& gpu, const GraphicsPipeline& pipeline, uint32 attachmentIndex, VkImage image);

			void FreeResources(Gpu& gpu);

			// - DATA MEMBERS ------------------------------------

		public:
			VkImage     image;
			VkImageView view;

			// ---------------------------------------------------

			friend void Swap(Attachment&, Attachment&) ETNoexceptHint;
		};

		// ---

	public:
		class Pass {
			// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Disable copy construction.
			Pass(const Pass&) = delete;
			//!	Constructs this @ref Pass instance.
			Pass(Pass&&) ETNoexceptHint;
			//!	Constructs this @ref Pass instance.
			Pass() ETNoexceptHint;

			~Pass();

			// ---------------------------------------------------

		public:
			VkRect2D GetRenderArea() const ETNoexceptHint;

			// ---------------------------------------------------

		public:
			VkResult BindResources(Gpu& gpu, const GraphicsPipeline& pipeline, const ArrayList<Attachment>& attachments, uint32 passIndex);

			void FreeResources(Gpu& gpu);

			// - DATA MEMBERS ------------------------------------

		public:
			ResolutionScale scaler;
			VkRenderPass    nativeRenderPass;
			VkFramebuffer   nativeFramebuffer;
			VkExtent2D      granularity;
			VkRect2D        nativeArea;

			// ---------------------------------------------------

			friend void Swap(Pass&, Pass&) ETNoexceptHint;
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
		const Pass* GetPasses() const ETNoexceptHint;

		uint32 GetPassCount() const ETNoexceptHint;

		VkQueryPool GetTimingPool() const ETNoexceptHint;

		// ---------------------------------------------------

	public:
		VkResult UpdateDynamicScaling(Gpu& gpu);

		// ---------------------------------------------------

	public:
		VkResult BindResources(Gpu& gpu, const GraphicsPipeline& pipeline, VkExtent2D dimensions, uint32 layers, ExternalImageMap externalImages = ExternalImageMap {});

		void FreeResources(Gpu& gpu);

		// ---------------------------------------------------

		//!	Disable copy assignment.
		Framebuffer& operator=(const Framebuffer&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		VkQueryPool              _timingPool;
		ArrayList<Attachment>    _attachments;
		ArrayList<VmaAllocation> _allocations;
		ArrayList<Pass>          _passes;

		// ---------------------------------------------------

		friend void Swap(Framebuffer&, Framebuffer&) ETNoexceptHint;
	};

	// ---

	class Viewport : public Framebuffer {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		Viewport(const Viewport&) = delete;
		//!	Construct this @ref Viewport instance.
		Viewport(Viewport&&) ETNoexceptHint;
		//!	Construct this @ref Viewport instance.
		Viewport();

		~Viewport() = default;

		// ---------------------------------------------------

	public:
		VkResult Resize(Gpu& gpu, VkExtent2D dimensions, uint32 layers, ExternalImageMap externalImages = ExternalImageMap {});

		// ---------------------------------------------------

	public:
		bool TryAcquire(const GraphicsPipeline& generator) ETNoexceptHint;

		void Release() ETNoexceptHint;

		// ---------------------------------------------------

	private:
		Atomic<const GraphicsPipeline*> _generator;

		// ---------------------------------------------------

		friend void Swap(Viewport&, Viewport&) ETNoexceptHint;

		friend struct ViewportDisposer;
	};

	// ---

	struct ViewportDisposer {
		ETPureFunctionHint void operator()(Viewport* viewport) const ETNoexceptHint;
	};

}}} // namespace Eldritch2::Graphics::Vulkan

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/Vulkan/GraphicsPipeline.inl>
//------------------------------------------------------------------//
