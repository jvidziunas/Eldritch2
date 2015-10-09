/*==================================================================*\
  ResourceView.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2012 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
namespace FileSystem {

	namespace FileSystem	= ::Eldritch2::FileSystem;
	namespace Utility		= ::Eldritch2::Utility;

// ---------------------------------------------------

	ETInlineHint const ::Eldritch2::UTF8String<>& ResourceView::GetName() const {
		return _name;
	}

}	// namespace FileSystem
}	// namespace Eldritch2