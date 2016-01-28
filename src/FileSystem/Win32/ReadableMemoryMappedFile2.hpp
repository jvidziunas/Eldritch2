/*==================================================================*\
  ReadableMemoryMappedFile2.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <FileSystem/Win32/ReadableMemoryMappedFile.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace FileSystem {
namespace Win32 {

	class ReadableMemoryMappedFile2 : public ReadableMemoryMappedFile {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref ReadableMemoryMappedFile2 instance.
		ReadableMemoryMappedFile2( const ::Eldritch2::Range<const char*> mappedRegion );

		//!	Destroys this @ref ReadableMemoryMappedFile2 instance.
		~ReadableMemoryMappedFile2() = default;

	// ---------------------------------------------------

		void	PrefetchRange( const ::Eldritch2::Range<const char*> memoryRange ) const override;
	};

}	// namespace Win32
}	// namespace FileSystem
}	// namespace Eldritch2