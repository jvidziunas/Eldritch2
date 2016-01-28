/*==================================================================*\
  WorldView.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
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
		class	World;
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
		struct ScriptExecutionPreparationVisitor {};
		struct DeletionPreparationVisitor {};

	// ---------------------------------------------------

		template <void (WorldView::*TickFunction)( Scheduler::WorkerContext& )>
		void	InvokeTickFunction( Scheduler::WorkerContext::FinishCounter& finishCounter, Scheduler::WorkerContext& executingContext );

		template <void (WorldView::*TickFunction)( Scheduler::WorkerContext& )>
		void	InvokeTickFunction( Scheduler::WorkerContext& executingContext );

		template <typename ViewVisitor>
		void	BroadcastViewVisitor( ViewVisitor&& visitor );

	// - WORLD VIEW SANDBOX METHODS ----------------------

		virtual void	OnFrameTick( Scheduler::WorkerContext& executingContext );

		virtual void	OnPreScriptTick( Scheduler::WorkerContext& executingContext );

		virtual void	OnScriptTick( Scheduler::WorkerContext& executingContext );

		virtual void	OnPostScriptTick( Scheduler::WorkerContext& executingContext );

	// ---------------------------------------------------

		virtual void	OnGameStart( Scheduler::WorkerContext& executingContext );

	// ---------------------------------------------------

		virtual void	AcceptViewVisitor( const ScriptExecutionPreparationVisitor );
		virtual void	AcceptViewVisitor( const DeletionPreparationVisitor );

	// ---------------------------------------------------

	protected:
		//!	Retrieves a read-only view of the hosting @ref World.
		/*!	@returns A const reference to the owning @ref World.
			@threadsafe
			*/
		ETInlineHint const Foundation::World&	GetOwningWorld() const;

		//!	Retrieves the hosting game engine's @ref ContentLibrary.
		/*!	@returns A const reference to the @ref ContentLibrary used by the @ref GameEngine that owns *this.
			@threadsafe
			*/
		const FileSystem::ContentLibrary&		GetContentLibrary() const;

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
