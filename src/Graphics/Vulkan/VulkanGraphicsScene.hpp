/*==================================================================*\
  VulkanGraphicsScene.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/GraphicsPipeline.hpp>
#include <Graphics/Vulkan/BatchCoordinator.hpp>
#include <Graphics/Vulkan/DescriptorTable.hpp>
#include <Graphics/Vulkan/GpuResources.hpp>
#include <Graphics/Vulkan/OutputWindow.hpp>
#include <Graphics/Vulkan/CommandList.hpp>
#include <Graphics/ResolutionScale.hpp>
#include <Graphics/Vulkan/GpuHeap.hpp>
#include <Graphics/GraphicsScene.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Graphics {
		namespace Vulkan {
			class	Vulkan;
		}
	}
}

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {

	enum class PipelineType : uint32 {
		Lighting,
		ShadowMaps,

		COUNT
	};

// ---

	struct View {
	//	Set of planes to clip objects against.
		Vector		frustumPlanes[6];
		VkViewport	viewport;
		VkRect2D	scissorArea;
		uint8		id;
	};

// ---

	class PlayerView {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref PlayerView instance.
		PlayerView( CountedPointer<OutputWindow> target, Transformation localToWorld, Angle verticalFov );
	//!	Disable copy construction.
		PlayerView( const PlayerView& ) = delete;

		~PlayerView() = default;

	// ---------------------------------------------------

	public:
		Transformation ETSimdCall	GetLocalToWorld() const;

		void ETSimdCall				SetLocalToWorld( Transformation localToWorld );

	// ---------------------------------------------------

	public:
		Angle	GetVerticalFov() const;

		void	SetVerticalFov( Angle angle );

	// ---------------------------------------------------

	//!	Disable copy assignment.
		PlayerView&	operator=( const PlayerView& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		ResolutionScale					_scaler;
		Transformation					_localToWorld;
		Angle							_verticalFov;

		CountedPointer<OutputWindow>	_target;
	};

// ---

	class VulkanGraphicsScene : public GraphicsScene {
	// - TYPE PUBLISHING ---------------------------------

	public:
		enum : uint32		{ MaxQueuedFrames = 2u };
		enum : VkDeviceSize {
			DescriptorPoolSizeInElements =                 256u,
			DefaultTransformArenaSize    =  16u * 1024u * 1024u, /*  16MB */
			GpuHeapBlockSize             = 256u * 1024u * 1024u, /* 256MB */
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Disable copy construction.
		VulkanGraphicsScene( const VulkanGraphicsScene& ) = delete;
	//!	Constructs this @ref VulkanGraphicsScene instance.
		VulkanGraphicsScene();

		~VulkanGraphicsScene() = default;

	// ---------------------------------------------------

	public:
		VkResult	SubmitGpuCommands( Scheduling::JobExecutor& executor, Vulkan& vulkan );

	// ---------------------------------------------------

	public:
		VkResult	BindResources( Scheduling::JobExecutor& executor, Vulkan& vulkan, VkDeviceSize transformArenaSize = DefaultTransformArenaSize );

		void		FreeResources( Scheduling::JobExecutor& executor, Vulkan& vulkan );

	// ---------------------------------------------------

	//!	Disable copy assignment.
		VulkanGraphicsScene&	operator=( const VulkanGraphicsScene& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
	/*!	Heap for world-local resources. This should be used to create things like shader uniforms and pipeline-specific
		framebuffer attachments. */
		GpuHeap				_heap;
		UniformBuffer		_transforms;
		BatchCoordinator	_batchCoordinator;
	//	ShaderImage			_shadowAtlas;
		uint32				_frameId;

		struct Frame {
			DescriptorTable	descriptors;

			CommandList		commands;
			VkFence			commandsConsumed;

			GraphicsPipeline	pipelines[PipelineType::COUNT];
		}					_queuedFrames[MaxQueuedFrames];
	};

}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/Vulkan/VulkanGraphicsScene.inl>
//------------------------------------------------------------------//