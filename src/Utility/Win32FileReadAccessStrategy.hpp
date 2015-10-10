/*==================================================================*\
  Win32FileReadAccessStrategy.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/FileReadAccessStrategy.hpp>
#include <Utility/Memory/InstanceAllocatable.hpp>
#ifndef WIN32_LEAN_AND_MEAN
#	define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#	define NOMINMAX
#endif
#include <Windows.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace FileSystem {

	namespace FileSystem	= ::Eldritch2::FileSystem;

// ---------------------------------------------------

	class Win32FileReadAccessStrategy : public Utility::InstanceAllocatableMixin<FileSystem::FileReadAccessStrategy> {
	public:
		// Constructs this Win32FileReadAccessStrategy instance.
		Win32FileReadAccessStrategy( ::HANDLE fileHandle, size_t sectorSizeInBytes );

		// Destroys this Win32FileReadAccessStrategy instance.
		~Win32FileReadAccessStrategy();

	// ---------------------------------------------------

		// Begins an (optionally) asynchronous read of a block of data starting at the current file pointer and continuing for (at most) the specified
		// number of bytes. The buffer itself must persist at least until the next call to FinishRead() returns.
		// Not thread-safe, though independent Win32FileReadAccessStrategy instances can safely access the same file.
		::Eldritch2::ErrorCode	BeginRead( void* destinationBuffer, size_t lengthToReadInBytes ) override sealed;
		// Begins an (optionally) asynchronous read of a block of data starting at the file offset passed in through the third parameter and continuing for
		// (at most) the specified number of bytes. The buffer itself must persist at least until the next call to FinishRead() returns.
		// Not thread-safe, though independent Win32FileReadAccessStrategy instances can safely access the same file.
		::Eldritch2::ErrorCode	BeginRead( void* destinationBuffer, size_t lengthToReadInBytes, ::Eldritch2::uint64 fileOffsetInBytes ) override sealed;

		// Tests to see whether the previous asynchronous read started by a call to BeginRead() has completed.
		// Thread-safe.
		bool					IsReadComplete() const override sealed;

		// Blocks until the read begun by a previous call to BeginRead() completes, returning the number of bytes successfully copied.
		// Not thread-safe, though independent Win32FileReadAccessStrategy instances can safely access the same file.
		BlockingResult			FinishRead() override sealed;

	// ---------------------------------------------------

		// Retrieves the total on-disk size of the file/pseudofile being accessed.
		// Thread-safe.
		::Eldritch2::uint64	GetSizeInBytes() const override sealed;

		// Retrieves the physical media sector size for this file/pseudofile. All destination buffers must be minimally aligned to
		// this value, and all reads must be an even multiple of this.
		// Thread-safe.
		size_t				GetPhysicalMediaSectorSizeInBytes() const override sealed;

	// ---------------------------------------------------

	private:
		const ::HANDLE			_fileHandle;
		const size_t			_sectorSizeInBytes;
		mutable ::OVERLAPPED	_overlapped;
	};

}	// namespace FileSystem
}	// namespace Eldritch2