/*==================================================================*\
  SynchronousFileReader.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Mpl/CharTypes.hpp>
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

	class SynchronousFileReader {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
#	if ET_PLATFORM_WINDOWS
	//!	Constructs this @ref SynchronousFileReader instance.
		explicit SynchronousFileReader( HANDLE fileHandle );
#	endif
	//!	Disable copying.
		SynchronousFileReader( const SynchronousFileReader& ) = delete;
	//!	Constructs this @ref SynchronousFileReader instance.
		SynchronousFileReader( SynchronousFileReader&& );

		~SynchronousFileReader();

	// ---------------------------------------------------

	public:
		static Eldritch2::Result<SynchronousFileReader>	Open( const Eldritch2::SystemChar* const path );

	// ---------------------------------------------------

	public:
	//! Performs a blocking read operation to the output device this @ref SynchronousFileReader uses as its backing.
		Eldritch2::ErrorCode	Read( void* const destinationBuffer, size_t lengthToReadInBytes, Eldritch2::uint64 fileOffsetInBytes );
	//! Performs a blocking read operation to the output device this @ref SynchronousFileReader uses as its backing.
		Eldritch2::ErrorCode	Read( void* const destinationBuffer, size_t lengthToReadInBytes );

	// ---------------------------------------------------

	public:
	//!	Retrieves the total on-disk size of the file/pseudofile being accessed.
	/*!	@returns On disk size of the file, in bytes.
		@remarks Thread-safe. */
		Eldritch2::uint64	GetSizeInBytes() const;

	// ---------------------------------------------------

	//!	Disable assignment.
		SynchronousFileReader&	operator=( const SynchronousFileReader& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
#	if ET_PLATFORM_WINDOWS
		HANDLE	_fileHandle;
#	endif
	};

}	// namespace Platform
}	// namespace Eldritch2