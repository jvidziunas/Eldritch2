/*==================================================================*\
  CrunchImageSource.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/ImageSource.hpp>
//------------------------------------------------------------------//

namespace crnd {
using crnd_unpack_context = void*;
}

namespace Eldritch2 { namespace Graphics {

	class CrunchImageSource : public ImageSource {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		CrunchImageSource(const CrunchImageSource&) = delete;
		//!	Constructs this @ref CrunchImageSource instance.
		CrunchImageSource();

		~CrunchImageSource();

		// ---------------------------------------------------

	public:
		SubimageDescription GetDescription(uint32 subimageId) const override;

		uint32 GetSliceCount() const override;

		uint32 GetMipCount() const override;

		void StreamTexels(const StreamRequest& request) const override;

		// ---------------------------------------------------

	public:
		ErrorCode BindResources(const char* begin, const char* end);

		void FreeResources();

		// ---------------------------------------------------

		//!	Disable copy assignment.
		CrunchImageSource& operator=(const CrunchImageSource&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		crnd::crnd_unpack_context _context;
		uint32                    _width;
		uint32                    _height;
		uint32                    _mips;
		uint32                    _slices;
	};

}} // namespace Eldritch2::Graphics
