/*==================================================================*\
  ContentProvider.inl
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
namespace Platform {

	ETInlineHint void ContentProvider::CopyFreeFile( WellKnownDirectory directory, const Eldritch2::Utf8Char* const destinationFileName, const Eldritch2::Utf8Char* const sourceFileName, OverwriteBehavior overwriteBehavior ) {
		CopyFreeFile( directory, destinationFileName, directory, sourceFileName, overwriteBehavior );
	}

// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint const Eldritch2::String<Eldritch2::SystemChar>& ContentProvider::GetPathFromSpecifier( WellKnownDirectory directory ) const {
		switch( directory ) {
			case WellKnownDirectory::UserDocuments:			return _userDocumentsDirectory;
			case WellKnownDirectory::AppDataLocal:			return _localAppDataDirectory;
			case WellKnownDirectory::AppDataShared:			return _sharedAppDataDirectory;
			case WellKnownDirectory::Packages:				return _packageDirectory;
			case WellKnownDirectory::DownloadedPackages:	return _downloadedPackageDirectory;
			case WellKnownDirectory::WorkingDirectory:		return _workingDirectory;
			case WellKnownDirectory::Logs:					return _logDirectory;
		}	// switch( directory )
	}

}	// namespace Platform
}	// namespace Eldritch2