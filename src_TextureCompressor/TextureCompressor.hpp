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
#include <Common/Containers/Path.hpp>
#include <Tools/CrtpTool.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
class ErrorCode;
}

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
		ErrorCode ProcessImage(const Path& path);

		ErrorCode Process();

		void RegisterOptions(OptionRegistrar& options);

		// ---------------------------------------------------

		//!	Disable copy assignment.
		TextureCompressor& operator=(const TextureCompressor&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		HashSet<Path> _sourcePaths;
		uint32        _threadCount;
		uint32        _imageQuality;
		uint32        _mipLevels;
		uint32        _alphaToCoverageSamplesPerPixel;
		bool          _correctAlphaTestDistribution;
		bool          _isColorData;
	};

}} // namespace Eldritch2::Tools
