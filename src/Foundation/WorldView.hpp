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

	namespace Scheduler {
		class	WorkerContext;
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

		virtual void	AcceptTaskVisitor( ::Eldritch2::Allocator& subtaskAllocator, Scheduler::WorkerContext& executingContext, Scheduler::Task& visitingTask, const FrameTickTaskVisitor );
		virtual void	AcceptTaskVisitor( ::Eldritch2::Allocator& subtaskAllocator, Scheduler::WorkerContext& executingContext, Scheduler::Task& visitingTask, const PreScriptTickTaskVisitor );
		virtual void	AcceptTaskVisitor( ::Eldritch2::Allocator& subtaskAllocator, Scheduler::WorkerContext& executingContext, Scheduler::Task& visitingTask, const ScriptTickTaskVisitor );
		virtual void	AcceptTaskVisitor( ::Eldritch2::Allocator& subtaskAllocator, Scheduler::WorkerContext& executingContext, Scheduler::Task& visitingTask, const PostScriptTickTaskVisitor );

	// ---------------------------------------------------

		virtual void	AcceptViewVisitor( const ScriptExecutionPreparationVisitor );
		virtual void	AcceptViewVisitor( const DeletionPreparationVisitor );
		virtual void	AcceptViewVisitor( const LoadFinalizationVisitor );

	// ---------------------------------------------------

	protected:
		template <typename TaskVisitor>
		void	BroadcastTaskVisitor( ::Eldritch2::Allocator& subtaskAllocator, Scheduler::WorkerContext& executingContext, Scheduler::Task& visitingTask, TaskVisitor&& visitor );

		template <typename ViewVisitor>
		void	BroadcastViewVisitor( ViewVisitor&& visitor );

	// - WORLD VIEW SANDBOX METHODS ----------------------

		//!	Retrieves a read-only view of the hosting @ref World.
		/*!	 @returns A const reference to the owning @ref World.
			*/
		ETInlineHint const Foundation::World&	GetOwningWorld() const;

		//!	Retrieves the hosting game engine's @ref ContentLibrary.
		/*!	@returns A const reference to the @ref ContentLibrary used by the @ref GameEngine that owns *this.
			*/
		const FileSystem::ContentLibrary&		GetEngineContentLibrary() const;

		//!	Retrieves the hosting world's general-purpose @ref Allocator.
		::Eldritch2::Allocator&					GetWorldAllocator();

		//!	Notifies the @ref World that the view is interested in pausing simulation.
		void									SetPaused( bool paused = true );

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		//!	Constructs this @ref WorldView instance.
		/*!	@param[in] owningWorld <Parameter Description>
			*/
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
