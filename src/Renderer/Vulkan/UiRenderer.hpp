/*==================================================================*\
  UiRenderer.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/UiRenderer.hpp>
//------------------------------------------------------------------//
#include <vulkan/vulkan.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Scheduling {
		class	JobFiber;
	}
}

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {

	class UiRenderer : public Renderer::UiRenderer {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref UiRenderer instance.
		UiRenderer( const UiRenderer& ) = default;
	//!	Constructs this @ref UiRenderer instance.
		UiRenderer();

		~UiRenderer() = default;

	// ---------------------------------------------------

	public:
		void	BuildCommandBuffers( Scheduling::JobFiber& executor, VkDevice device, VkCommandPool commandPool ) const;

	// ---------------------------------------------------

	private:
		// Eldritch2::ArraySet<>
	};

}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2