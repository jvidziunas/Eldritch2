/*==================================================================*\
  KeyframeAnimationView.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Assets/AssetView.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Animation {
namespace AssetViews {

	class KeyframeAnimationView : public Assets::AssetView {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref KeyframeAnimationView instance.
		KeyframeAnimationView( const Eldritch2::Utf8Char* const name );
	//!	Disable copying.
		KeyframeAnimationView( const KeyframeAnimationView& ) = delete;

		~KeyframeAnimationView() = default;

	// ---------------------------------------------------

	//!	Disable assignment.
		KeyframeAnimationView&	operator=( const KeyframeAnimationView& ) = delete;
	};

}	// namespace AssetViews
}	// namespace Animation
}	// namespace Eldritch2