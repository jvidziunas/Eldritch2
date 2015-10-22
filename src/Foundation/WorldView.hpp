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
#include <Scheduler/Task.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Foundation {
		struct	FrameBarrierSet;
		class	World;
	}

	namespace FileSystem {
		class	ContentLibrary;
	}

	namespace Scripting {
		class	ScriptMessageSink;
	}

	namespace Scheduler {
		class	WorkerContext;
	}

	class	Allocator;
}

namespace Eldritch2 {
namespace Foundation {

	namespace Foundation	= ::Eldritch2::Foundation;
	namespace FileSystem	= ::Eldritch2::FileSystem;
	namespace Scripting		= ::Eldritch2::Scripting;
	namespace Scheduler		= ::Eldritch2::Scheduler;

// ---------------------------------------------------

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

		virtual void	AcceptTaskVisitor( ::Eldritch2::Allocator& subtaskAllocator, Scheduler::WorkerContext& executingContext, Scheduler::Task& visitingTask, const FrameTickTaskVisitor );
		virtual void	AcceptTaskVisitor( ::Eldritch2::Allocator& subtaskAllocator, Scheduler::WorkerContext& executingContext, Scheduler::Task& visitingTask, const PreScriptTickTaskVisitor );
		virtual void	AcceptTaskVisitor( ::Eldritch2::Allocator& subtaskAllocator, Scheduler::WorkerContext& executingContext, Scheduler::Task& visitingTask, const ScriptTickTaskVisitor );
		virtual void	AcceptTaskVisitor( ::Eldritch2::Allocator& subtaskAllocator, Scheduler::WorkerContext& executingContext, Scheduler::Task& visitingTask, const PostScriptTickTaskVisitor );

	// ---------------------------------------------------

		virtual void	AcceptViewVisitor( const ScriptExecutionPreparationVisitor );
		virtual void	AcceptViewVisitor( Scripting::ScriptMessageSink& messageSink );
		virtual void	AcceptViewVisitor( const DeletionPreparationVisitor );
		virtual void	AcceptViewVisitor( const LoadFinalizationVisitor );

	// ---------------------------------------------------

	protected:
		template <typename TaskVisitor>
		void	BroadcastTaskVisitor( ::Eldritch2::Allocator& subtaskAllocator, Scheduler::WorkerContext& executingContext, Scheduler::Task& visitingTask, TaskVisitor&& visitor );

		template <typename ViewVisitor>
		void	BroadcastViewVisitor( ViewVisitor&& visitor );

	// - WORLD VIEW SANDBOX METHODS ----------------------

		//!	Retrieves a read-only view of the hosting world. Useful for inspecting shared state.
		ETInlineHint const Foundation::World&	GetOwningWorld() const;

		//!	Retrieves the hosting game engine's content library.
		const FileSystem::ContentLibrary&		GetEngineContentLibrary() const;

		//!	Retrieves the hosting world's general-purpose memory allocator.
		::Eldritch2::Allocator&					GetWorldAllocator();

		//!	Notifies the world that a view is interested in pausing simulation.
		void									SetPaused( bool paused = true );

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		//!	Constructs this @ref WorldView instance.
		WorldView( Foundation::World& owningWorld );

	public:
		//!	Destroys this @ref WorldView instance.
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
