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
#include <Tools/ToolCRTPBase.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	template <typename Iterator>
	class	Range;
}

namespace Eldritch2 {
namespace Tools {

	template <class GlobalAllocator, class FileAccessorFactory>
	class TextureCompressor : GlobalAllocator, FileAccessorFactory, public Tools::ToolCRTPBase<TextureCompressor<GlobalAllocator, FileAccessorFactory>> {
	// - TYPE PUBLISHING ---------------------------------

	public:
		using	AllocatorType			= GlobalAllocator;
		using	FileAccessorFactoryType = FileAccessorFactory;
		using	BaseToolType			= Tools::ToolCRTPBase<TextureCompressor<Allocator, FileAccessorFactory>>;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		//!	Constructs this @ref TextureCompressor instance.
		TextureCompressor();

		~TextureCompressor() = default;

	// ---------------------------------------------------

		ETInlineHint AllocatorType&	GetAllocator();

	// ---------------------------------------------------

		int	ProcessInputFiles( Eldritch2::Range<const UTF8Char**> inputFiles );
	};

}	// namespace Tools
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <TextureCompressor.inl>
//------------------------------------------------------------------//

