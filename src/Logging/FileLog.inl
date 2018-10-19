/*==================================================================*\
  FileLog.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  �2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 { namespace Logging {

	ETInlineHint ETForceInlineHint void Swap(FileLog& lhs, FileLog& rhs) {
		Swap(lhs._appender, rhs._appender);
	}

}} // namespace Eldritch2::Logging