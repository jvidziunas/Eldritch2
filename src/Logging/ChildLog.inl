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

	ETInlineHint Logging::Log& ChildLog::SetParent( Logging::Log& newParent ) {
		auto&	oldLogger( GetParent() );

		_parent = &newParent;

		return oldLogger;
	}

// ---------------------------------------------------

	ETInlineHint Logging::Log& ChildLog::GetParent() const {
		return *_parent;
	}

}	// namespace Logging
}	// namespace Eldritch2

