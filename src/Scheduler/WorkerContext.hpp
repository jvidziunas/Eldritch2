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
#include <atomic>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scheduler {

	class WorkerContext {
	// - TYPE PUBLISHING ---------------------------------

	public:
		using FinishCounter = ::std::atomic<int>;
		using WorkFunction	= void (*)( void*, WorkerContext& );

	// ---
		
		struct WorkItem {
			void*			argument;
			WorkFunction	function;
		};

	// ---

		struct CompletionItem {
			FinishCounter*	counter;
			WorkItem		workItem;
		};

	// ---------------------------------------------------
		
		template <size_t workItemCount>
		ETInlineHint void	Enqueue( FinishCounter& finishCounter, const WorkItem (&workItems)[workItemCount] );
		void				Enqueue( FinishCounter& finishCounter, const WorkItem* workItemBegin, const WorkItem* workItemEnd );
		void				Enqueue( FinishCounter& finishCounter, const WorkItem& workItem );

		virtual void		WaitForCounter( FinishCounter& finishCounter, int value = 0 ) abstract;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
		//!	Constructs this @ref WorkerContext instance.
		WorkerContext( const CompletionItem& defaultItem, ::Eldritch2::Allocator& allocator );
		//!	Constructs this @ref WorkerContext instance.
		WorkerContext( WorkerContext&& );
		//!	Destroys this @ref WorkerContext instance.
		~WorkerContext() = default;

	// ---------------------------------------------------

		CompletionItem	PopCompletionItem();

	// - DATA MEMBERS ------------------------------------

		::Eldritch2::ResizableArray<CompletionItem>	_completionItems;
	};

}	// namespace Scheduler
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Scheduler/WorkerContext.inl>
//------------------------------------------------------------------//