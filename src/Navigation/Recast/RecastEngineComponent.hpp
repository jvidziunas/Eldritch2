/*==================================================================*\
  RecastEngineComponent.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Core/EngineComponent.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Navigation { namespace Recast {

	class RecastEngineComponent : public Core::EngineComponent {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		RecastEngineComponent(const RecastEngineComponent&) = delete;
		//!	Constructs this @ref RecastEngineComponent instance.
		RecastEngineComponent(const Blackboard& services);

		~RecastEngineComponent() = default;

		// ---------------------------------------------------

	public:
		Result<UniquePointer<Core::WorldComponent>> CreateWorldComponent(Allocator& allocator, const Core::World& world) override;

		// ---------------------------------------------------

	public:
		void AcceptVisitor(Core::PropertyRegistrar& properties) override;
		void AcceptVisitor(Assets::AssetApiBuilder& factories) override;
	};

}}} // namespace Eldritch2::Navigation::Recast
