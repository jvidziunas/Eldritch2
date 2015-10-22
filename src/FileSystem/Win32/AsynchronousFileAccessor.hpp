/*==================================================================*\
  AsynchronousFileAccessor.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <FileSystem/AsynchronousFileReader.hpp>
#include <FileSystem/AsynchronousFileWriter.hpp>
//------------------------------------------------------------------//
#include <Windows.h>
//------------------------------------------------------------------//
#include <atomic>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace FileSystem {

	namespace FileSystem	= ::Eldritch2::FileSystem;

// ---------------------------------------------------

namespace Win32 {

	class AsynchronousFileAccessor : public FileSystem::AsynchronousFileReader, public FileSystem::AsynchronousFileWriter {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref AsynchronousFileAccessor instance.
		AsynchronousFileAccessor( const ::HANDLE fileHandle, const size_t mediaSectorSizeInBytes );

		//! Destroys this @ref AsynchronousFileAccessor instance.
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

}	// namespace Win32
}	// namespace FileSystem
}	// namespace Eldritch2