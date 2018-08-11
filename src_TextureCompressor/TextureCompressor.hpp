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

namespace Eldritch2 { namespace Tools {

	class TextureCompressor : public CrtpTool<TextureCompressor> {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		TextureCompressor(const TextureCompressor&) = delete;
		//!	Constructs this @ref TextureCompressor instance.
		TextureCompressor();

		~TextureCompressor() = default;

		// ---------------------------------------------------

	public:
		void RegisterOptions(OptionRegistrar& options);

		int ProcessImage(const Utf8Char* path);

		int Process();

		// ---------------------------------------------------

		//!	Disable copy assignment.
		TextureCompressor& operator=(const TextureCompressor&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		HashSet<String<>> _sourcePaths;
		uint32            _threadCount;
		uint32            _imageQuality;
		uint32            _mipLevels;
		uint32            _alphaToCoverageSamplesPerPixel;
		bool              _correctAlphaTestDistribution;
		bool              _isColorData;
	};

}} // namespace Eldritch2::Tools
