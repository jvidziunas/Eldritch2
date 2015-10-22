/*==================================================================*\
  WorldView.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  �2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/ScriptAPIRegistrationInitializationVisitor.hpp>
#include <Scheduler/CRTPTransientTask.hpp>
#include <Utility/Memory/InstanceNew.hpp>
#include <Renderer/Direct3D11/WorldView.hpp>
#include <Utility/MPL/VectorTypes.hpp>
#include <Utility/ErrorCode.hpp>
//------------------------------------------------------------------//
#include <D3D11.h>
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

	static ETThreadLocal	::Renderer::Direct3D11::WorldView*	activeScriptWorldViewForThread = nullptr;

}	// anonymous namespace

namespace Eldritch2 {
namespace Renderer {
namespace Direct3D11 {

	WorldView::WorldView( World& owningWorld, const MeshResourceView& defaultMesh ) : Foundation::WorldView( owningWorld ), _defaultMesh( defaultMesh ), _meshPool( UTF8L("Direct3D11 World View Mesh Pool Allocator"), 128u, GetWorldAllocator() ) {}

// ---------------------------------------------------

	WorldView::~WorldView() {}

// ---------------------------------------------------

	void WorldView::AcceptViewVisitor( const ScriptExecutionPreparationVisitor ) {
		activeScriptWorldViewForThread = this;
	}

// ---------------------------------------------------

	void WorldView::AcceptTaskVisitor( Allocator& subtaskAllocator, WorkerContext& executingContext, Task& visitingTask, const PreScriptTickTaskVisitor ) {
		class DrawViewsTask : public CRTPTransientTask<DrawViewsTask> {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref DrawViewsTask instance.
			ETInlineHint DrawViewsTask( WorldView& worldView, WorkerContext& executingContext, Task& visitingTask, Allocator& subtaskAllocator ) : CRTPTransientTask<DrawViewsTask>( visitingTask, Scheduler::ContinuationTaskSemantics ),
																																				   _subtaskAllocator( subtaskAllocator ),
																																				   _worldView( worldView ) {
				TrySchedulingOnContext( executingContext );
			}

		// ---------------------------------------------------

			const UTF8Char* const GetHumanReadableName() const override sealed {
				return UTF8L("Draw Views Task");
			}

		// ---------------------------------------------------

			Task* Execute( WorkerContext& /*executingContext*/ ) override sealed {
				return nullptr;
			}

		// - DATA MEMBERS ------------------------------------

		private:
			Allocator&	_subtaskAllocator;
			WorldView&	_worldView;
		};

	// ---

		new(subtaskAllocator, Allocator::AllocationOption::TEMPORARY_ALLOCATION) DrawViewsTask( *this, executingContext, visitingTask, subtaskAllocator );
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

		return *activeScriptWorldViewForThread;
	}

}	// namespace Direct3D11
}	// namespace Renderer
}	// namespace Eldritch2