/*==================================================================*\
  SynchronousFileAccessor.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <FileSystem/SynchronousFileReader.hpp>
#include <FileSystem/SynchronousFileWriter.hpp>
//------------------------------------------------------------------//
#include <Windows.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace FileSystem {

	namespace FileSystem	= ::Eldritch2::FileSystem;

// ---------------------------------------------------

namespace Win32 {

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

}	// namespace Win32
}	// namespace FileSystem
}	// namespace Eldritch2