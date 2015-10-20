/*==================================================================*\
  Win32ContentProvider.hpp
  ------------------------------------------------------------------
  Purpose:
  Windows implementation for the content provider interface. This
  object translates high-level calls to read/serialize content from
  the hard disk into low-level API commands for paging file contents
  into the running process' address space.

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/AsynchronousFileReader.hpp>
#include <Utility/AsynchronousFileWriter.hpp>
#include <Utility/SynchronousFileReader.hpp>
#include <Utility/SynchronousFileWriter.hpp>
#include <Packages/ContentProvider.hpp>
//------------------------------------------------------------------//
#include <Windows.h>
//------------------------------------------------------------------//
#include <atomic>
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
	// - TYPE PUBLISHING ---------------------------------

	public:
		class SynchronousFileAccessor : public FileSystem::SynchronousFileReader, public FileSystem::SynchronousFileWriter {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//! Constructs this @ref SynchronousFileAccessor instance.
			SynchronousFileAccessor( const ::HANDLE fileHandle, const size_t mediaSectorSizeInBytes );

			//! Destroys this @ref SynchronousFileAccessor instance.
			~SynchronousFileAccessor();

		// ---------------------------------------------------

			SynchronousFileReader::BlockingResult	Read( void* const destinationBuffer, const size_t lengthToReadInBytes ) override sealed;
			SynchronousFileReader::BlockingResult	Read( void* const destinationBuffer, const size_t lengthToReadInBytes, const ::Eldritch2::uint64 fileOffsetInBytes ) override sealed;

		// ---------------------------------------------------

			SynchronousFileWriter::BlockingResult	Write( const void* const sourceBuffer, const size_t lengthToWriteInBytes ) override sealed;
			SynchronousFileWriter::BlockingResult	Write( const void* const sourceBuffer, const size_t lengthToWriteInBytes, const ::Eldritch2::uint64 fileOffsetInBytes ) override sealed;

		// ---------------------------------------------------

			void	AdvanceToEnd() override sealed;

		// ---------------------------------------------------

			void	SetFileSize( const ::Eldritch2::uint64 fileSizeInBytes ) override sealed;

		// ---------------------------------------------------

			uint64	GetFileCursorInBytes() const override sealed;

			uint64	GetSizeInBytes() const override sealed;

		// ---------------------------------------------------

			size_t	GetPhysicalMediaSectorSizeInBytes() const override sealed;

		// - DATA MEMBERS ------------------------------------

		private:
			const ::HANDLE	_fileHandle;
			const size_t	_mediaSectorSizeInBytes;
		};

	// ---

		class AsynchronousFileAccessor : public FileSystem::AsynchronousFileReader, public FileSystem::AsynchronousFileWriter {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//! Constructs this @ref AsynchronousFileAccessor instance.
			AsynchronousFileAccessor( const ::HANDLE fileHandle, const size_t mediaSectorSizeInBytes );

			//! Destroys this @ref Win32AsynchronousFileAccessor instance.
			~AsynchronousFileAccessor();

		// ---------------------------------------------------

			::Eldritch2::ErrorCode	BeginRead( void* const destinationBuffer, const size_t lengthToReadInBytes, const ::Eldritch2::uint64 fileOffsetInBytes ) override sealed;

		// ---------------------------------------------------

			::Eldritch2::ErrorCode	BeginWrite( const void* const sourceBuffer, const size_t lengthToWriteInBytes, const ::Eldritch2::uint64 fileOffsetInBytes ) override sealed;

		// ---------------------------------------------------

			bool	IsReadComplete() override sealed;

			bool	IsWriteComplete() override sealed;

		// ---------------------------------------------------

			AsynchronousFileReader::BlockingResult	BlockUntilReadComplete() override sealed;

			AsynchronousFileWriter::BlockingResult	BlockUntilWriteComplete() override sealed;

		// ---------------------------------------------------

			void	CancelRead() override sealed;

			void	CancelWrite() override sealed;

		// ---------------------------------------------------

			::Eldritch2::uint64 GetSizeInBytes() const override sealed;

			size_t				GetMediaSectorSizeInBytes() const override sealed;

		// ---------------------------------------------------

			void	AdvanceToEnd() override sealed;

		// ---------------------------------------------------

			void	SetFileSize( const ::Eldritch2::uint64 fileSizeInBytes ) override sealed;

		// - DATA MEMBERS ------------------------------------

		private:
			const ::HANDLE						_fileHandle;
			const size_t						_mediaSectorSizeInBytes;
#	if( ETIsPlatform64Bit() )
			union {
				size_t							_additionalReadBytes;
				size_t							_additionalWrittenBytes;
			};
			::std::atomic<::Eldritch2::uint32>	_cancellationFlag;
#	endif
			::OVERLAPPED						_overlapped;
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		//!	Constructs this @ref Win32ContentProvider instance.
		Win32ContentProvider( const Foundation::Win32SystemInterface& systemInterface );

		//!	Destroys this @ref Win32ContentProvider instance.
		~Win32ContentProvider() = default;

	// - FILE READ ACCESS --------------------------------

		using ContentProvider::CreateAsynchronousFileReader;
		Utility::Result<FileSystem::AsynchronousFileReader>		CreateAsynchronousFileReader( ::Eldritch2::Allocator& allocator, const ContentProvider::KnownContentLocation contentLocation, const ::Eldritch2::UTF8Char* const fileName ) override;

		using ContentProvider::CreateSynchronousFileReader;
		Utility::Result<FileSystem::SynchronousFileReader>		CreateSynchronousFileReader( ::Eldritch2::Allocator& allocator, const ContentProvider::KnownContentLocation contentLocation, const ::Eldritch2::UTF8Char* const fileName ) override;

		using ContentProvider::CreateReadableMemoryMappedFile;
		Utility::Result<FileSystem::ReadableMemoryMappedFile>	CreateReadableMemoryMappedFile( ::Eldritch2::Allocator& allocator, const ContentProvider::KnownContentLocation contentLocation, const ::Eldritch2::UTF8Char* const fileName ) override;

	// - FILE WRITE ACCESS -------------------------------

		using ContentProvider::CreateAsynchronousFileWriter;
		Utility::Result<FileSystem::AsynchronousFileWriter>	CreateAsynchronousFileWriter( ::Eldritch2::Allocator& allocator, const ContentProvider::KnownContentLocation contentLocation, const ::Eldritch2::UTF8Char* const fileName, const FileOverwriteBehavior overwriteBehavior ) override;

		using ContentProvider::CreateSynchronousFileWriter;
		Utility::Result<FileSystem::SynchronousFileWriter>	CreateSynchronousFileWriter( ::Eldritch2::Allocator& allocator, const ContentProvider::KnownContentLocation contentLocation, const ::Eldritch2::UTF8Char* const fileName, const FileOverwriteBehavior overwriteBehavior ) override;

	// - FILE APPEND ACCESS ------------------------------

		using ContentProvider::CreateSynchronousFileAppender;
		Utility::Result<FileSystem::SynchronousFileAppender>	CreateSynchronousFileAppender( ::Eldritch2::Allocator& allocator, const ContentProvider::KnownContentLocation contentLocation, const ::Eldritch2::UTF8Char* const fileName ) override;

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