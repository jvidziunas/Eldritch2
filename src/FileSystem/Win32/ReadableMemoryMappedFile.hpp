/*==================================================================*\
  ReadableMemoryMappedFile.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <FileSystem/ReadableMemoryMappedFile.hpp>
#include <Utility/Containers/Range.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace FileSystem {
namespace Win32 {

	class ReadableMemoryMappedFile : public FileSystem::ReadableMemoryMappedFile {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref ReadableMemoryMappedFile instance.
		ReadableMemoryMappedFile( const ::Eldritch2::Range<const char*> mappedRegion );

		//!	Destroys this @ref ReadableMemoryMappedFile instance.
		~ReadableMemoryMappedFile();

	// ---------------------------------------------------

		size_t	GetAccessibleRegionSizeInBytes() const override;

	// ---------------------------------------------------

		void	PrefetchRange( const ::Eldritch2::Range<const char*> memoryRange ) const override;

	// ---------------------------------------------------

		const void*	GetAddressForFileByteOffset( const size_t rawFileOffsetInBytes ) const override;

	// - DATA MEMBERS ------------------------------------

	private:
		const ::Eldritch2::Range<const char*>	_region;
	};

}	// namespace Win32
}	// namespace FileSystem
}	// namespace Eldritch2