/*==================================================================*\
  BytecodePackageResourceView.inl
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
namespace AngelScript {

	ETInlineHint const ModuleHandle& BytecodePackageResourceView::GetScriptModule() const {
		return _module;
	}

}	// namespace AngelScript
}	// namespace Scripting
}	// namespace Eldritch2
