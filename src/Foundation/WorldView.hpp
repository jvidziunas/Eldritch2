/*==================================================================*\
  WorldView.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  �2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/IntrusiveForwardListHook.hpp>
#include <Scheduler/WorkerContext.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Foundation {
		struct	FrameBarrierSet;
		class	World;
	}

	namespace Scripting {
		class	MessageBus;
	}

	namespace FileSystem {
		class	ContentLibrary;
	}

	class	Allocator;
}

namespace Eldritch2 {
namespace Foundation {

	class ETPureAbstractHint WorldView : public ::Eldritch2::IntrusiveForwardListBaseHook {
	// - TYPE PUBLISHING ---------------------------------

	public:
		struct FrameTickTaskVisitor {};

		struct PreScriptTickTaskVisitor {};
		struct ScriptTickTaskVisitor {};
		struct PostScriptTickTaskVisitor {};

		struct ScriptExecutionPreparationVisitor {};
		struct DeletionPreparationVisitor {};
		struct LoadFinalizationVisitor {};

	// ---------------------------------------------------

		virtual void	AcceptTaskVisitor( Scheduler::WorkerContext& executingContext, Scheduler::WorkerContext::FinishCounter& finishCounter, const FrameTickTaskVisitor );
		virtual void	AcceptTaskVisitor( Scheduler::WorkerContext& executingContext, Scheduler::WorkerContext::FinishCounter& finishCounter, const PreScriptTickTaskVisitor );
		virtual void	AcceptTaskVisitor( Scheduler::WorkerContext& executingContext, Scheduler::WorkerContext::FinishCounter& finishCounter, const ScriptTickTaskVisitor );
		virtual void	AcceptTaskVisitor( Scheduler::WorkerContext& executingContext, Scheduler::WorkerContext::FinishCounter& finishCounter, const PostScriptTickTaskVisitor );
		virtual void	AcceptTaskVisitor( Scheduler::WorkerContext& executingContext, Scheduler::WorkerContext::FinishCounter& finishCounter, Scripting::MessageBus& visitor );

	// ---------------------------------------------------

		virtual void	AcceptViewVisitor( const ScriptExecutionPreparationVisitor );
		virtual void	AcceptViewVisitor( const DeletionPreparationVisitor );
		virtual void	AcceptViewVisitor( const LoadFinalizationVisitor );

	// ---------------------------------------------------

	protected:
		template <typename TaskVisitor>
		void	BroadcastTaskVisitor( Scheduler::WorkerContext& executingContext, Scheduler::WorkerContext::FinishCounter& finishCounter, TaskVisitor&& visitor );

		template <typename ViewVisitor>
		void	BroadcastViewVisitor( ViewVisitor&& visitor );

	// - WORLD VIEW SANDBOX METHODS ----------------------

		//!	Retrieves a read-only view of the hosting @ref World.
		/*!	@returns A const reference to the owning @ref World.
			@threadsafe
			*/
		ETInlineHint const Foundation::World&	GetOwningWorld() const;

		//!	Retrieves the hosting game engine's @ref ContentLibrary.
		/*!	@returns A const reference to the @ref ContentLibrary used by the @ref GameEngine that owns *this.
			@threadsafe
			*/
		const FileSystem::ContentLibrary&		GetEngineContentLibrary() const;

		//!	Retrieves the hosting world's general-purpose @ref Allocator.
		/*!	@threadsafe
			*/
		::Eldritch2::Allocator&					GetWorldAllocator();

		//!	Notifies the @ref World that the view is interested in pausing simulation.
		/*!	@threadsafe
			*/
		void									SetPaused( bool paused = true );

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		//!	Constructs this @ref WorldView instance.
		/*!	@param[in] owningWorld @ref World instance responsible for this @ref WorldView.
			*/
		WorldView( Foundation::World& owningWorld );

	public:
		virtual ~WorldView() = default;
	
	// - DATA MEMBERS ------------------------------------

	protected:
		Foundation::World&	_owningWorld;
	};

}	// namespace Foundation
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Foundation/WorldView.inl>
//------------------------------------------------------------------//
