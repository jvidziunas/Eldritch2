/*==================================================================*\
  PipelineBatcher.hpp
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
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {
	class SpirVShaderSet;
}}} // namespace Eldritch2::Graphics::Vulkan

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	class PipelineBatcher {
		// - TYPE PUBLISHING ---------------------------------

	public:
		enum : size_t {
			PassNames,
			RenderPasses,
		};

		// ---

		class CommandPool {
			// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//! Disable copy construction.
			CommandPool(const CommandPool&) = delete;
			//!	Constructs this @ref CommandPool instance.
			CommandPool(CommandPool&&) ETNoexceptHint;
			//!	Constructs this @ref CommandPool instance.
			CommandPool() ETNoexceptHint;

			~CommandPool() = default;

			// ---------------------------------------------------

		public:
			VkResult BindResources(Gpu& gpu);

			void FreeResources(Gpu& gpu) ETNoexceptHint;

			// - DATA MEMBERS ------------------------------------

		public:
			ETCacheLineAligned Mutex poolMutex;
			VkCommandPool            pool;
		};

		// ---

		using StageList                 = SoaList<String, VkRenderPass>;
		using PipelineCommandBufferList = SoaList<VkPipeline, VkCommandBuffer>;
		using CommandPoolList           = ArrayList<CommandPool>;
		using StageIndex                = typename StageList::SizeType;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		PipelineBatcher(const PipelineBatcher&) = delete;
		//!	Constructs this @ref PipelineBatcher instance.
		PipelineBatcher(PipelineBatcher&&) ETNoexceptHint;
		//!	Constructs this @ref PipelineBatcher instance.
		PipelineBatcher() ETNoexceptHint;

		~PipelineBatcher() = default;

		// ---------------------------------------------------

	public:
		VkResult Compile(Gpu& gpu, const SpirVShaderSet& shaders, uint32 constantsByteSize, const byte constants[]);
		template <typename Constants>
		VkResult Compile(Gpu& gpu, const SpirVShaderSet& shaders, const Constants& constants);

		// ---------------------------------------------------

	public:
		VkResult BindResources(Gpu& gpu, Span<const byte*> cacheSeed, const PipelineBuilder& pipeline, CommandPoolList::SizeType poolCount);

		void FreeResources(Gpu& gpu) ETNoexceptHint;

		// ---------------------------------------------------

	private:
		//!	Disable copy assignment.
		PipelineBatcher& operator=(const PipelineBatcher&) = delete;

		// ---------------------------------------------------

	private:
		CommandPool& FindPool(VkPipeline pipeline) ETNoexceptHint;

		// - DATA MEMBERS ------------------------------------

	private:
		VkPipelineCache           _cache;
		VkPipelineLayout          _layout;
		StageList                 _stages;
		CommandPoolList           _commandPools;
		PipelineCommandBufferList _pipelineCommands;

		// ---------------------------------------------------

		friend void Swap(PipelineBatcher&, PipelineBatcher&) ETNoexceptHint;
	};

}}} // namespace Eldritch2::Graphics::Vulkan

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/Vulkan/PipelineBatcher.inl>
//------------------------------------------------------------------//
