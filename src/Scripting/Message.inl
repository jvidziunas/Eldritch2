/*==================================================================*\
  Message.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scripting {

	ETInlineHint const ::Eldritch2::UTF8Char* const Message::GetTypeName() const {
		return _typeName;
	}

// ---------------------------------------------------

	ETInlineHint void* Message::GetPayload() const {
		return nullptr;
	}

}	// namespace Scripting
}	// namespace Eldritch2