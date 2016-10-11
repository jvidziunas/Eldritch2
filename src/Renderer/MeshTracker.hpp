/*==================================================================*\
  MeshTrackingSystem.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/ConcurrentCounterHash.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Renderer {

	template <typename MeshAsset>
	using MeshTracker = Eldritch2::ConcurrentCounterHash<const MeshAsset*, Eldritch2::uint32>;

}	// namespace Renderer
}	// namespace Eldritch2