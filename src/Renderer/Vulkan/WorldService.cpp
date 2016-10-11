/*==================================================================*\
  WorldService.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/Vulkan/ScriptComponents/PlatformWindowSwapChain.hpp>
#include <Renderer/Vulkan/ScriptComponents/SceneCamera.hpp>
#include <Renderer/Vulkan/ScriptComponents/SphereLight.hpp>
#include <Renderer/Vulkan/ScriptComponents/RenderMesh.hpp>
#include <Renderer/Vulkan/WorldService.hpp>
#include <Scheduling/JobFiber.hpp>
#include <Core/World.hpp>
//------------------------------------------------------------------//
#include <microprofile/microprofile.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Core;
using namespace ::Eldritch2::Scheduling;
using namespace ::Eldritch2::Scripting;

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {

	WorldService::WorldService( const World& world ) : Core::WorldService( world.GetServiceBlackboard() ), _sceneRenderer( world.GetAllocator() ) {}

// ---------------------------------------------------

	Utf8Literal WorldService::GetName() const {
		return "Vulkan World Service";
	}

// ---------------------------------------------------

	void WorldService::RegisterScriptApi( ApiRegistrar& visitor ) {
		ScriptComponents::PlatformWindowSwapChain::RegisterScriptApi( visitor );
		ScriptComponents::SceneCamera::RegisterScriptApi( visitor );
		ScriptComponents::SphereLight::RegisterScriptApi( visitor );
		ScriptComponents::RenderMesh::RegisterScriptApi( visitor );
	}

// ---------------------------------------------------

	void WorldService::AcceptVisitor( ServiceBlackboard& blackboard ) {
		blackboard.Publish<Renderer::UiRenderer>( _uiRenderer );
		blackboard.Publish<Vulkan::SceneRenderer>( _sceneRenderer );
	}

// ---------------------------------------------------

	void WorldService::OnScriptPreTick( JobFiber& executor ) {
		MICROPROFILE_SCOPEI( GetName(), "Dispatch world render commands", 0x0011F0 );

		JobBarrier	renderBarrier( 0 );

	//	Draw all UI elements.
		executor.Enqueue( renderBarrier, [this] ( JobFiber& executor ) {
			MICROPROFILE_SCOPEI( GetName(), "Draw UI Components", 0xAB11F0 );

			_uiRenderer.BuildCommandBuffers( executor, VK_NULL_HANDLE, VK_NULL_HANDLE );
		} );

	//	Draw all conventional elements.
		executor.Enqueue( renderBarrier, [this] ( JobFiber& executor ) {
			MICROPROFILE_SCOPEI( GetName(), "Draw Main Scene", 0x0C11F0 );

			_sceneRenderer.BuildCommandBuffers( executor, VK_NULL_HANDLE, VK_NULL_HANDLE );
		} );

		executor.AwaitBarrier( renderBarrier );
	}

}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2

