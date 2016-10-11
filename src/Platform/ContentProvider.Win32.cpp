/*==================================================================*\
  ContentProvider.Win32.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Memory/StandardLibrary.hpp>
#include <Platform/ContentProvider.hpp>
#include <Utility/Memory/Allocator.hpp>
#include <Utility/UniquePointer.hpp>
#include <Build.hpp>
//------------------------------------------------------------------//
#include <Windows.h>
#include <ShLwApi.h>
#include <ShlObj.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Platform {
namespace {

	class TaskMemFreeDeleter {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref TaskMemFreeDeleter instance.
		ETInlineHint TaskMemFreeDeleter( size_t stringLengthInBytes ) : _stringLengthInBytes( stringLengthInBytes ) {}
	//!	Constructs this @ref TaskMemFreeDeleter instance.
		TaskMemFreeDeleter() = default;

		~TaskMemFreeDeleter() = default;

	// ---------------------------------------------------

		ETInlineHint ETPureFunctionHint void	operator()( void* const data ) {
			CoTaskMemFree( data );
		}

	// ---------------------------------------------------

		ETInlineHint size_t	GetSize() const {
			return _stringLengthInBytes;
		}

	// - DATA MEMBERS ------------------------------------

	private:
		size_t	_stringLengthInBytes;
	};

// ---

	static ETPureFunctionHint UniquePointer<SystemChar[], TaskMemFreeDeleter> GetKnownFolderPath( REFKNOWNFOLDERID folderId ) {
		wchar_t*	path( nullptr );

		if( FAILED( SHGetKnownFolderPath( folderId, 0, nullptr, &path ) ) ) {
			return nullptr;
		}

		return { path, TaskMemFreeDeleter( StringLength( path ) ) };
	}

// ---------------------------------------------------

	static ETPureFunctionHint String<SystemChar> GetUserDocumentsDirectory( Allocator& allocator ) {
	//	Retrieve the Documents library path for this machine.
		String<SystemChar>	result( { allocator, "User Documents Directory Path Allocator" } );
		const auto			basePath( GetKnownFolderPath( FOLDERID_Documents ) );

		result.Append( basePath.Begin(), basePath.End() ).Append( L"\\My Games\\" WPROJECT_NAME L"\\" );

	//	Ensure the directory exists.
		CreateDirectoryW( result, nullptr );

		return eastl::move( result );
	}

// ---------------------------------------------------

	static ETPureFunctionHint String<SystemChar> GetLocalAppDataDirectory( Allocator& allocator ) {
	//	Retrieve the AppDataLocal library path for this machine.
		String<SystemChar>	result( { allocator, "Local App Data Directory Path Allocator" } );
		const auto			basePath( GetKnownFolderPath( FOLDERID_LocalAppData ) );

		result.Append( basePath.Begin(), basePath.End() ).Append( L"\\" WPROJECT_NAME L"\\" );

	//	Ensure the directory exists.
		CreateDirectoryW( result, nullptr );

		return eastl::move( result );
	}

// ---------------------------------------------------

	static ETPureFunctionHint String<SystemChar> GetSharedAppDataDirectory( Allocator& allocator ) {
	//	Retrieve the AppDataRoaming library path for this machine.
		String<SystemChar>	result( { allocator, "Local App Data Directory Path Allocator" } );
		const auto			basePath( GetKnownFolderPath( FOLDERID_RoamingAppData ) );

		result.Append( basePath.Begin(), basePath.End() ).Append( L"\\" WPROJECT_NAME L"\\" );

	//	Ensure the directory exists.
		CreateDirectoryW( result, nullptr );

		return eastl::move( result );
	}

// ---------------------------------------------------

	static ETPureFunctionHint String<SystemChar> GetWorkingDirectory( Allocator& allocator ) {
		SystemChar	path[MAX_PATH];
		auto		resultLengthInCharacters( GetCurrentDirectoryW( static_cast<DWORD>(_countof( path )), path ) );

	//	Ensure the path ends with a directory separator.
		if( path[resultLengthInCharacters - 1] != L'\\' ) {
			Eldritch2::AppendString( path, L"\\" );
			++resultLengthInCharacters;
		}

		return String<SystemChar>( path, path + resultLengthInCharacters, { allocator, "Log Directory Path Allocator" } );
	}

// ---------------------------------------------------

	static ETPureFunctionHint String<SystemChar> GetPackageDirectory( Allocator& allocator, const String<SystemChar>& workingDirectoryPath ) {
		String<SystemChar>	result( { allocator, "Package Directory Path Allocator" } );

		result.Append( workingDirectoryPath ).Append( L"Logs\\" );

	//	Ensure the directory exists.
		CreateDirectoryW( result, nullptr );

		return eastl::move( result );
	}

// ---------------------------------------------------

	static ETPureFunctionHint String<SystemChar> GetDownloadedPackagesDirectoryPath( Allocator& allocator, const String<SystemChar>& documentsPath ) {
		String<SystemChar>	result( { allocator, "Downloaded Packages Directory Path Allocator" } );

		result.Append( documentsPath ).Append( L"DownloadedPackages\\" );

	//	Ensure the directory exists.
		CreateDirectoryW( result, nullptr );

		return eastl::move( result );
	}

// ---------------------------------------------------

	static ETPureFunctionHint String<SystemChar> GetLogDirectory( Allocator& allocator, const String<SystemChar>& documentsPath ) {
		String<SystemChar>	result( { allocator, "Log Directory Path Allocator" } );

		result.Append( documentsPath ).Append( L"Logs\\" );

	//	Ensure the directory exists.
		CreateDirectoryW( result, nullptr );

		return eastl::move( result );
	}

}	// anonymous namespace

	ContentProvider::ContentProvider( Allocator& allocator ) : _allocator( allocator, "Content Provider Allocator" ),
															   _workingDirectory( GetWorkingDirectory( _allocator ) ),
															   _userDocumentsDirectory( GetUserDocumentsDirectory( _allocator ) ),
															   _localAppDataDirectory( GetLocalAppDataDirectory( _allocator ) ),
															   _sharedAppDataDirectory( GetSharedAppDataDirectory( _allocator ) ),
															   _packageDirectory( GetPackageDirectory( _allocator, _workingDirectory ) ),
															   _downloadedPackageDirectory( GetDownloadedPackagesDirectoryPath( _allocator, _userDocumentsDirectory ) ),
															   _logDirectory( GetLogDirectory( _allocator, _userDocumentsDirectory ) ) {}

// ---------------------------------------------------

	void ContentProvider::CopyFreeFile( WellKnownDirectory destinationDirectory, const Utf8Char* const destinationFileName, WellKnownDirectory sourceDirectory, const Utf8Char* const sourceFileName, OverwriteBehavior overwriteBehavior ) {
		const auto	sourcePath( ExpandPath( sourceDirectory, sourceFileName ) );
		const auto	destinationPath( ExpandPath( destinationDirectory, destinationFileName ) );

		CopyFileW( sourcePath, destinationPath, OverwriteBehavior::SkipIfExists == overwriteBehavior ? TRUE : FALSE );
	}

// ---------------------------------------------------

	void ContentProvider::DeleteFreeFile( WellKnownDirectory directory, const Utf8Char* const fileName ) {
		const auto	path( ExpandPath( directory, fileName ) );

		DeleteFileW( path );
	}

// ---------------------------------------------------

	bool ContentProvider::CheckFileExists( WellKnownDirectory directory, const Utf8Char* const fileName ) {
		const auto	path( ExpandPath( directory, fileName ) );
		const DWORD	attributes( GetFileAttributesW( path ) );

		return attributes != INVALID_FILE_ATTRIBUTES && !( attributes & FILE_ATTRIBUTE_DIRECTORY );
	}

}	// namespace Platform
}	// namespace Eldritch2



