/*==================================================================*\
  SteamworksWorldView.hpp
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
		class	SteamworksNetworkingService;
	}
}

namespace Eldritch2 {
namespace Networking {

	namespace Foundation	= ::Eldritch2::Foundation;
	namespace Networking	= ::Eldritch2::Networking;
	namespace Scripting		= ::Eldritch2::Scripting;
	namespace Utility		= ::Eldritch2::Utility;

// ---------------------------------------------------

	class SteamworksWorldView : public Foundation::WorldView {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		// Constructs this SteamworksWorldView instance.
		SteamworksWorldView( Networking::SteamworksNetworkingService& networkingService, const int replicationChannelID, Foundation::World& owningWorld );

		// Destroys this SteamworksWorldView instance.
		~SteamworksWorldView();

	// ---------------------------------------------------

	protected:
		void	AcceptTaskVisitor( ::Eldritch2::Allocator& subtaskAllocator, Scheduler::WorkerContext& executingContext, Scheduler::Task& visitingTask, const PreScriptTickTaskVisitor ) override sealed;
		void	AcceptTaskVisitor( ::Eldritch2::Allocator& subtaskAllocator, Scheduler::WorkerContext& executingContext, Scheduler::Task& visitingTask, const PostScriptTickTaskVisitor ) override sealed;

	// ---------------------------------------------------

		void	AcceptViewVisitor( Scripting::ScriptMessageSink& messageSink );

	// - DATA MEMBERS ------------------------------------

	private:
		Networking::SteamworksNetworkingService&	_networkingService;
		const int									_replicationChannelID;
	};

}	// namespace Foundation
}	// namespace Eldritch2