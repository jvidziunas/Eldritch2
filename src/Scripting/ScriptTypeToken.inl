/*==================================================================*\
  ScriptTypeToken.inl
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
namespace Detail {

	ETInlineHint ScriptTypeToken::ScriptTypeToken( RawToken value ) : _value( value ) {}

// ---------------------------------------------------

	ETInlineHint ScriptTypeToken::operator RawToken() const {
		return _value;
	}

}	// namespace Detail

	template <typename Native>
	ETInlineHint ScriptTypeToken<Native>::ScriptTypeToken( RawToken token ) : Detail::ScriptTypeToken( token ) {}

}	// namespace Scripting
}	// namespace Eldritch2

