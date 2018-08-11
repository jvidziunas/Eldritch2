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
		//!	Constructs this @ref RecastEngineComponent instance.
		RecastEngineComponent(const ObjectLocator& services);
		//!	Disable copy construction.
		RecastEngineComponent(const RecastEngineComponent&) = delete;

		~RecastEngineComponent() = default;

		// ---------------------------------------------------

	public:
		Result<UniquePointer<Core::WorldComponent>> CreateWorldComponent(Allocator& allocator, const ObjectLocator& services) override;

		// ---------------------------------------------------

	public:
		void PublishConfiguration(Core::PropertyRegistrar& properties) override;

		void PublishAssetTypes(Assets::AssetApiBuilder& factories) override;
	};

}}} // namespace Eldritch2::Navigation::Recast
