/*==================================================================*\
  TextureCompressor.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Tools {

	template <class GlobalAllocator, class FileAccessorFactory>
	TextureCompressor<GlobalAllocator, FileAccessorFactory>::TextureCompressor() : AllocatorType( UTF8L("Root Allocator") ), BaseToolType( GetAllocator() ) {}

// ---------------------------------------------------

	template <class GlobalAllocator, class FileAccessorFactory>
	ETInlineHint typename TextureCompressor<GlobalAllocator, FileAccessorFactory>::FileAccessorFactoryType& TextureCompressor<GlobalAllocator, FileAccessorFactory>::GetFileAccessorFactory() {}

// ---------------------------------------------------

	template <class GlobalAllocator, class FileAccessorFactory>
	ETInlineHint typename TextureCompressor<GlobalAllocator, FileAccessorFactory>::AllocatorType& TextureCompressor<GlobalAllocator, FileAccessorFactory>::GetAllocator() {}

// ---------------------------------------------------

	template <class GlobalAllocator, class FileAccessorFactory>
	int	TextureCompressor<GlobalAllocator, FileAccessorFactory>::ProcessInputFiles( Eldritch2::Range<const UTF8Char**> inputFiles ) {
		for( const UTF8Char* file : inputFiles ) {

		}

		return 0;
	}

}	// namespace Tools
}	// namespace Eldritch2

