/*==================================================================*\
  CrunchImageSource.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/CrunchImageSource.hpp>
//------------------------------------------------------------------//
ET_PUSH_COMPILER_WARNING_STATE()
/*	(6001) Bogus uninitialized memory warnings for Crunch.
	(6201) Array indexing in template class is unsafe for some (assumed known) template values.
	(6326) Switch statement involves non-type template parameter value; whining ensues. Non-idiomatic C++, but safe. */
ET_SET_MSVC_WARNING_STATE(disable : 6001 6201 6326)
#include <crunch/inc/crn_decomp.h>
ET_POP_COMPILER_WARNING_STATE()
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics {

	using namespace ::crnd;

	CrunchImageSource::CrunchImageSource() :
		_context(nullptr),
		_width(0u),
		_height(0u),
		_mips(0u),
		_slices(0u) {}

	// ---------------------------------------------------

	CrunchImageSource::~CrunchImageSource() {
		FreeResources();
	}

	// ---------------------------------------------------

	ImageSource::SubimageDescription CrunchImageSource::GetDescription(uint32 subimageId) const {
		if (_slices * _mips < subimageId) {
			//	Request is out of bounds.
			return ImageSource::SubimageDescription { 0u, 0u, 0u };
		}

		const uint32 mip(subimageId % _slices);
		return ImageSource::SubimageDescription { _width >> mip, _height >> mip, 1u };
	}

	// ---------------------------------------------------

	uint32 CrunchImageSource::GetSliceCount() const {
		return _slices;
	}

	// ---------------------------------------------------

	uint32 CrunchImageSource::GetMipCount() const {
		return _mips;
	}

	// ---------------------------------------------------

	void CrunchImageSource::StreamTexels(const StreamRequest& request) const {
		if ((_slices * _mips) < request.subimageId) {
			//	Request is out of bounds.
			return;
		}

		//	Crunch wants all slices to have their own pointer.
		const uint32 sliceCount(1u);
		void** const outputs(ETStackAlloc(void*, sliceCount));

		for (uint32 slice = 0; slice < sliceCount; ++slice) {
			outputs[slice] = static_cast<char*>(request.target) + (slice * request.sliceStrideInBytes);
		}

		crnd_unpack_level_segmented(
			_context,
			nullptr,
			0,
			outputs,
			sliceCount * request.sliceStrideInBytes,
			request.scanlineStrideInBytes,
			request.subimageId % _slices);
	}

	// ---------------------------------------------------

	ErrorCode CrunchImageSource::BindResources(const char* begin, const char* end) {
		const crnd_unpack_context context(crnd_unpack_begin(begin, uint32(end - begin)));
		crn_texture_info          textureDescription;

		if (!context || !crnd_get_texture_info(begin, uint32(end - begin), &textureDescription)) {
			return Error::InvalidParameter;
		}

		_width  = textureDescription.m_width;
		_height = textureDescription.m_height;
		_mips   = textureDescription.m_levels;
		_slices = textureDescription.m_faces;

		return Error::None;
	}

	// ---------------------------------------------------

	void CrunchImageSource::FreeResources() {
		crnd_unpack_end(eastl::exchange(_context, nullptr));
	}

}} // namespace Eldritch2::Graphics
