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
#include <Graphics/Vulkan/GpuResources.hpp>
//------------------------------------------------------------------//
#include <vulkan/vulkan_core.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Graphics {
		namespace Vulkan {
			class	CommandList;
		}
	}
}

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {

	class BatchCoordinator {
	// - TYPE PUBLISHING ---------------------------------

	public:
		enum : VkDeviceSize {
			InstanceBufferSize  = 2u * 1024u * 1024u, /*  2MB */
			ParameterBufferSize =        16u * 1024u  /* 16KB */
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Disable copy construction.
		BatchCoordinator( const BatchCoordinator& ) = delete;
	//!	Constructs this @ref BatchCoordinator instance.
		BatchCoordinator();

		~BatchCoordinator() = default;

	// ---------------------------------------------------

	public:
		VkResult	BindResources( GpuHeap& heap );

		void		FreeResources( GpuHeap& heap );

	// ---------------------------------------------------

	public:
		void	AddThing( void* thing );

		void	ResetCounts();

	// ---------------------------------------------------

	public:
		void	RecordDraws( CommandList& commands, VertexBuffer& vertices, IndexBuffer& indices );

	// ---------------------------------------------------

	//!	Disable copy assignment.
		BatchCoordinator& operator=( const BatchCoordinator& ) = delete;

		friend void Swap( BatchCoordinator&, BatchCoordinator& );

	// - DATA MEMBERS ------------------------------------
	
	private:
	//!	Container for draw call parameters to be passed to the GPU.
		UniformBuffer				_drawParameters;
		ArrayMap<void*, uint32_t>	_countsByThing;
	};

}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2