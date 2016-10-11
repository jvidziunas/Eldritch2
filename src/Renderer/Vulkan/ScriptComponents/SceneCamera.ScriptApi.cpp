/*==================================================================*\
  SceneCamera.ScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/Vulkan/ScriptComponents/SceneCamera.hpp>
#include <Renderer/Vulkan/SceneRenderer.hpp>
#include <Scripting/ApiRegistrar.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Scripting;

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {
namespace ScriptComponents {
namespace {

	void ConstructSceneCamera( void* thisPointer, SceneRenderer& renderer, const PlatformWindowSwapChain& swapChain, const RigidTransform& transform, float32 horizontalFovAngle, float32 aspectRatio ) {
		renderer.AttachCamera( *new(thisPointer) SceneCamera( swapChain, transform, RadianMeasure( horizontalFovAngle ), aspectRatio ) );
	}

}	// anonymous namespace

	void SceneCamera::RegisterScriptApi( ApiRegistrar& registrar ) {
		registrar.DefineClass<SceneCamera>( {/* Constructors */
												DefineOverloadedConstructor( void(*)( void*, Injected<SceneRenderer&>, const PlatformWindowSwapChain&, const RigidTransform&, float32, float32 ), ConstructSceneCamera )
											},
											{/*	Properties */},
											{/*	Methods */},
											{/*	Operators */},
											{/*	Fields */} );
	}

}	// namespace ScriptComponents
}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2