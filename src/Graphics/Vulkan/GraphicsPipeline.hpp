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
#include <Graphics/Vulkan/BatchCoordinator.hpp>
#include <Graphics/Vulkan/DescriptorTable.hpp>
#include <Graphics/GraphicsPipelineApi.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {
	class PipelineAttachmentDescription;
}}} // namespace Eldritch2::Graphics::Vulkan

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	class GraphicsPipeline {
		// - TYPE PUBLISHING ---------------------------------

	public:
		class Pass {
			// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref Pass instance.
			Pass(const Pass&) ETNoexceptHint = default;
			//!	Constructs this @ref Pass instance.
			Pass() ETNoexceptHint;

			~Pass();

			// ---------------------------------------------------

		public:
			VkResult BindResources(Gpu& gpu, const PipelinePassDescription& pass, const PipelineAttachmentDescription globalAttachments[]);

			void FreeResources(Gpu& gpu);

			// - DATA MEMBERS ------------------------------------

		public:
			VkRenderPass renderPass;
		};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		GraphicsPipeline(const GraphicsPipeline&) = delete;
		//!	Constructs this @ref GraphicsPipeline instance.
		GraphicsPipeline(GraphicsPipeline&&);
		//!	Constructs this @ref GraphicsPipeline instance.
		GraphicsPipeline();

		~GraphicsPipeline() = default;

		// ---------------------------------------------------

	public:
		ETConstexpr const BatchCoordinator& GetBatches() const ETNoexceptHint;
		ETConstexpr BatchCoordinator& GetBatches() ETNoexceptHint;

		ETConstexpr const DescriptorTable& GetShaderResources() const ETNoexceptHint;
		ETConstexpr DescriptorTable& GetShaderResources() ETNoexceptHint;

		// ---------------------------------------------------

	public:
		VkResult BindResources(Gpu& gpu, const GraphicsPipelineBuilder& builder, size_t commandPoolCount);

		void FreeResources(Gpu& gpu);

		// ---------------------------------------------------

		//!	Disable copy assignment.
		GraphicsPipeline& operator=(const GraphicsPipeline&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		BatchCoordinator _batches;
		DescriptorTable  _shaderResources;

		// ---------------------------------------------------

		friend void Swap(GraphicsPipeline&, GraphicsPipeline&);
	};

}}} // namespace Eldritch2::Graphics::Vulkan

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/Vulkan/GraphicsPipeline.inl>
//------------------------------------------------------------------//
