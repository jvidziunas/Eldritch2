/*==================================================================*\
  Window.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/Noncopyable.hpp>
#include <Utility/MPL/CharTypes.hpp>
#include <Utility/MPL/IntTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Foundation {

	namespace Foundation	= ::Eldritch2::Foundation;
	namespace Utility		= ::Eldritch2::Utility;

// ---------------------------------------------------

	class ETPureAbstractHint Window : public Utility::Noncopyable {
	// - TYPE PUBLISHING ---------------------------------

	public:
		struct PresentationParameters {
			enum class Mode {
				WINDOWED,
				WINDOWED_FULLSCREEN,
				FULLSCREEN
			};

			PresentationParameters::Mode	presentationMode;
			::Eldritch2::uint32				widthInPixels;
			::Eldritch2::uint32				heightInPixels;
		};

	// ---------------------------------------------------

		virtual PresentationParameters	GetPresentationParameters() const abstract;

	// ---------------------------------------------------

		virtual void	SetCaption( const ::Eldritch2::UTF8Char* const captionString ) abstract;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
		// Constructs this Window instance.
		Window();

	public:
		// Destroys this Window instance.
		virtual ~Window();
	};

}	// namespace Foundation
}	// namespace Eldritch2
