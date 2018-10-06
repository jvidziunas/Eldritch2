/*==================================================================*\
  BatchCoordinator.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/GpuResourceApi.hpp>
#include <Graphics/Vulkan/SpirVShaderSet.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {
	class GraphicsPipelineBuilder;
}}} // namespace Eldritch2::Graphics::Vulkan

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	class BatchCoordinator {
		// - TYPE PUBLISHING ---------------------------------

	public:
		class CommandPool {
			// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref CommandPool instance.
			CommandPool(CommandPool&&) ETNoexceptHint;
			//! Disable copy construction.
			CommandPool(const CommandPool&) = delete;
			//!	Constructs this @ref CommandPool instance.
			CommandPool() ETNoexceptHint;

			~CommandPool() = default;

			// ---------------------------------------------------

		public:
			VkResult BindResources(Gpu& gpu);

			void FreeResources(Gpu& gpu);

			// - DATA MEMBERS ------------------------------------

		public:
			Mutex         poolMutex;
			VkCommandPool pool;
		};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		BatchCoordinator(const BatchCoordinator&) = delete;
		//!	Constructs this @ref BatchCoordinator instance.
		BatchCoordinator(BatchCoordinator&&);
		//!	Constructs this @ref BatchCoordinator instance.
		BatchCoordinator();

		~BatchCoordinator() = default;

		// ---------------------------------------------------

	public:
		bool Contains(VkPipeline pipeline) const ETNoexceptHint;

		// ---------------------------------------------------

	public:
		VkResult BindShaderSet(Gpu& gpu, const SpirVShaderSet& shaders, const void* batchConstants, uint32 constantsByteSize);
		template <typename Constants>
		VkResult BindShaderSet(Gpu& gpu, const SpirVShaderSet& shaders, const Constants& constants);

		// ---------------------------------------------------

	public:
		VkResult BindResources(Gpu& gpu, const GraphicsPipelineBuilder& pipeline, size_t commandPoolCount);

		void FreeResources(Gpu& gpu);

		// ---------------------------------------------------

		//!	Disable copy assignment.
		BatchCoordinator& operator=(const BatchCoordinator&) = delete;

		// ---------------------------------------------------

	private:
		CommandPool& FindPool(VkPipeline pipeline) ETNoexceptHint;

		// - DATA MEMBERS ------------------------------------

	private:
		VkPipelineLayout                         _layout;
		UniformBuffer                            _drawParameters;
		ArrayList<VkRenderPass>                  _renderPasses;
		ArrayList<CommandPool>                   _commandPools;
		SoArrayList<VkPipeline, VkCommandBuffer> _commandsByPipeline;

		// ---------------------------------------------------

		friend void Swap(BatchCoordinator&, BatchCoordinator&);
	};

}}} // namespace Eldritch2::Graphics::Vulkan

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/Vulkan/BatchCoordinator.inl>
//------------------------------------------------------------------//
