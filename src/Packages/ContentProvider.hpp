/*==================================================================*\
  ContentProvider.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/Noncopyable.hpp>
#include <Utility/MPL/CharTypes.hpp>
#include <Utility/MPL/IntTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Configuration {
		class	ConfigurationDatabase;
	}

	namespace FileSystem {
		class	ReadableMemoryMappedFile;
		class	SynchronousFileAppender;
		class	AsynchronousFileReader;
		class	AsynchronousFileWriter;
		class	SynchronousFileReader;
		class	SynchronousFileWriter;
	}

	namespace Scheduler {
		class	TaskScheduler;
	}

	namespace Utility {
		template <typename ResultObjectType>
		struct	ResultPair;
	}

	class	Allocator;
	class	ErrorCode;
	template <typename IteratorType>
	class	Range;
}

namespace Eldritch2 {
namespace FileSystem {

	namespace Configuration	= ::Eldritch2::Configuration;
	namespace FileSystem	= ::Eldritch2::FileSystem;
	namespace Scheduler		= ::Eldritch2::Scheduler;
	namespace Utility		= ::Eldritch2::Utility;

// ---------------------------------------------------

	class ETPureAbstractHint ContentProvider : private Utility::Noncopyable {
	// - TYPE PUBLISHING ---------------------------------

	public:
		enum class KnownContentLocation : ::Eldritch2::uint32 {
			USER_DOCUMENTS,
			APPDATA_LOCAL,
			APPDATA_SHARED,
			PACKAGE_DIRECTORY,
			DOWNLOADED_CONTENT_DIRECTORY,

		// ---

			COUNT
		};

		enum class FileOverwriteBehavior : ::Eldritch2::uint32 {
			FAIL_IF_FILE_EXISTS,
			OVERWRITE_IF_FILE_EXISTS,

		// ---

			DEFAULT = OVERWRITE_IF_FILE_EXISTS
		};

		enum class PackageOverrideBehavior : ::Eldritch2::uint32 {
			ALLOW_OVERRIDE,
			DISABLE_OVERRIDE,
			FORCE_OVERRIDE,

		// ---

			DEFAULT = ALLOW_OVERRIDE
		};

	// - FILE READ ACCESS --------------------------------

		// Creates an asynchronous file reader object that can be used to access the contents of a file/stream in such a way that other tasks can (optionally)
		// complete while waiting for the backing device to finish the request.
		template <typename KnownContentLocationIterator>
		Utility::ResultPair<FileSystem::AsynchronousFileReader>			CreateAsynchronousFileReader( ::Eldritch2::Allocator& allocator, const ::Eldritch2::Range<KnownContentLocationIterator>& locations, const ::Eldritch2::UTF8Char* const fileName );
		virtual Utility::ResultPair<FileSystem::AsynchronousFileReader>	CreateAsynchronousFileReader( ::Eldritch2::Allocator& allocator, const KnownContentLocation contentLocation, const ::Eldritch2::UTF8Char* const fileName ) abstract;
		
		// Creates an synchronous file reader object that can be used to access the contents of a file/stream without requiring that the calling thread be able to
		// complete additional work while the underlying device operates.
		template <typename KnownContentLocationIterator>
		Utility::ResultPair<FileSystem::SynchronousFileReader>			CreateSynchronousFileReader( ::Eldritch2::Allocator& allocator, const ::Eldritch2::Range<KnownContentLocationIterator>& locations, const ::Eldritch2::UTF8Char* const fileName );
		virtual Utility::ResultPair<FileSystem::SynchronousFileReader>	CreateSynchronousFileReader( ::Eldritch2::Allocator& allocator, const KnownContentLocation contentLocation, const ::Eldritch2::UTF8Char* const fileName ) abstract;

		
		// Creates a memory-mapped file object that can be read from (but not written to).
		template <typename KnownContentLocationIterator>
		Utility::ResultPair<FileSystem::ReadableMemoryMappedFile>			CreateReadableMemoryMappedFile( ::Eldritch2::Allocator& allocator, const ::Eldritch2::Range<KnownContentLocationIterator> locations, const ::Eldritch2::UTF8Char* const fileName );
		virtual Utility::ResultPair<FileSystem::ReadableMemoryMappedFile>	CreateReadableMemoryMappedFile( ::Eldritch2::Allocator& allocator, const KnownContentLocation contentLocation, const ::Eldritch2::UTF8Char* const fileName ) abstract;

	// - FILE WRITE ACCESS -------------------------------

		// Creates an asynchronous file writer object that can be used to access the contents of a file/stream in such a way that other tasks can (optionally)
		// complete while waiting for the backing device to finish the request.
		template <typename KnownContentLocationIterator>
		Utility::ResultPair<FileSystem::AsynchronousFileWriter>			CreateAsynchronousFileWriter( ::Eldritch2::Allocator& allocator, const ::Eldritch2::Range<KnownContentLocationIterator>& locations, const ::Eldritch2::UTF8Char* const fileName, const FileOverwriteBehavior overwriteBehavior = FileOverwriteBehavior::DEFAULT );
		virtual Utility::ResultPair<FileSystem::AsynchronousFileWriter>	CreateAsynchronousFileWriter( ::Eldritch2::Allocator& allocator, const KnownContentLocation contentLocation, const ::Eldritch2::UTF8Char* const fileName, const FileOverwriteBehavior overwriteBehavior = FileOverwriteBehavior::DEFAULT ) abstract;

		// Creates an synchronous file writer object that can be used to access the contents of a file/stream without requiring that the calling thread be able to
		// complete additional work while the underlying device operates.
		template <typename KnownContentLocationIterator>
		Utility::ResultPair<FileSystem::SynchronousFileWriter>			CreateSynchronousFileWriter( ::Eldritch2::Allocator& allocator, const ::Eldritch2::Range<KnownContentLocationIterator>& locations, const ::Eldritch2::UTF8Char* const fileName, const FileOverwriteBehavior overwriteBehavior = FileOverwriteBehavior::DEFAULT );
		virtual Utility::ResultPair<FileSystem::SynchronousFileWriter>	CreateSynchronousFileWriter( ::Eldritch2::Allocator& allocator, const KnownContentLocation contentLocation, const ::Eldritch2::UTF8Char* const fileName, const FileOverwriteBehavior overwriteBehavior = FileOverwriteBehavior::DEFAULT ) abstract;

	// - FILE APPEND ACCESS ------------------------------

		template <typename KnownContentLocationIterator>
		Utility::ResultPair<FileSystem::SynchronousFileAppender>			CreateSynchronousFileAppender( ::Eldritch2::Allocator& allocator, const ::Eldritch2::Range<KnownContentLocationIterator>& locations, const ::Eldritch2::UTF8Char* const fileName );
		virtual Utility::ResultPair<FileSystem::SynchronousFileAppender>	CreateSynchronousFileAppender( ::Eldritch2::Allocator& allocator, const KnownContentLocation contentLocation, const ::Eldritch2::UTF8Char* const fileName ) abstract;

	// - FREE (UNPACKAGED) FILE MANIPULATION -------------

		virtual void	CopyFreeFile( const KnownContentLocation contentLocation, const ::Eldritch2::UTF8Char* const destinationFileName, const ::Eldritch2::UTF8Char* const sourceFileName, const FileOverwriteBehavior overwriteBehavior = FileOverwriteBehavior::DEFAULT ) abstract;

		virtual void	DeleteFreeFile( const KnownContentLocation contentLocation, const ::Eldritch2::UTF8Char* const fileName ) abstract;

	// ---------------------------------------------------

		virtual ::Eldritch2::ErrorCode	RegisterConfigurableVariables( Configuration::ConfigurationDatabase& configurationDatabase );

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
		// Constructs this ContentProvider instance. 
		ContentProvider();

		// Destroys this ContentProvider instance.
		~ContentProvider();
	};

}	// namespace FileSystem
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Packages/ContentProvider.inl>
//------------------------------------------------------------------//