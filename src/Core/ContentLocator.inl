/*==================================================================*\
  ContentLocator.inl
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 { namespace Core {

	ETConstexpr ETForceInlineHint ContentLocator::ContentLocator(ContentDatabase& content) ETNoexceptHint : _content(ETAddressOf(content)) {}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint ContentDatabase& ContentLocator::GetContent() const ETNoexceptHint {
		return *_content;
	}

}} // namespace Eldritch2::Core
