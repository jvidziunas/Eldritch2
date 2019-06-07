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
#include <Common/Containers/SoaList.hpp>
#include <Common/Containers/Path.hpp>
#include <Common/FileReader.hpp>
#include <Tools/CrtpTool.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Tools {

	class TextureCompressor : public CrtpTool<TextureCompressor> {
	public:
		using ImageList = SoaList<Path, FileReader>;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		TextureCompressor(const TextureCompressor&) = delete;
		//!	Constructs this @ref TextureCompressor instance.
		TextureCompressor() ETNoexceptHint;

		~TextureCompressor() = default;

		// ---------------------------------------------------

	public:
		Result ProcessImage(Logging::Log& log, ImageList::Reference sourceImage);

		Result Process(Logging::Log& log);

		void RegisterOptions(OptionRegistrar& options);

		// ---------------------------------------------------

		//!	Disable copy assignment.
		TextureCompressor& operator=(const TextureCompressor&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		ImageList _sourceImages;
		uint32    _threadCount;
		uint32    _imageQuality;
		uint32    _mipLevels;
		uint32    _alphaToCoverageSamplesPerPixel;
		bool      _correctAlphaTestDistribution;
		bool      _isColorData;
	};

}} // namespace Eldritch2::Tools
