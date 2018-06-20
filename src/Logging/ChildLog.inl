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

namespace Eldritch2 { namespace Logging {

	ETInlineHint ChildLog::ChildLog(Log& parent) :
		_parent(eastl::addressof(parent)) {}

	// ---------------------------------------------------

	ETInlineHint Log& ChildLog::SetParent(Log& parent) {
		return *eastl::exchange(_parent, eastl::addressof(parent));
	}

	// ---------------------------------------------------

	ETInlineHint Log& ChildLog::GetParent() const {
		return *_parent;
	}

}} // namespace Eldritch2::Logging
