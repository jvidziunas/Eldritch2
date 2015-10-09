/*==================================================================*\
  Win32FileWriteAccessStrategy.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/FileWriteAccessStrategy.hpp>
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
	namespace Utility		= ::Eldritch2::Utility;

// ---------------------------------------------------

	class Win32FileWriteAccessStrategy : public Utility::InstanceAllocatableMixin<FileSystem::FileWriteAccessStrategy> {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		// Constructs this Win32FileAccessStrategy instance.
		Win32FileWriteAccessStrategy( ::HANDLE fileHandle, size_t sectorSizeInBytes );

		// Destroys this Win32FileAccessStrategy instance.
		~Win32FileWriteAccessStrategy();

	// - ASYNCHRONOUS FILE WRITES-------------------------

		// Begins an asynchronous write of the data contained in the buffer identified by the sourceData parameter and spanning
		// the specified length. The buffer itself must persist at least until the next call to FinishWrite() returns.
		::Eldritch2::ErrorCode	BeginWrite( const void*	sourceBuffer, size_t dataLengthInBytes ) override sealed;
		// Moves the internal file write pointer to the offset (relative to the beginning of the file) specified in fileOffsetInBytes, 
		// then begins an asynchronous write of the data contained in the buffer identified by the sourceData parameter and spanning
		// the specified length. The buffer itself must persist at least until the next call to FinishWrite() returns.
		::Eldritch2::ErrorCode	BeginWrite( const void* sourceBuffer, size_t dataLengthInBytes, ::Eldritch2::uint64 fileOffsetInBytes ) override sealed;

		// Blocks until the write begun by a previous call to BeginWrite() completes, returning the number of bytes successfully copied.
		 size_t					FinishWrite() override sealed;

	// - DISK GEOMETRY QUERIES ---------------------------

		// Retrieves the physical media sector size for this file/pseudofile. All source buffers must be minimally aligned to this value.
		size_t	GetPhysicalMediaSectorSizeInBytes() const override sealed;

	// ---------------------------------------------------

		// Moves the internal file write pointer to immediately past the end of all written data.
		void	AdvanceToEnd() override sealed;

		void	SetFileSize( ::Eldritch2::uint64 fileSizeInBytes ) override sealed;

	// - DATA MEMBERS ------------------------------------

	private:
		const ::HANDLE			_fileHandle;
		const size_t			_sectorSizeInBytes;
		mutable ::OVERLAPPED	_overlapped;
	};

}	// namespace FileSystem
}	// namespace Eldritch2