/*==================================================================*\
  Win32ContentProvider.hpp
  ------------------------------------------------------------------
  Purpose:
  Windows implementation for the content provider interface. This
  object translates high-level calls to read/serialize content from
  the hard disk into low-level API commands for paging file contents
  into the running process' address space.

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Packages/ContentProvider.hpp>
//------------------------------------------------------------------//
#include <Windows.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Foundation {
		class	Win32SystemInterface;
	}

	namespace Scheduler {
		class	TaskScheduler;
	}
}

namespace Eldritch2 {
namespace FileSystem {

	namespace FileSystem	= ::Eldritch2::FileSystem;
	namespace Foundation	= ::Eldritch2::Foundation;
	namespace Scheduler		= ::Eldritch2::Scheduler;
	namespace Utility		= ::Eldritch2::Utility;

// ---------------------------------------------------

	class Win32ContentProvider : public FileSystem::ContentProvider {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		// Constructs this Win32ContentProvider instance.
		Win32ContentProvider( const Foundation::Win32SystemInterface& systemInterface );

		// Destroys this Win32ContentProvider instance.
		~Win32ContentProvider();

	// - FILE READ ACCESS --------------------------------

		// Creates an asynchronous file reader object that can be used to access the contents of a file/stream in such a way that other tasks can (optionally)
		// complete while waiting for the backing device to finish the request.
		using ContentProvider::CreateAsynchronousFileReader;
		Utility::ResultPair<FileSystem::AsynchronousFileReader>		CreateAsynchronousFileReader( ::Eldritch2::Allocator& allocator, const ContentProvider::KnownContentLocation contentLocation, const ::Eldritch2::UTF8Char* const fileName ) override;

		// Creates an synchronous file reader object that can be used to access the contents of a file/stream without requiring that the calling thread be able to
		// complete additional work while the underlying device operates.
		using ContentProvider::CreateSynchronousFileReader;
		Utility::ResultPair<FileSystem::SynchronousFileReader>		CreateSynchronousFileReader( ::Eldritch2::Allocator& allocator, const ContentProvider::KnownContentLocation contentLocation, const ::Eldritch2::UTF8Char* const fileName ) override;

		// Creates a memory-mapped file object that can be read from (but not written to).
		using ContentProvider::CreateReadableMemoryMappedFile;
		Utility::ResultPair<FileSystem::ReadableMemoryMappedFile>	CreateReadableMemoryMappedFile( ::Eldritch2::Allocator& allocator, const ContentProvider::KnownContentLocation contentLocation, const ::Eldritch2::UTF8Char* const fileName ) override;

	// - FILE WRITE ACCESS -------------------------------

		// Creates an asynchronous file writer object that can be used to access the contents of a file/stream in such a way that other tasks can (optionally)
		// complete while waiting for the backing device to finish the request.
		using ContentProvider::CreateAsynchronousFileWriter;
		Utility::ResultPair<FileSystem::AsynchronousFileWriter>	CreateAsynchronousFileWriter( ::Eldritch2::Allocator& allocator, const ContentProvider::KnownContentLocation contentLocation, const ::Eldritch2::UTF8Char* const fileName, const FileOverwriteBehavior overwriteBehavior ) override;

		// Creates an synchronous file writer object that can be used to access the contents of a file/stream without requiring that the calling thread be able to
		// complete additional work while the underlying device operates.
		using ContentProvider::CreateSynchronousFileWriter;
		Utility::ResultPair<FileSystem::SynchronousFileWriter>	CreateSynchronousFileWriter( ::Eldritch2::Allocator& allocator, const ContentProvider::KnownContentLocation contentLocation, const ::Eldritch2::UTF8Char* const fileName, const FileOverwriteBehavior overwriteBehavior ) override;

	// - FILE APPEND ACCESS ------------------------------

		using ContentProvider::CreateSynchronousFileAppender;
		Utility::ResultPair<FileSystem::SynchronousFileAppender>	CreateSynchronousFileAppender( ::Eldritch2::Allocator& allocator, const ContentProvider::KnownContentLocation contentLocation, const ::Eldritch2::UTF8Char* const fileName ) override;

	// - FREE (UNPACKAGED) FILE MANIPULATION -------------

		void	CopyFreeFile( const ContentProvider::KnownContentLocation contentLocation, const ::Eldritch2::UTF8Char* const destinationFileName, const ::Eldritch2::UTF8Char* const sourceFileName, const ContentProvider::FileOverwriteBehavior overwriteBehavior ) override;

		void	DeleteFreeFile( const ContentProvider::KnownContentLocation contentLocation, const ::Eldritch2::UTF8Char* const fileName ) override;

	// ---------------------------------------------------

	private:
		typedef size_t (*GetSectorSizeFromHandleFunction)(::HANDLE);
		typedef wchar_t	KnownContentPath[MAX_PATH];

	// ---------------------------------------------------

		ETInlineHint const wchar_t*	PathFromKnownContentLocation( ContentProvider::KnownContentLocation location ) const;

	// - DATA MEMBERS ------------------------------------

		const GetSectorSizeFromHandleFunction	_getSectorSizeFromHandleFunction;
		KnownContentPath						_packageContentPath;
		KnownContentPath						_knownContentPaths[static_cast<size_t>(ContentProvider::KnownContentLocation::COUNT)];
	};

}	// namespace Foundation
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Packages/Win32ContentProvider.inl>
//------------------------------------------------------------------//