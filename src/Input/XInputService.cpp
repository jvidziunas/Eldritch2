/*==================================================================*\
  XInputService.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scheduler/CRTPTransientTask.hpp>
#include <Utility/Memory/InstanceNew.hpp>
#include <Input/XInputService.hpp>
#include <Utility/ErrorCode.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Foundation;
using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Scheduler;
using namespace ::Eldritch2::Input;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Input {

	XInputService::XInputService( GameEngine& owningEngine ) : GameEngineService( owningEngine ) {}

// ---------------------------------------------------

	XInputService::~XInputService() {}

// ---------------------------------------------------

	const UTF8Char* const XInputService::GetName() const {
		return UTF8L("XInput Service");
	}

// ---------------------------------------------------

	void XInputService::AcceptInitializationVisitor( ScriptAPIRegistrationInitializationVisitor& typeRegistrar ) {
		Controller::ExposeScriptAPI( typeRegistrar );
	}

// ---------------------------------------------------

	void XInputService::AcceptTaskVisitor( Allocator& subtaskAllocator, Task& visitingTask, WorkerContext& executingContext, const ServiceTickTaskVisitor ) {
		class SampleXInputTask : public CRTPTransientTask<SampleXInputTask> {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			// Constructs this SampleXInputTask instance.
			ETInlineHint SampleXInputTask( XInputService& host, Task& visitingTask, WorkerContext& executingContext ) : CRTPTransientTask<SampleXInputTask>( visitingTask, Scheduler::CodependentTaskSemantics ),
																														_host( host ) {
				TrySchedulingOnContext( executingContext );
			}

		// ---------------------------------------------------

			const UTF8Char* const GetHumanReadableName() const override {
				return UTF8L("Sample XInput Devices Task");
			}

			Task* Execute( WorkerContext& /*executingContext*/ ) override {
				for( auto& controller : _host._controllers ) {
					controller.ReadInput();
				}

				return nullptr;
			}

		// - DATA MEMBERS ------------------------------------

		private:
			XInputService&	_host;
		};

	// ---

		new(subtaskAllocator, Allocator::AllocationOption::TEMPORARY_ALLOCATION) SampleXInputTask( *this, visitingTask, executingContext );
	}

}	// namespace Input
}	// namespace Eldritch2