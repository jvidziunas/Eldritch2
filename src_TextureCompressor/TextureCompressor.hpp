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
#include <Common/Containers/HashSet.hpp>
#include <Common/Containers/String.hpp>
#include <Tools/CrtpTool.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Tools {

	class TextureCompressor : public CrtpTool<TextureCompressor> {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref TextureCompressor instance.
		TextureCompressor();

		~TextureCompressor() = default;

	// ---------------------------------------------------

	public:
		Allocator&	GetAllocator() const;

	// ---------------------------------------------------

	public:
		void	RegisterOptions( OptionRegistrar& options );

		int		ProcessFile( const Utf8Char* path );

		int		Process();

	// - DATA MEMBERS ------------------------------------

	private:
		uint32				_quality;
		bool				_isPerceptualData;
		bool				_useMipmaps;
		
		HashSet<String<>>	_sourcePaths;
	};

}	// namespace Tools
}	// namespace Eldritch2
