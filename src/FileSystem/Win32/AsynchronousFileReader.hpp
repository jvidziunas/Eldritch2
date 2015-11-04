/*==================================================================*\
  AsynchronousFileReader.hpp
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
//------------------------------------------------------------------//
#include <Windows.h>
//------------------------------------------------------------------//
#include <atomic>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace FileSystem {
namespace Win32 {

	class AsynchronousFileReader : public FileSystem::AsynchronousFileReader {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref AsynchronousFileReader instance.
		AsynchronousFileReader( const ::HANDLE fileHandle, const size_t mediaSectorSizeInBytes );

		//! Destroys this @ref AsynchronousFileReader instance.
		~AsynchronousFileReader();

	// ---------------------------------------------------

		::Eldritch2::ErrorCode	BeginRead( void* const destinationBuffer, const size_t lengthToReadInBytes, const ::Eldritch2::uint64 fileOffsetInBytes ) override sealed;

	// ---------------------------------------------------

		bool	IsReadComplete() override sealed;

	// ---------------------------------------------------

		AsynchronousFileReader::BlockingResult	BlockUntilReadComplete() override sealed;

	// ---------------------------------------------------

		void	CancelRead() override sealed;

	// ---------------------------------------------------

		::Eldritch2::uint64 GetSizeInBytes() const override sealed;

		size_t				GetMediaSectorSizeInBytes() const override sealed;

	// - DATA MEMBERS ------------------------------------

	private:
		const ::HANDLE						_fileHandle;
		const size_t						_mediaSectorSizeInBytes;
		size_t								_additionalReadBytes;
		::std::atomic<::Eldritch2::uint32>	_cancellationFlag;
		::OVERLAPPED						_overlapped;
	};

}	// namespace Win32
}	// namespace FileSystem
}	// namespace Eldritch2