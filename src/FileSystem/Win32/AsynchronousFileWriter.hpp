/*==================================================================*\
  AsynchronousFileWriter.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <FileSystem/AsynchronousFileWriter.hpp>
//------------------------------------------------------------------//
#include <Windows.h>
//------------------------------------------------------------------//
#include <atomic>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace FileSystem {
namespace Win32 {

	class AsynchronousFileWriter : public FileSystem::AsynchronousFileWriter {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref AsynchronousFileWriter instance.
		AsynchronousFileWriter( const ::HANDLE fileHandle, const size_t mediaSectorSizeInBytes );

		//! Destroys this @ref AsynchronousFileWriter instance.
		~AsynchronousFileWriter();

	// ---------------------------------------------------

		::Eldritch2::ErrorCode	BeginWrite( const void* const sourceBuffer, const size_t lengthToWriteInBytes, const ::Eldritch2::uint64 fileOffsetInBytes ) override sealed;

	// ---------------------------------------------------

		bool	IsWriteComplete() override sealed;

	// ---------------------------------------------------

		AsynchronousFileWriter::BlockingResult	BlockUntilWriteComplete() override sealed;

	// ---------------------------------------------------

		void	CancelWrite() override sealed;

	// ---------------------------------------------------

		size_t	GetMediaSectorSizeInBytes() const override sealed;

	// ---------------------------------------------------

		void	AdvanceToEnd() override sealed;

	// ---------------------------------------------------

		void	SetFileSize( const ::Eldritch2::uint64 fileSizeInBytes ) override sealed;

	// - DATA MEMBERS ------------------------------------

	private:
		const ::HANDLE						_fileHandle;
		const size_t						_mediaSectorSizeInBytes;
		size_t								_additionalWrittenBytes;
		::std::atomic<::Eldritch2::uint32>	_cancellationFlag;
		::OVERLAPPED						_overlapped;
	};

}	// namespace Win32
}	// namespace FileSystem
}	// namespace Eldritch2

