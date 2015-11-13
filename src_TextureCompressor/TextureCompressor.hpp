/*==================================================================*\
  TextureCompressor.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Tools/Tool.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	template <typename Iterator>
	class	Range;
}

namespace Eldritch2 {
namespace Tools {

	template <class GlobalAllocator, class FileAccessorFactory>
	class TextureCompressorTool : GlobalAllocator, FileAccessorFactory, public Tools::ToolCRTPBase<TextureCompressorTool<GlobalAllocator, FileAccessorFactory> {
	// - TYPE PUBLISHING ---------------------------------

	public:
		using	AllocatorType			= GlobalAllocator;
		using	FileAccessorFactoryType = FileAccessorFactory;
		using	BaseToolType			= Tools::ToolCRTPBase<TextureCompressorTool<Allocator, FileAccessorFactory, MetadataVisitor>>;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		//!	Constructs this @ref TextureCompressorTool instance.
		TextureCompressorTool();

		//!	Destroys this @ref TextureCompressorTool instance.
		~TextureCompressorTool() = default;

	// ---------------------------------------------------

		ETInlineHint FileAccessorFactoryType&	GetFileAccessorFactory();

		ETInlineHint AllocatorType&				GetAllocator();

	// ---------------------------------------------------

		int	ProcessInputFiles( ::Eldritch2::Range<const UTF8Char**> inputFiles );
	};

}	// namespace Tools
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <TextureCompressor.inl>
//------------------------------------------------------------------//

