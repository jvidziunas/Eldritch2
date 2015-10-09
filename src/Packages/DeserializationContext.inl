/*==================================================================*\
  DeserializationContext.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
namespace FileSystem {

	ETInlineHint const FileSystem::ContentPackage& DeserializationContext::GetBoundPackage() const {
		return *_packageReference;
	}

// ---------------------------------------------------

	ETInlineHint FileSystem::ContentPackage& DeserializationContext::GetBoundPackage() {
		return *_packageReference;
	}

}	// namespace FileSystem
}	// namespace Eldritch2