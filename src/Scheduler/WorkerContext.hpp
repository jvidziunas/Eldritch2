/*==================================================================*\
  WorkerContext.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/ResizableArray.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scheduler {

	namespace Scheduler	= ::Eldritch2::Scheduler;

// ---------------------------------------------------

	class WorkerContext {
	// - TYPE PUBLISHING ---------------------------------

	public:
		struct WorkItem {
		// - TYPE PUBLISHING ---------------------------------

			typedef void (*CodePointer)(void*, WorkerContext&);

		// ---

			CodePointer	codePointer;
			void*		data;
		};

	// ---------------------------------------------------

		void	Enqueue( const WorkItem& workItem );

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
		//! Constructs this @ref WorkerContext instance.
		WorkerContext( const WorkItem& defaultWorkItem, ::Eldritch2::Allocator& allocator );

		// Destroys this @ref WorkerContext instance.
		~WorkerContext() = default;

	// ---------------------------------------------------

		WorkItem	PopTask();

	// - DATA MEMBERS ------------------------------------

		::Eldritch2::ResizableArray<WorkItem>	_workItems;
	};

}	// namespace Scheduler
}	// namespace Eldritch2