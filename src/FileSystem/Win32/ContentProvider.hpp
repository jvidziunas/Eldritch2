/*==================================================================*\
  ContentProvider.hpp
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
#include <FileSystem/Win32/AsynchronousFileAccessor.hpp>
#include <FileSystem/Win32/SynchronousFileAccessor.hpp>
#include <FileSystem/ContentProvider.hpp>
//------------------------------------------------------------------//
#include <Windows.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace System {
		namespace Win32 {
			class	SystemInterface;
		}
	}

	namespace Scheduler {
		class	TaskScheduler;
	}
}

namespace Eldritch2 {
namespace FileSystem {

	namespace FileSystem	= ::Eldritch2::FileSystem;
	namespace Scheduler		= ::Eldritch2::Scheduler;
	namespace Utility		= ::Eldritch2::Utility;
	namespace System		= ::Eldritch2::System;

// ---------------------------------------------------

namespace Win32 {

	class ContentProvider : public FileSystem::ContentProvider {
	// - TYPE PUBLISHING ---------------------------------

	public:
		using AsynchronousFileReader	= FileSystem::Win32::AsynchronousFileAccessor;
		using AsynchronousFileWriter	= FileSystem::Win32::AsynchronousFileAccessor;
		using SynchronousFileReader		= FileSystem::Win32::SynchronousFileAccessor;
		using SynchronousFileWriter		= FileSystem::Win32::SynchronousFileAccessor;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		//!	Constructs this @ref ContentProvider instance.
		ContentProvider( const System::Win32::SystemInterface& systemInterface );

		//!	Destroys this @ref ContentProvider instance.
		~ContentProvider() = default;

	// - FILE READ ACCESS --------------------------------

		using FileSystem::ContentProvider::CreateAsynchronousFileReader;
		Utility::Result<FileSystem::AsynchronousFileReader>		CreateAsynchronousFileReader( ::Eldritch2::Allocator& allocator, const KnownContentLocation contentLocation, const ::Eldritch2::UTF8Char* const fileName ) override;

		using FileSystem::ContentProvider::CreateSynchronousFileReader;
		Utility::Result<FileSystem::SynchronousFileReader>		CreateSynchronousFileReader( ::Eldritch2::Allocator& allocator, const KnownContentLocation contentLocation, const ::Eldritch2::UTF8Char* const fileName ) override;

		using FileSystem::ContentProvider::CreateReadableMemoryMappedFile;
		Utility::Result<FileSystem::ReadableMemoryMappedFile>	CreateReadableMemoryMappedFile( ::Eldritch2::Allocator& allocator, const KnownContentLocation contentLocation, const ::Eldritch2::UTF8Char* const fileName ) override;

	// - FILE WRITE ACCESS -------------------------------

		using FileSystem::ContentProvider::CreateAsynchronousFileWriter;
		Utility::Result<FileSystem::AsynchronousFileWriter>	CreateAsynchronousFileWriter( ::Eldritch2::Allocator& allocator, const KnownContentLocation contentLocation, const ::Eldritch2::UTF8Char* const fileName, const FileOverwriteBehavior overwriteBehavior ) override;

		using FileSystem::ContentProvider::CreateSynchronousFileWriter;
		Utility::Result<FileSystem::SynchronousFileWriter>	CreateSynchronousFileWriter( ::Eldritch2::Allocator& allocator, const KnownContentLocation contentLocation, const ::Eldritch2::UTF8Char* const fileName, const FileOverwriteBehavior overwriteBehavior ) override;

	// - FILE APPEND ACCESS ------------------------------

		using FileSystem::ContentProvider::CreateSynchronousFileAppender;
		Utility::Result<FileSystem::SynchronousFileAppender>	CreateSynchronousFileAppender( ::Eldritch2::Allocator& allocator, const KnownContentLocation contentLocation, const ::Eldritch2::UTF8Char* const fileName ) override;

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
		KnownContentPath						_packageContentPath;
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