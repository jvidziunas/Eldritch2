/*==================================================================*\
  WorkerContext.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scheduler/WorkerContext.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Scheduler;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Scheduler {

	WorkerContext::WorkerContext( const WorkItem& defaultWorkItem, ::Eldritch2::Allocator& allocator ) : _workItems( 0u, allocator, UTF8L("Worker Context Job Queue Allocator") ) {
		_workItems.Reserve( 64u );
		_workItems.EmplaceBack( defaultWorkItem );
	}

// ---------------------------------------------------

	void WorkerContext::Enqueue( const WorkItem& workItem ) {
		_workItems.EmplaceBack( workItem );
	}

// ---------------------------------------------------

	WorkerContext::WorkItem WorkerContext::PopTask() {
		const WorkItem result( _workItems.Back() );

		if( _workItems.Size() > 1u ) {
			_workItems.PopBack();
		}

		return result;
	}

}	// namespace Scheduler
}	// namespace Eldritch2