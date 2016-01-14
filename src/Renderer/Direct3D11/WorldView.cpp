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
#include <Scheduler/CRTPTransientTask.hpp>
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

	void WorldView::AcceptTaskVisitor( Allocator& subtaskAllocator, WorkerContext& executingContext, Task& visitingTask, const PreScriptTickTaskVisitor ) {
		class SubmitDrawCommandsTask : public CRTPTransientTask<SubmitDrawCommandsTask> {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref SubmitDrawCommandsTask instance.
			ETInlineHint SubmitDrawCommandsTask( WorldView& worldView, WorkerContext& executingContext, Task& visitingTask, Allocator& subtaskAllocator ) : CRTPTransientTask<SubmitDrawCommandsTask>( visitingTask, Scheduler::ContinuationTaskSemantics ),
																																							_subtaskAllocator( subtaskAllocator ),
																																							_worldView( worldView ) {
				TrySchedulingOnContext( executingContext );
			}

			~SubmitDrawCommandsTask() = default;

		// ---------------------------------------------------

			const UTF8Char* const GetHumanReadableName() const override sealed {
				return UTF8L("Submit Draw Commands Task");
			}

		// ---------------------------------------------------

			Task* Execute( WorkerContext& executingContext ) override sealed {
				for( const auto& camera : _worldView._attachedCameras ) {
					// new(_subtaskAllocator, Allocator::AllocationOption::TEMPORARY_ALLOCATION) ASDFTask();
				}

				return nullptr;
			}

		// - DATA MEMBERS ------------------------------------

		private:
			Allocator&	_subtaskAllocator;
			WorldView&	_worldView;
		};

	// ---

		new(subtaskAllocator, Allocator::AllocationOption::TEMPORARY_ALLOCATION) SubmitDrawCommandsTask( *this, executingContext, visitingTask, subtaskAllocator );
	}

// ---------------------------------------------------

	void WorldView::AcceptTaskVisitor( Allocator& /*subtaskAllocator*/, WorkerContext& /*executingContext*/, Task& /*visitingTask*/, const PostScriptTickTaskVisitor ) {}

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