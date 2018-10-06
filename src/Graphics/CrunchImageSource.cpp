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
/*	(6001) Bogus uninitialized memory warnings for Crunch.
	(6201) Array indexing in template class is unsafe for some (assumed known) template values.
	(6326) Switch statement involves non-type template parameter value; whining ensues. Non-idiomatic C++, but safe. */
ET_PUSH_MSVC_WARNING_STATE(disable : 6001 6201 6326)
#include <crunch/inc/crn_decomp.h>
ET_POP_MSVC_WARNING_STATE()
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics {

	using namespace ::crnd;

	namespace {

		ETInlineHint ETForceInlineHint ErrorCode DescribeImage(CrunchImageSource::ImageDescription& outDescription, Range<const char*> bytes) {
			crn_texture_info texture;
			if (!crnd_get_texture_info(bytes.Begin(), uint32(bytes.GetSize()), ETAddressOf(texture))) {
				return Error::InvalidParameter;
			}

			switch (texture.m_format) {
			case cCRNFmtDXT1:
				outDescription.format = GpuFormat::BC1_Unorm;
				break;
			case cCRNFmtDXT5_CCxY:
				outDescription.format = GpuFormat::BC3_YCoCg;
				break;
			case cCRNFmtDXT5:
			case cCRNFmtDXT5_xGxR:
			case cCRNFmtDXT5_xGBR:
			case cCRNFmtDXT5_AGBR:
			case cCRNFmtDXT5A:
				outDescription.format = GpuFormat::BC3_Unorm;
				break;
			case cCRNFmtDXN_XY:
			case cCRNFmtDXN_YX:
				outDescription.format = GpuFormat::BC5_Unorm;
				break;
			default:
				return Error::InvalidParameter;
			} // switch (texture.m_format)

			outDescription.mips        = texture.m_levels;
			outDescription.slices      = texture.m_faces;
			outDescription.texelWidth  = texture.m_width;
			outDescription.texelHeight = texture.m_height;
			outDescription.texelDepth  = 1u;

			return Error::None;
		}

	} // anonymous namespace

	CrunchImageSource::CrunchImageSource() :
		_context(nullptr) {}

	// ---------------------------------------------------

	CrunchImageSource::~CrunchImageSource() {
		FreeResources();
	}

	// ---------------------------------------------------

	ImageSource::ImageDescription CrunchImageSource::GetDescription() const {
		return _description;
	}

	// ---------------------------------------------------

	void CrunchImageSource::StreamTexels(const StreamRequest& request) const {
		ET_ASSERT(_description.slices * _description.mips > request.subimageId, "Subimage {} is out-of-bounds for image {}", request.subimageId, fmt::ptr(this));

		//	Crunch wants all slices to have their own pointer.
		const uint32 sliceCount(1u);
		void** const outputs(ETStackAlloc(void*, sliceCount));

		for (uint32 slice = 0; slice < sliceCount; ++slice) {
			outputs[slice] = static_cast<char*>(request.target) + (slice * request.sliceStrideInBytes);
		}

		crnd_unpack_level_segmented(
			_context,
			/*pSrc =*/nullptr,
			/*src_size_in_bytes =*/0u,
			outputs,
			sliceCount * request.sliceStrideInBytes,
			request.scanlineStrideInBytes,
			request.subimageId % _description.slices);
	}

	// ---------------------------------------------------

	ErrorCode CrunchImageSource::BindResources(Range<const char*> bytes) {
		crnd_unpack_context context(crnd_unpack_begin(bytes.Begin(), uint32(bytes.GetSize())));
		ET_ABORT_UNLESS(context ? Error::None : Error::InvalidParameter);
		ET_AT_SCOPE_EXIT(crnd_unpack_end(context));

		ET_ABORT_UNLESS(DescribeImage(_description, bytes))
		Swap(_context, context);

		return Error::None;
	}

	// ---------------------------------------------------

	void CrunchImageSource::FreeResources() {
		crnd_unpack_end(eastl::exchange(_context, nullptr));
	}

}} // namespace Eldritch2::Graphics
