/*==================================================================*\
  Physics.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/PhysX/Physics.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Physics { namespace PhysX {

	using namespace ::Eldritch2::Physics::PhysX::AssetViews;

	Physics::Physics(const PhysicsAsset& asset, PhysicsClip& clip) ETNoexceptHint : _asset(ETAddressOf(asset)), _clip(ETAddressOf(clip)) {}

}}} // namespace Eldritch2::Physics::PhysX
