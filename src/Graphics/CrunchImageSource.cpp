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
	ET_SET_MSVC_WARNING_STATE( disable : 6001 6201 6326 )
#	include <crunch/inc/crn_decomp.h>
ET_POP_COMPILER_WARNING_STATE()
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Graphics {
namespace {

	using namespace ::crnd;

	static ETInlineHint ETPureFunctionHint ImageSource::Dimensions AsDimensions( const crn_texture_info& source ) {
		return ImageSource::Dimensions{ source.m_width, source.m_height, static_cast<uint16>(source.m_faces), static_cast<uint16>(source.m_levels) };
	}

}	// anonymous namespace

	using namespace ::crnd;

	CrunchImageSource::CrunchImageSource() : _context( nullptr ) {}

// ---------------------------------------------------

	CrunchImageSource::~CrunchImageSource() {
		FreeResources();
	}

// ---------------------------------------------------

	ImageSource::Dimensions CrunchImageSource::GetDimensions() const {
		return _dimensions;
	}

// ---------------------------------------------------

	void CrunchImageSource::StreamTexels( const StreamRequest& request ) const {
		if (_dimensions.sliceCount <= request.arraySlice) {
			return;
		}

	//	Crunch wants all slices to have their own pointer.
		unsigned int	sliceCount( 1 );
		void** const	outputs( static_cast<void**>(_alloca( sliceCount * sizeof(void*) )) );

		for (unsigned int slice = 0; slice < sliceCount; ++slice) {
			outputs[slice] = static_cast<char*>(request.target) + ( slice * request.sliceStrideInBytes );
		}

		crnd_unpack_level_segmented(
			_context,
			nullptr,
			0,
			outputs,
			sliceCount * request.sliceStrideInBytes,
			request.scanlineStrideInBytes,
			(_dimensions.mips * request.arraySlice) + request.mip
		);
	}

// ---------------------------------------------------

	ErrorCode CrunchImageSource::BindResources( Range<const char*> header ) {
		const crnd_unpack_context	context( crnd_unpack_begin( header.Begin(), static_cast<uint32>(header.GetSize()) ) );
		crn_texture_info			textureInfo;

		if (!context || !crnd_get_texture_info( header.Begin(), static_cast<uint32>(header.GetSize()), &textureInfo )) {
			return Error::InvalidParameter;
		}

		_dimensions = AsDimensions( textureInfo );

		return Error::None;
	}

// ---------------------------------------------------

	void CrunchImageSource::FreeResources() {
		crnd_unpack_end( eastl::exchange( _context, nullptr ) );
	}

}	// namespace Graphics
}	// namespace Eldritch2