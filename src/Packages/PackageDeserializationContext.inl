/*==================================================================*\
  PackageDeserializationContext.inl
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

	ETInlineHint const FileSystem::ContentPackage& PackageDeserializationContext::GetBoundPackage() const {
		return *_packageReference;
	}

// ---------------------------------------------------

	ETInlineHint FileSystem::ContentPackage& PackageDeserializationContext::GetBoundPackage() {
		return *_packageReference;
	}

}	// namespace FileSystem
}	// namespace Eldritch2