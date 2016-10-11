/*==================================================================*\
  BytecodePackageView.inl
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
namespace AssetViews {

	ETInlineHint const AngelScript::BytecodeMetadata& BytecodePackageView::GetMetadata() const {
		return _metadata;
	}

// ---------------------------------------------------

	ETInlineHint const asIScriptModule& BytecodePackageView::GetScriptModule() const {
		return *_module;
	}

}	// namespace AssetViews
}	// namespace AngelScript
}	// namespace Scripting
}	// namespace Eldritch2
