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

	ETInlineHint ETForceInlineHint ChildLog::ChildLog() ETNoexceptHint : _parent(nullptr) {}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void ChildLog::Write(const Utf8Char* string, size_t lengthInOctets) ETNoexceptHint {
		_parent->Write(string, lengthInOctets);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint ErrorCode ChildLog::BindResources(Log& parent) {
		_parent = ETAddressOf(parent);
		return Error::None;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void ChildLog::FreeResources() {
		_parent = nullptr;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void Swap(ChildLog& lhs, ChildLog& rhs) {
		using ::Eldritch2::Swap;

		Swap(lhs._parent, rhs._parent);
	}

}} // namespace Eldritch2::Logging
