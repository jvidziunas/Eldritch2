/*==================================================================*\
  World.inl
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
namespace Foundation {

	namespace Foundation	= ::Eldritch2::Foundation;
	namespace FileSystem	= ::Eldritch2::FileSystem;
	namespace Scheduler		= ::Eldritch2::Scheduler;

// ---------------------------------------------------

	ETInlineHint const FileSystem::ContentPackage& World::GetRootPackage() const {
		return *static_cast<const FileSystem::ContentPackage*>(_package);
	}

// ---------------------------------------------------

	ETInlineHint ::Eldritch2::ErrorCode World::GetLastError() const {
		return _lastError;
	}

}	// namespace Foundation
}	// namespace Eldritch2