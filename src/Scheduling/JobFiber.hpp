/*==================================================================*\
  JobFiber.hpp
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
#include <functional>
#include <atomic>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scheduling {

	using JobBarrier = std::atomic<int>;

// ---------------------------------------------------

	class JobFiber {
	// - TYPE PUBLISHING ---------------------------------

	public:
		class CompletionItem {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
		//!	Constructs this @ref CompletionItem instance.
			template <typename WorkItem>
			CompletionItem( JobBarrier& barrier, WorkItem&& workItem );
		//!	Constructs this @ref CompletionItem instance.
			CompletionItem( CompletionItem&& );

			~CompletionItem() = default;

		// ---------------------------------------------------

			JobBarrier*						barrier;
			std::function<void (JobFiber&)>	workItem;
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
	//!	Constructs this @ref JobFiber instance.
		JobFiber( CompletionItem&& defaultItem, Eldritch2::Allocator& allocator );
	//!	Constructs this @ref JobFiber instance.
		JobFiber( JobFiber&& );

		~JobFiber() = default;

	// ---------------------------------------------------
		
	public:
		virtual void	AwaitBarrier( JobBarrier& barrier, int value = 0 ) abstract;

		template <typename WorkItem>
		void			Enqueue( JobBarrier& barrier, WorkItem&& workItem );

	// ---------------------------------------------------

	public:
		CompletionItem	PopCompletionItem();

	// ---------------------------------------------------

	protected:
	//!	Binds this @ref JobFiber as the active context on the calling thread.
	/*!	@see @ref GetActiveJobFiber */
		void	MakeActive();

	// - DATA MEMBERS ------------------------------------

	protected:
		Eldritch2::ResizableArray<CompletionItem>	_completionItems;
	};

// ---------------------------------------------------

//!	Retrieves the active worker context for the thread.
/*!	@returns A reference to the previously-bound @ref JobFiber for the calling thread.
	@see @ref JobFiber::MakeActive */
	Scheduling::JobFiber&	GetActiveJobFiber();

}	// namespace Scheduling
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Scheduling/JobFiber.inl>
//------------------------------------------------------------------//