/*==================================================================*\
  WorldView.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Networking/Steamworks/EngineService.hpp>
#include <Networking/Steamworks/WorldView.hpp>
#include <Scheduler/CRTPTransientTask.hpp>
#include <Utility/Memory/InstanceNew.hpp>
#include <Utility/Concurrency/Lock.hpp>
#include <Utility/ErrorCode.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Networking;
using namespace ::Eldritch2::Foundation;
using namespace ::Eldritch2::Scheduler;
using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Utility;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Networking {
namespace Steamworks {

	WorldView::WorldView( EngineService& networkingService, const int replicationChannelID, World& owningWorld ) : Foundation::WorldView( owningWorld ), _networkingService( networkingService ), _replicationChannelID( replicationChannelID ) {}

// ---------------------------------------------------

	void WorldView::AcceptTaskVisitor( Allocator& subtaskAllocator, WorkerContext& executingContext, Task& visitingTask, const PreScriptTickTaskVisitor ) {
		class ReceivePacketsTask : public CRTPTransientTask<ReceivePacketsTask> {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			ETInlineHint ReceivePacketsTask( WorldView& host, WorkerContext& executingContext, Task& preScriptTickTask ) : CRTPTransientTask<ReceivePacketsTask>( preScriptTickTask, Scheduler::CodependentTaskSemantics ), _host( host ) {
				TrySchedulingOnContext( executingContext );
			}

		// ---------------------------------------------------

			const UTF8Char* const GetHumanReadableName() const override sealed {
				return UTF8L("Steamworks Receive Packets Task");
			}

			Task* Execute( WorkerContext& /*executingContext*/ ) override sealed {
				::CSteamID	senderID;
				char		tempBuffer[EngineService::NETWORK_MTU_SIZE];

				while( _host._networkingService.TryRecievePacket( _host._replicationChannelID, senderID, tempBuffer ) ) {

				}

				return nullptr;
			}

		// - DATA MEMBERS ------------------------------------

		private:
			WorldView&	_host;
		};

	// ---

		new(subtaskAllocator, Allocator::AllocationOption::TEMPORARY_ALLOCATION) ReceivePacketsTask( *this, executingContext, visitingTask );
	}

// ---------------------------------------------------

	void WorldView::AcceptTaskVisitor( Allocator& subtaskAllocator, WorkerContext& executingContext, Task& visitingTask, const PostScriptTickTaskVisitor ) {
		class BroadcastPacketsTask : public CRTPTransientTask<BroadcastPacketsTask> {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			ETInlineHint BroadcastPacketsTask( WorldView& host, WorkerContext& executingContext, Task& postScriptTickTask ) : CRTPTransientTask<BroadcastPacketsTask>( postScriptTickTask, Scheduler::CodependentTaskSemantics ), _host( host ) {
				TrySchedulingOnContext( executingContext );
			}

		// ---------------------------------------------------

			const UTF8Char* const GetHumanReadableName() const override sealed {
				return UTF8L("Steamworks Broadcast Packets Task");
			}

			Task* Execute( WorkerContext& /*executingContext*/ ) override sealed {
				const auto	channelID( _host._replicationChannelID );
				auto&		networkingService( _host._networkingService );
				char		tempBuffer[EngineService::NETWORK_MTU_SIZE];

				return nullptr;
			}

		// - DATA MEMBERS ------------------------------------

		private:
			WorldView&	_host;
		};

	// ---

		new(subtaskAllocator, Allocator::AllocationOption::TEMPORARY_ALLOCATION) BroadcastPacketsTask( *this, executingContext, visitingTask );
	}

// ---------------------------------------------------

	void WorldView::AcceptViewVisitor( ScriptMessageSink& /*messageSink*/ ) {
		// Broadcast join/leave notifications, etc.
	}

}	// namespace Steamworks
}	// namespace Networking
}	// namespace Eldritch2