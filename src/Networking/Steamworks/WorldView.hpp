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
#include <Foundation/WorldView.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Networking {
		namespace Steamworks {
			class	EngineService;
		}
	}
}

namespace Eldritch2 {
namespace Networking {

	namespace Foundation	= ::Eldritch2::Foundation;
	namespace Networking	= ::Eldritch2::Networking;
	namespace Scripting		= ::Eldritch2::Scripting;
	namespace Utility		= ::Eldritch2::Utility;

// ---------------------------------------------------

namespace Steamworks {

	class WorldView : public Foundation::WorldView {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref WorldView instance.
		WorldView( Steamworks::EngineService& networkingService, const int replicationChannelID, Foundation::World& owningWorld );

		//!	Destroys this @ref WorldView instance.
		~WorldView() = default;

	// ---------------------------------------------------

	protected:
		void	AcceptTaskVisitor( ::Eldritch2::Allocator& subtaskAllocator, Scheduler::WorkerContext& executingContext, Scheduler::Task& visitingTask, const PreScriptTickTaskVisitor ) override sealed;
		void	AcceptTaskVisitor( ::Eldritch2::Allocator& subtaskAllocator, Scheduler::WorkerContext& executingContext, Scheduler::Task& visitingTask, const PostScriptTickTaskVisitor ) override sealed;

	// ---------------------------------------------------

		void	AcceptViewVisitor( Scripting::ScriptMessageSink& messageSink );

	// - DATA MEMBERS ------------------------------------

	private:
		Steamworks::EngineService&	_networkingService;
		const int					_replicationChannelID;
	};

}	// namespace Steamworks
}	// namespace Foundation
}	// namespace Eldritch2