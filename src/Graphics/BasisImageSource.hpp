/*==================================================================*\
  BasisImageSource.hpp
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
#include <basis/transcoder/basisu_transcoder.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics {

	class BasisImageSource : public ImageSource {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		BasisImageSource(const BasisImageSource&) = delete;
		//!	Constructs this @ref CrunchImageSource instance.
		BasisImageSource() ETNoexceptHint;

		~BasisImageSource();

		// ---------------------------------------------------

	public:
		ImageDescriptor DescribeSelf() const ETNoexceptHint override;

		// ---------------------------------------------------

	public:
		void StreamTexels(const StreamRequest& request) const ETNoexceptHint override;

		// ---------------------------------------------------

	public:
		Result BindResources(Span<const byte*> bytes);

		void FreeResources() ETNoexceptHint;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		BasisImageSource& operator=(const BasisImageSource&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		basist::basisu_transcoder _transcoder;
	};

}} // namespace Eldritch2::Graphics
