/*==================================================================*\
  WorldComponent.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Core/WorldComponent.hpp>
//------------------------------------------------------------------//
#include <microprofile/microprofile.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Core {

		using namespace ::Eldritch2::Scheduling;
		using namespace ::Eldritch2::Scripting;

		void WorldComponent::AcceptVisitor(JobExecutor& /*executor*/, const EarlyInitializationVisitor) {
			MICROPROFILE_SCOPEI("World/Initialization", "Default early initialization function", 0xA011F0);

		//	Default implementation should not do anything.
		}

	// ---------------------------------------------------

		void WorldComponent::AcceptVisitor(JobExecutor& /*executor*/, const LateInitializationVisitor) {
			MICROPROFILE_SCOPEI("World/LateInitialization", "Default late initialization function", 0xA011F0);

		//	Default implementation should not do anything.
		}

	// ---------------------------------------------------

		void WorldComponent::AcceptVisitor(JobExecutor& /*executor*/, const VariableTickVisitor& /*visitor*/) {
			MICROPROFILE_SCOPEI("World/VariableTick", "Default variable-rate tick function", 0xA011F0);

		//	Default implementation should not do anything.
		}

	// ---------------------------------------------------

		void WorldComponent::AcceptVisitor(JobExecutor& /*executor*/, const EarlyTickVisitor&) {
			MICROPROFILE_SCOPEI("World/EarlyTick", "Default early tick function", 0xA011F0);

		//	Default implementation should not do anything.
		}

	// ---------------------------------------------------

		void WorldComponent::AcceptVisitor(JobExecutor& /*executor*/, const StandardTickVisitor&) {
			MICROPROFILE_SCOPEI("World/Tick", "Default standard tick function", 0xA011F0);

		//	Default implementation should not do anything.
		}

	// ---------------------------------------------------

		void WorldComponent::AcceptVisitor(JobExecutor& /*executor*/, const LateTickVisitor&) {
			MICROPROFILE_SCOPEI("World/LateTick", "Default late tick function", 0xA011F0);

		//	Default implementation should not do anything.
		}

	// ---------------------------------------------------

		void WorldComponent::AcceptVisitor(JobExecutor& /*executor*/, const TearDownVisitor) {
			MICROPROFILE_SCOPEI("World/TearDown", "Default tear down function", 0xA011F0);

		//	Default implementation should not do anything.
		}

	// ---------------------------------------------------

		void WorldComponent::AcceptVisitor(Wren::ApiBuilder& /*api*/) {
			MICROPROFILE_SCOPEI("World/Initialization", "Default Wren script API registration", 0xBBBBBB);

		//	Default implementation should not do anything.
		}

	// ---------------------------------------------------

		void WorldComponent::AcceptVisitor(Blackboard& /*services*/) {
			MICROPROFILE_SCOPEI("World/Initialization", "Default service attach function", 0xA011F0);

		//	Default implementation should not do anything.
		}

	}	// namespace Core
}	// namespace Eldritch2