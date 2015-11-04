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
#include <FileSystem/SynchronousFileReader.hpp>
//------------------------------------------------------------------//
#include <Windows.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace FileSystem {
namespace Win32 {

	class SynchronousFileReader : public FileSystem::SynchronousFileReader {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref SynchronousFileReader instance.
		SynchronousFileReader( const ::HANDLE fileHandle, const size_t mediaSectorSizeInBytes );

		//! Destroys this @ref SynchronousFileReader instance.
		~SynchronousFileReader();

	// ---------------------------------------------------

		BlockingResult	Read( void* const destinationBuffer, const size_t lengthToReadInBytes ) override sealed;
		BlockingResult	Read( void* const destinationBuffer, const size_t lengthToReadInBytes, const ::Eldritch2::uint64 fileOffsetInBytes ) override sealed;

	// ---------------------------------------------------

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