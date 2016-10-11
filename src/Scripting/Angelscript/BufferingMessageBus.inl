/*==================================================================*\
  MessageBus.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scripting {
namespace AngelScript {

	ETInlineHint void BufferingMessageBus::NotifyOfObjectDestruction( asIScriptObject* const object ) {
		_queueDirectory.Erase( object );
	}

}	// namespace AngelScript
}	// namespace Scripting
}	// namespace Eldritch2