/*==================================================================*\
  WorldView.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/ScriptAPIRegistrationInitializationVisitor.hpp>
#include <Renderer/Direct3D11/WorldView.hpp>
#include <Utility/Memory/InstanceNew.hpp>
#include <Utility/MPL/VectorTypes.hpp>
#include <Utility/ErrorCode.hpp>
//------------------------------------------------------------------//
#if( ET_COMPILER_IS_MSVC )
//	MSVC complains about macro redefinitions, since a few DirectX components separately
//	define some HRESULT values without an include guard. The definitions themselves are consistent,
//	so just disable the warning.
#	pragma warning( push )
#	pragma warning( disable : 4005 )
#endif
#include <D3D11.h>
#if( ET_COMPILER_IS_MSVC )
#	pragma warning( pop )
#endif
//------------------------------------------------------------------//

//==================================================================//
// LIBRARIES
//==================================================================//
ET_LINK_LIBRARY( "D3D11.lib" )
//------------------------------------------------------------------//

using namespace ::Eldritch2::Foundation;
using namespace ::Eldritch2::FileSystem;
using namespace ::Eldritch2::Animation;
using namespace ::Eldritch2::Scheduler;
using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Renderer;
using namespace ::Eldritch2;

namespace {

	static ETThreadLocal Renderer::Direct3D11::WorldView*	activeScriptWorldViewForThread = nullptr;

}	// anonymous namespace

namespace Eldritch2 {
namespace Renderer {
namespace Direct3D11 {

	WorldView::WorldView( World& owningWorld, const MeshResourceView& defaultMesh ) : Foundation::WorldView( owningWorld ),
																					  _defaultMesh( defaultMesh ),
																					  _meshPool( UTF8L("Mesh Pool Allocator"), sizeof(MeshComponent), 128u, GetWorldAllocator() ),
																					  _visibilitySystem( { owningWorld.GetPropertyByKey( UTF8L("VisibilityCellLength"), 16.0f ), owningWorld.GetPropertyByKey( UTF8L("VisibilityCellHeight"), 32.0f ) },
																										 { owningWorld.GetPropertyByKey( UTF8L("VisibilityCellLength"), 16.0f ), owningWorld.GetPropertyByKey( UTF8L("VisibilityCellHeight"), 32.0f ) },
																										 { GetWorldAllocator(), UTF8L("Shadow Caster Cell Allocator") },
																										 { GetWorldAllocator(), UTF8L("Mesh Visibility Cell Allocator") } ),
																					  _shadowMaterialUsageCache( { GetWorldAllocator(), UTF8L("Shadow Material Usage Cache") } ) {}

// ---------------------------------------------------

	void WorldView::AcceptViewVisitor( const ScriptExecutionPreparationVisitor ) {
		activeScriptWorldViewForThread = this;
	}

// ---------------------------------------------------

	void WorldView::AcceptTaskVisitor( WorkerContext& /*executingContext*/, WorkerContext::FinishCounter& /*finishCounter*/, const PreScriptTickTaskVisitor ) {}

// ---------------------------------------------------

	void WorldView::AcceptTaskVisitor( WorkerContext& /*executingContext*/, WorkerContext::FinishCounter& /*finishCounter*/, const PostScriptTickTaskVisitor ) {}

// ---------------------------------------------------

	void WorldView::ExposeScriptAPI( ScriptAPIRegistrationInitializationVisitor& visitor ) {
		SceneCameraComponent::ExposeScriptAPI( visitor );
		MeshComponent::ExposeScriptAPI( visitor );
	}

// ---------------------------------------------------

	WorldView& WorldView::GetActiveWorldView() {
		ETRuntimeAssert( nullptr != activeScriptWorldViewForThread );

	// ---

		return *activeScriptWorldViewForThread;
	}

}	// namespace Direct3D11
}	// namespace Renderer
}	// namespace Eldritch2