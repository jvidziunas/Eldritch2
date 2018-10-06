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
		ImageDescription GetDescription() const override;

		// ---------------------------------------------------

	public:
		void StreamTexels(const StreamRequest& request) const override;

		// ---------------------------------------------------

	public:
		ErrorCode BindResources(Range<const char*> bytes);

		void FreeResources();

		// ---------------------------------------------------

		//!	Disable copy assignment.
		CrunchImageSource& operator=(const CrunchImageSource&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		crnd::crnd_unpack_context _context;
		ImageDescription          _description;
	};

}} // namespace Eldritch2::Graphics
