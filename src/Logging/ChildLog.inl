/*==================================================================*\
  ChildLog.inl
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
namespace Logging {

	ETInlineHint ChildLog::ChildLog( Log& logger ) : _parent( &logger ) {}

// ---------------------------------------------------

	ETInlineHint Log& ChildLog::SetParent( Log& parent ) {
		auto&	oldLogger( GetParent() );

		_parent = &parent;

		return oldLogger;
	}

// ---------------------------------------------------

	ETInlineHint Log& ChildLog::GetParent() const {
		return *_parent;
	}

}	// namespace Logging
}	// namespace Eldritch2

