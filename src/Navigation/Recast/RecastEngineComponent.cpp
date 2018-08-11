/*==================================================================*\
  RecastEngineComponent.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Navigation/Recast/RecastEngineComponent.hpp>
#include <Navigation/Recast/RecastWorldComponent.hpp>
#include <Assets/AssetDatabase.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Assets;
using namespace ::Eldritch2::Core;

namespace Eldritch2 { namespace Navigation { namespace Recast {

	RecastEngineComponent::RecastEngineComponent(const ObjectLocator& services) :
		EngineComponent(services) {}

	// ---------------------------------------------------

	Result<UniquePointer<WorldComponent>> RecastEngineComponent::CreateWorldComponent(Allocator& allocator, const ObjectLocator& services) {
		UniquePointer<WorldComponent> recastComponent(MakeUnique<RecastWorldComponent>(allocator, services));
		if (!recastComponent) {
			return Error::OutOfMemory;
		}

		return eastl::move(recastComponent);
	}

	// ---------------------------------------------------

	void RecastEngineComponent::PublishConfiguration(PropertyRegistrar& /*properties*/) {
		ET_PROFILE_SCOPE("Engine/Initialization", "Recast property registration", 0xBBBBBB);
	}

	// ---------------------------------------------------

	void RecastEngineComponent::PublishAssetTypes(AssetApiBuilder& /*api*/) {
		ET_PROFILE_SCOPE("Engine/Initialization", "Recast asset factory registration", 0xBBBBBB);
	}

}}} // namespace Eldritch2::Navigation::Recast
