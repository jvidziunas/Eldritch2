/*==================================================================*\
  CollisionMessage.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/PhysX/ScriptMessages/CollisionMessage.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Scripting;

namespace Eldritch2 {
namespace Physics {
namespace PhysX {
namespace ScriptMessages {

	CollisionMessage::CollisionMessage( float32 force ) : force( force ) {}

}	// namespace ScriptMessages
}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2