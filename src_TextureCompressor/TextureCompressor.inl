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
	TextureCompressorTool<GlobalAllocator, FileAccessorFactory>::TextureCompressorTool() : AllocatorType( UTF8L("Root Allocator") ), BaseToolType( GetAllocator() ) {}

// ---------------------------------------------------

	template <class GlobalAllocator, class FileAccessorFactory>
	ETInlineHint typename TextureCompressorTool<GlobalAllocator, FileAccessorFactory>::FileAccessorFactoryType& TextureCompressorTool<GlobalAllocator, FileAccessorFactory>::GetFileAccessorFactory() {}

// ---------------------------------------------------

	template <class GlobalAllocator, class FileAccessorFactory>
	ETInlineHint typename TextureCompressorTool<GlobalAllocator, FileAccessorFactory>::AllocatorType& TextureCompressorTool<GlobalAllocator, FileAccessorFactory>::GetAllocator() {}

// ---------------------------------------------------

	template <class GlobalAllocator, class FileAccessorFactory>
	int	TextureCompressorTool<GlobalAllocator, FileAccessorFactory>::ProcessInputFiles( ::Eldritch2::Range<const UTF8Char**> inputFiles ) {
		for( const UTF8Char* file : inputFiles ) {

		}

		return 0;
	}

}	// namespace Tools
}	// namespace Eldritch2

