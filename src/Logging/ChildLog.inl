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

	ETInlineHint ETForceInlineHint ChildLog::ChildLog(Log& parent) ETNoexceptHint : _parent(ETAddressOf(parent)) {}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void ChildLog::Write(const Utf8Char* string, size_t lengthInOctets) ETNoexceptHint {
		_parent->Write(string, lengthInOctets);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint Log* ChildLog::SetParent(Log& parent) ETNoexceptHint {
		return eastl::exchange(_parent, ETAddressOf(parent));
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint Log* ChildLog::GetParent() const ETNoexceptHint {
		return _parent;
	}

}} // namespace Eldritch2::Logging
