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
#include <Graphics/Vulkan/PipelineBatcher.hpp>
#include <Graphics/PipelineBuilder.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	class GraphicsPipeline {
		// - TYPE PUBLISHING ---------------------------------

	public:
		class Stage {
			// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref Stage instance.
			Stage(const Stage&) ETNoexceptHint = default;
			//!	Constructs this @ref Stage instance.
			Stage() ETNoexceptHint;

			~Stage();

			// ---------------------------------------------------

		public:
			VkResult BindResources(Gpu& gpu, const StageBuilder& stage, const VkAttachmentDescription attachments[]);

			void FreeResources(Gpu& gpu) ETNoexceptHint;

			// - DATA MEMBERS ------------------------------------

		public:
			VkRenderPass renderPass;
		};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		GraphicsPipeline(const GraphicsPipeline&) = delete;
		//!	Constructs this @ref GraphicsPipeline instance.
		GraphicsPipeline(GraphicsPipeline&&) ETNoexceptHint;
		//!	Constructs this @ref GraphicsPipeline instance.
		GraphicsPipeline() ETNoexceptHint = default;

		~GraphicsPipeline() = default;

		// ---------------------------------------------------

	public:
		ETConstexpr const DescriptorTable& GetShaderResources() const ETNoexceptHint;

		ETConstexpr const PipelineBatcher& GetBatches() const ETNoexceptHint;

		// ---------------------------------------------------

	public:
		VkResult BindResourceViews(Gpu& gpu);

		VkResult PushToGpu(Gpu& gpu);

		// ---------------------------------------------------

	public:
		VkResult BindResources(Gpu& gpu, const PipelineBuilder& builder, PipelineBatcher::CommandPoolList::SizeType poolCount);

		void FreeResources(Gpu& gpu) ETNoexceptHint;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		GraphicsPipeline& operator=(const GraphicsPipeline&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		PipelineBatcher _batches;
		DescriptorTable _shaderResources;

		// ---------------------------------------------------

		friend void Swap(GraphicsPipeline&, GraphicsPipeline&) ETNoexceptHint;
	};

}}} // namespace Eldritch2::Graphics::Vulkan

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/Vulkan/GraphicsPipeline.inl>
//------------------------------------------------------------------//
