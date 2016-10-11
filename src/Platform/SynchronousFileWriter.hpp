/*==================================================================*\
  SynchronousFileWriter.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/CharTypes.hpp>
#include <Utility/Mpl/IntTypes.hpp>
#include <Utility/Mpl/Platform.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	class	ErrorCode;
	template <typename>
	class	Result;
}

#if ET_PLATFORM_WINDOWS
using HANDLE = void*;
#endif

namespace Eldritch2 {
namespace Platform {

	class SynchronousFileWriter {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
#	if ET_PLATFORM_WINDOWS
		SynchronousFileWriter( HANDLE fileHandle );
#	else
		static_assert( false, "SynchronousFileWriter needs implementation for your platform!" );
#	endif
	//! Disable copying.
		SynchronousFileWriter( const SynchronousFileWriter& ) = delete;
	//!	Constructs this @ref SynchronousFileWriter instance.
		SynchronousFileWriter( SynchronousFileWriter&& );
	
		~SynchronousFileWriter();

	// ---------------------------------------------------

	public:
		static Eldritch2::Result<SynchronousFileWriter>	CreateOrTruncate( const Eldritch2::SystemChar* const path );

		static Eldritch2::Result<SynchronousFileWriter>	CreateOrOpen( const Eldritch2::SystemChar* const path );

		static Eldritch2::Result<SynchronousFileWriter>	Open( const Eldritch2::SystemChar* const path );

	// ---------------------------------------------------

	public:
	//!	Performs a blocking write operation to the output device this SynchronousFileWriter uses as its backing.
	/*!	@param[in] sourceBuffer Pointer to a region of memory containing the bit patterns to be written.
		@param[in] lengthToWriteInBytes Number of bytes to write.
		@param[in] fileOffsetInBytes Offset into the file to begin writing data. This must be a multiple of the backing media sector size.
		@returns an @ref ErrorCode describing the result of the operation. */
		Eldritch2::ErrorCode	Write( const void* const sourceBuffer, size_t lengthToWriteInBytes, Eldritch2::uint64 fileOffsetInBytes );
	//!	Performs a blocking write operation to the output device this SynchronousFileWriter uses as its backing.
	/*!	@param[in] sourceBuffer Pointer to a region of memory containing the bit patterns to be written.
		@param[in] lengthToWriteInBytes Number of bytes to write. This must be a multiple of the backing media sector size.
		@returns an @ref ErrorCode describing the result of the operation.
		@see @ref Write( const void*, const size_t, const Eldritch2::uint64 ) */
		Eldritch2::ErrorCode	Write( const void* const sourceBuffer, size_t lengthToWriteInBytes );

	// ---------------------------------------------------

	public:
	//! Returns the current implicit write offset, specified relative to the beginning of the file.
	/*!	@returns The offset, in bytes. */
		Eldritch2::uint64	GetFileCursorInBytes() const;

	//!	Moves the internal file write pointer to immediately past the end of all written data.
		void				AdvanceToEnd();

	//!	Extends or truncates the backing file so it exactly occupies the specified amount of space on the backing storage.
	/*!	@param[in] fileSizeInBytes The amount of valid data to mark the backing file as possessing. Note that this does _not_ need to be a multiple of the media sector size. */
		void				SetFileSize( Eldritch2::uint64 fileSizeInBytes );

	// ---------------------------------------------------

	//!	Disable assignment.
		SynchronousFileWriter&	operator=( const SynchronousFileWriter& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		HANDLE	_fileHandle;
	};

}	// namespace Platform
}	// namespace Eldritch2