/*==================================================================*\
  SceneCamera.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/Vulkan/ScriptComponents/PlatformWindowSwapChain.hpp>
#include <Utility/Containers/ArraySet.hpp>
#include <Utility/CountedPointer.hpp>
#include <Renderer/Camera.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Scripting {
		class	ApiRegistrar;
	}

	namespace Animation {
		namespace ScriptComponents {
			class	Armature;
		}
	}
}

struct	VkViewport;

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {
namespace ScriptComponents {

	class SceneCamera : public Renderer::Camera {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref SceneCamera instance.
		SceneCamera( const PlatformWindowSwapChain& swapChain, Eldritch2::RigidTransform transform, Eldritch2::RadianMeasure horizontalFovAngle, Eldritch2::float32 aspectRatio );
	//!	Disable copying.
		SceneCamera( const SceneCamera& ) = delete;

		~SceneCamera();

	// - SCRIPT API REFERENCE ----------------------------

	public:
	//! Registers all script-callable methods for the @ref SceneCamera type with the specified script type registrar.
		static void	RegisterScriptApi( Scripting::ApiRegistrar& registrar );

	// ---------------------------------------------------

	public:
		VkViewport	GetViewport() const;

	// - DATA MEMBERS ------------------------------------

	private:
		Eldritch2::CountedPointer<const PlatformWindowSwapChain, Eldritch2::NoopDisposer>	_swapChain;
	//!	Collection of armatures that should always be animated for this camera. Generally this will contain the armature the camera is attached to (and thus pulls transform data from)
		Eldritch2::ArraySet<const Animation::ScriptComponents::Armature*>					_armaturesToAlwaysAnimate;
	};

}	// namespace ScriptComponents
}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2