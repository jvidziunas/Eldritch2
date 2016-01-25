/*==================================================================*\
  ContentProvider.hpp
  ------------------------------------------------------------------
  Purpose:
  Windows implementation for the content provider interface. This
  object translates high-level calls to read/serialize content from
  the hard disk into low-level API commands for paging file contents
  into the running process' address space.

  ------------------------------------------------------------------
  �2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <FileSystem/Win32/ReadableMemoryMappedFile2.hpp>
#include <FileSystem/Win32/AsynchronousFileReader.hpp>
#include <FileSystem/Win32/AsynchronousFileWriter.hpp>
#include <FileSystem/Win32/SynchronousFileReader.hpp>
#include <FileSystem/Win32/SynchronousFileWriter.hpp>
#include <FileSystem/ContentProvider.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace System {
		namespace Win32 {
			class	SystemInterface;
		}
	}

	namespace Scheduler {
		class	ThreadScheduler;
	}
}

namespace Eldritch2 {
namespace FileSystem {
namespace Win32 {

	class ContentProvider : public FileSystem::ContentProvider {
	// - TYPE PUBLISHING ---------------------------------

	public:
		using ReadableMemoryMappedFile2 = FileSystem::Win32::ReadableMemoryMappedFile2;
		using ReadableMemoryMappedFile	= FileSystem::Win32::ReadableMemoryMappedFile;
		using AsynchronousFileReader	= FileSystem::Win32::AsynchronousFileReader;
		using AsynchronousFileWriter	= FileSystem::Win32::AsynchronousFileWriter;
		using SynchronousFileReader		= FileSystem::Win32::SynchronousFileReader;
		using SynchronousFileWriter		= FileSystem::Win32::SynchronousFileWriter;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		//!	Constructs this @ref ContentProvider instance.
		ContentProvider( const System::Win32::SystemInterface& systemInterface );

		//!	Destroys this @ref ContentProvider instance.
		~ContentProvider() = default;

	// - FILE READ ACCESS --------------------------------

		using FileSystem::ContentProvider::CreateAsynchronousFileReader;
		::Eldritch2::Result<FileSystem::AsynchronousFileReader>		CreateAsynchronousFileReader( ::Eldritch2::Allocator& allocator, const KnownContentLocation contentLocation, const ::Eldritch2::UTF8Char* const fileName ) override;

		using FileSystem::ContentProvider::CreateSynchronousFileReader;
		::Eldritch2::Result<FileSystem::SynchronousFileReader>		CreateSynchronousFileReader( ::Eldritch2::Allocator& allocator, const KnownContentLocation contentLocation, const ::Eldritch2::UTF8Char* const fileName ) override;

		using FileSystem::ContentProvider::CreateReadableMemoryMappedFile;
		::Eldritch2::Result<FileSystem::ReadableMemoryMappedFile>	CreateReadableMemoryMappedFile( ::Eldritch2::Allocator& allocator, const KnownContentLocation contentLocation, const ::Eldritch2::UTF8Char* const fileName ) override;

	// - FILE WRITE ACCESS -------------------------------

		using FileSystem::ContentProvider::CreateAsynchronousFileWriter;
		::Eldritch2::Result<FileSystem::AsynchronousFileWriter>	CreateAsynchronousFileWriter( ::Eldritch2::Allocator& allocator, const KnownContentLocation contentLocation, const ::Eldritch2::UTF8Char* const fileName, const FileOverwriteBehavior overwriteBehavior ) override;

		using FileSystem::ContentProvider::CreateSynchronousFileWriter;
		::Eldritch2::Result<FileSystem::SynchronousFileWriter>	CreateSynchronousFileWriter( ::Eldritch2::Allocator& allocator, const KnownContentLocation contentLocation, const ::Eldritch2::UTF8Char* const fileName, const FileOverwriteBehavior overwriteBehavior ) override;

	// - FILE APPEND ACCESS ------------------------------

		using FileSystem::ContentProvider::CreateSynchronousFileAppender;
		::Eldritch2::Result<FileSystem::SynchronousFileAppender>	CreateSynchronousFileAppender( ::Eldritch2::Allocator& allocator, const KnownContentLocation contentLocation, const ::Eldritch2::UTF8Char* const fileName ) override;

	// - FREE (UNPACKAGED) FILE MANIPULATION -------------

		void	CopyFreeFile( const KnownContentLocation contentLocation, const ::Eldritch2::UTF8Char* const destinationFileName, const ::Eldritch2::UTF8Char* const sourceFileName, const FileOverwriteBehavior overwriteBehavior ) override;

		void	DeleteFreeFile( const KnownContentLocation contentLocation, const ::Eldritch2::UTF8Char* const fileName ) override;

	// ---------------------------------------------------

	private:
		using GetSectorSizeFromHandleFunction	= size_t (*)( ::HANDLE );
		using KnownContentPath					= wchar_t[MAX_PATH];

	// ---------------------------------------------------

		ETInlineHint const wchar_t*	PathFromKnownContentLocation( const KnownContentLocation location ) const;

	// - DATA MEMBERS ------------------------------------

		const GetSectorSizeFromHandleFunction	_getSectorSizeFromHandleFunction;
		KnownContentPath						_knownContentPaths[static_cast<size_t>(KnownContentLocation::COUNT)];
	};

}	// namespace Win32
}	// namespace FileSystem
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <FileSystem/Win32/ContentProvider.inl>
//------------------------------------------------------------------//