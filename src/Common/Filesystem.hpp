/*==================================================================*\
  FileSystem.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Containers/PlatformString.hpp>
#include <Common/Mpl/IntTypes.hpp>
#include <Common/Function.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	class	ErrorCode;
}

namespace Eldritch2 {

	enum class KnownDirectory : uint32 {
		UserDocuments,
		AppDataLocal,
		AppDataShared,
		Packages,
		DownloadedPackages,
		WorkingDirectory,
		Logs
	};

// ---

	enum class OverwriteBehavior : uint32 {
		SkipIfExists,
		OverwriteIfExists
	};

// ---

	class FileSystem {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Disable copy construction.
		FileSystem( const FileSystem& ) = delete;
	//!	Constructs this @ref FileSystem instance. 
		FileSystem();

		~FileSystem() = default;

	// ---------------------------------------------------

	public:
		ETPureFunctionHint const PlatformString<>&	GetPath( KnownDirectory directory ) const;

		ETPureFunctionHint PlatformString<>			GetAbsolutePath( KnownDirectory directory, const Utf8Char* path ) const;

	// ---------------------------------------------------

	public:
		ErrorCode	EnumerateMatchingFiles( KnownDirectory directory, const Utf8Char* const path, Function<void ( const Utf8Char* /*path*/ )> handler ) const;

	// ---------------------------------------------------

	public:
		void	Copy( KnownDirectory destinationDirectory, const Utf8Char* const destinationPath, KnownDirectory sourceDirectory, const Utf8Char* const sourcePath, OverwriteBehavior overwriteBehavior );
		void	Copy( KnownDirectory directory, const Utf8Char* const destinationPath, const Utf8Char* const sourcePath, OverwriteBehavior overwriteBehavior );

		void	Delete( KnownDirectory directory, const Utf8Char* const path );

	// ---------------------------------------------------

	//!	Disable copy assignment.
		FileSystem&	operator=( const FileSystem& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		const PlatformString<>	_workingDirectory;
		const PlatformString<>	_userDocumentsDirectory;
		const PlatformString<>	_localAppDataDirectory;
		const PlatformString<>	_sharedAppDataDirectory;
		const PlatformString<>	_packageDirectory;
		const PlatformString<>	_downloadedPackageDirectory;
		const PlatformString<>	_logDirectory;
	};

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/FileSystem.inl>
//------------------------------------------------------------------//