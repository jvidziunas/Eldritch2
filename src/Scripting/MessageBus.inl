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

	template <typename Message>
	ETInlineHint void MessageBus::Dispatch( ScriptTypeToken<Message> typeToken, ObjectId destinationId, ObjectId senderId, Eldritch2::uint64 time, const Message& message ) {
		SendMessage( static_cast<TypeId>(typeToken), destinationId, senderId, time, &message );
	}

}	// namespace Scripting
}	// namespace Eldritch2



