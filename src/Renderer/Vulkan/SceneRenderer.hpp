/*==================================================================*\
  SceneRenderer.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/ArraySet.hpp>
#include <Platform/UserReadWriteMutex.hpp>
//------------------------------------------------------------------//
#include <vulkan/vulkan.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Scheduling {
		class	JobFiber;
	}

	namespace Renderer {
		namespace Vulkan {
			namespace ScriptComponents {
				class	SceneCamera;
			}
		}
	}
}

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {

	class SceneRenderer {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref SceneRenderer instance.
		SceneRenderer( Eldritch2::Allocator& allocator );
	//!	Constructs this @ref SceneRenderer instance.
		SceneRenderer( const SceneRenderer& ) = delete;

		~SceneRenderer() = default;

	// - ACTIVE CAMERA MANIPULATION ----------------------

	public:
	//!	Adds the specified camera to the list of cameras the world will generate draw commands for each frame.
	/*!	The world will continue to issue commands for the camera until it is explicitly removed; you do not
	 *	need to add the camera to this list each frame.
		@param[in] camera The camera to pull data from.
		@see @ref DetachCamera
		@remarks Idempotent, thread-safe. */
		void	AttachCamera( const ScriptComponents::SceneCamera& camera );

	//!	Notifies the world that you are no longer interested in creating draw commands for the camera.
	/*!	@param[in] camera The camera to remove from the active collection.
		@see @ref AttachCamera
		@remarks Idempotent, thread-safe. */
		void	DetachCamera( const ScriptComponents::SceneCamera& camera );

	// ---------------------------------------------------

	public:
		void	BuildCommandBuffers( Scheduling::JobFiber& executor, VkDevice device, VkCommandPool commandPool ) const;

	// ---------------------------------------------------

	private:
		mutable Platform::UserReadWriteMutex						_cameraMutex;
		Eldritch2::ArraySet<const ScriptComponents::SceneCamera*>	_cameras;
	};

}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Renderer/Vulkan/SceneRenderer.inl>
//------------------------------------------------------------------//

