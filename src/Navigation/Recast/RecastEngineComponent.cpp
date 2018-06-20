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
#include <microprofile/microprofile.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Assets;
using namespace ::Eldritch2::Core;

namespace Eldritch2 {
	namespace Navigation {
		namespace Recast {

			RecastEngineComponent::RecastEngineComponent(const Blackboard& services) : EngineComponent(services) {}

		// ---------------------------------------------------

			Result<UniquePointer<WorldComponent>> RecastEngineComponent::CreateWorldComponent(Allocator& allocator, const World& world) {
				UniquePointer<WorldComponent>	recastComponent(MakeUnique<RecastWorldComponent>(allocator, world));

				if (!recastComponent) {
					return Error::OutOfMemory;
				}

				return eastl::move(recastComponent);
			}

		// ---------------------------------------------------

			void RecastEngineComponent::AcceptVisitor(PropertyRegistrar& /*properties*/) {
				MICROPROFILE_SCOPEI("Engine/Initialization", "Recast property registration", 0xBBBBBB);
			}

		// ---------------------------------------------------

			void RecastEngineComponent::AcceptVisitor(AssetApiBuilder& /*api*/) {
				MICROPROFILE_SCOPEI("Engine/Initialization", "Recast asset factory registration", 0xBBBBBB);
			}

		}	// namespace Recast
	}	// namespace Navigation
}	// namespace Eldritch2