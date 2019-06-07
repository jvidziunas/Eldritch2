/*==================================================================*\
  RecastEngineComponent.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Navigation/Recast/RecastEngineComponent.hpp>
#include <Core/AssetDatabase.hpp>
//------------------------------------------------------------------//


namespace Eldritch2 { namespace Navigation { namespace Recast {

	using namespace ::Eldritch2::Core;

	// ---------------------------------------------------

	RecastEngineComponent::RecastEngineComponent(const ObjectInjector& services) ETNoexceptHint : EngineComponent(services) {}

	// ---------------------------------------------------

	void RecastEngineComponent::PublishApi(PropertyApiBuilder& /*api*/) {}

	// ---------------------------------------------------

	void RecastEngineComponent::PublishApi(AssetApiBuilder& /*api*/) {}

}}} // namespace Eldritch2::Navigation::Recast
