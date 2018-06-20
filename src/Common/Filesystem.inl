/*==================================================================*\
  FileSystem.inl
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Assert.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

ETInlineHint void FileSystem::Copy(KnownDirectory directory, const Utf8Char* const destinationFileName, const Utf8Char* const sourceFileName, OverwriteBehavior overwriteBehavior) {
	FileSystem::Copy(directory, destinationFileName, directory, sourceFileName, overwriteBehavior);
}

// ---------------------------------------------------

ETInlineHint void FileSystem::Move(KnownDirectory directory, const Utf8Char* const destinationFileName, const Utf8Char* const sourceFileName) {
	FileSystem::Move(directory, destinationFileName, directory, sourceFileName);
}

// ---------------------------------------------------

ETInlineHint ETPureFunctionHint const PlatformString<>& FileSystem::GetPath(KnownDirectory directory) const {
	switch (directory) {
	case KnownDirectory::UserDocuments: return _userDocumentsDirectory;
	case KnownDirectory::AppDataLocal: return _localAppDataDirectory;
	case KnownDirectory::AppDataShared: return _sharedAppDataDirectory;
	case KnownDirectory::Packages: return _packageDirectory;
	case KnownDirectory::DownloadedPackages: return _downloadedPackageDirectory;
	case KnownDirectory::WorkingDirectory: return _workingDirectory;
	case KnownDirectory::Logs:
		return _logDirectory;
		ET_NO_DEFAULT_CASE;
	} // switch( directory )
}

} // namespace Eldritch2
