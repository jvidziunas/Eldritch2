/*==================================================================*\
  WrenEngineComponent.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Core/EngineComponent.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Scripting { namespace Wren {

	class WrenEngineComponent : public Core::EngineComponent {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref WrenEngineComponent instance.
		WrenEngineComponent(const Blackboard& services);
		//!	Disable copy construction.
		WrenEngineComponent(const WrenEngineComponent&) = delete;

		~WrenEngineComponent() = default;

		// - ENGINE SERVICE SANDBOX METHODS ------------------

	public:
		Result<UniquePointer<Core::WorldComponent>> CreateWorldComponent(Allocator& allocator, const Core::World& world) override;

		void AcceptVisitor(Assets::AssetApiBuilder& factories) override;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		WrenEngineComponent& operator=(const WrenEngineComponent&) = delete;
	};

}}} // namespace Eldritch2::Scripting::Wren
