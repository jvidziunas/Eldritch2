/*==================================================================*\
  Direct3D11WorldView.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/ScriptAPIRegistrationInitializationVisitor.hpp>
#include <Renderer/D3D11/Direct3D11WorldView.hpp>
#include <Scheduler/CRTPTransientTask.hpp>
#include <Utility/Memory/InstanceNew.hpp>
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

	static ETThreadLocal	Direct3D11WorldView*	activeScriptWorldViewForThread = nullptr;

}	// anonymous namespace

namespace Eldritch2 {
namespace Renderer {

	Direct3D11WorldView::Direct3D11WorldView( World& owningWorld, const Direct3D11MeshView& defaultMesh ) : WorldView( owningWorld ), _defaultMesh( defaultMesh ), _meshPool( UTF8L("Direct3D11 World View Mesh Pool Allocator"), 128u, GetWorldAllocator() ) {}

// ---------------------------------------------------

	Direct3D11WorldView::~Direct3D11WorldView() {}

// ---------------------------------------------------

	void Direct3D11WorldView::AcceptViewVisitor( const ScriptExecutionPreparationVisitor ) {
		activeScriptWorldViewForThread = this;
	}

// ---------------------------------------------------

	void Direct3D11WorldView::AcceptTaskVisitor( Allocator& subtaskAllocator, WorkerContext& executingContext, Task& visitingTask, const PreScriptTickTaskVisitor ) {
		class DrawViewsTask : public CRTPTransientTask<DrawViewsTask> {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			// Constructs this DrawViewsTask instance.
			ETInlineHint DrawViewsTask( Direct3D11WorldView& worldView, WorkerContext& executingContext, Task& visitingTask, Allocator& subtaskAllocator ) : CRTPTransientTask<DrawViewsTask>( visitingTask, Scheduler::ContinuationTaskSemantics ),
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
			Allocator&				_subtaskAllocator;
			Direct3D11WorldView&	_worldView;
		};

	// ---

		new(subtaskAllocator, Allocator::AllocationOption::TEMPORARY_ALLOCATION) DrawViewsTask( *this, executingContext, visitingTask, subtaskAllocator );
	}

// ---------------------------------------------------

	void Direct3D11WorldView::AcceptTaskVisitor( Allocator& /*subtaskAllocator*/, WorkerContext& /*executingContext*/, Task& /*visitingTask*/, const PostScriptTickTaskVisitor ) {}

// ---------------------------------------------------

	void Direct3D11WorldView::ExposeScriptAPI( ScriptAPIRegistrationInitializationVisitor& visitor ) {
		SceneCameraComponent::ExposeScriptAPI( visitor );
		MeshComponent::ExposeScriptAPI( visitor );
	}

// ---------------------------------------------------

	Direct3D11WorldView& Direct3D11WorldView::GetActiveWorldView() {
		ETRuntimeAssert( nullptr != activeScriptWorldViewForThread );

		return *activeScriptWorldViewForThread;
	}

}	// namespace Renderer
}	// namespace Eldritch2