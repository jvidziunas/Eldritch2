/*==================================================================*\
  Armature.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/CharTypes.hpp>
//------------------------------------------------------------------//
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_BUTTON_TRIGGER_ON_RELEASE
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_FONT_BAKING
#include <nuklear/nuklear.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace UserInterface {
namespace Nuklear {

	class Context {
	// - TYPE PUBLISHING ---------------------------------

	public:
		class ETPureAbstractHint Panel {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		protected:
		//!	Constructs this @ref Panel instance.
			Panel( Context& context );
		//!	Constructs this @ref Panel instance.
			Panel( const Panel& ) = default;

			~Panel() = default;

		// ---------------------------------------------------

		public:
			void	DoText( const Eldritch2::Utf8Char* const text, nk_flags alignment );
			void	DoText( const Eldritch2::Utf8Char* const text, int textOctetCount, nk_flags flags );
			void	DoText( const Eldritch2::Utf8Char* const text, nk_flags alignment, struct nk_color textColor );
			void	DoText( const Eldritch2::Utf8Char* const text, int textOctetCount, nk_flags flags, struct nk_color textColor );
			void	DoText( const Eldritch2::Utf8Char* const text );
			void	DoText( const Eldritch2::Utf8Char* const text, int textOctetCount );
			void	DoText( const Eldritch2::Utf8Char* const text, struct nk_color textColor );
			void	DoText( const Eldritch2::Utf8Char* const text, int textOctetCount, struct nk_color textColor );

		// ---------------------------------------------------

		public:
			void	DoImage( struct nk_image image );

		// ---------------------------------------------------

		public:
			bool	DoButton( const Eldritch2::Utf8Char* const text, int textOctetCount );
			bool	DoButton( const Eldritch2::Utf8Char* const text );
			bool	DoButton( struct nk_color buttonColor );
			bool	DoButton( struct nk_image image );
			bool	DoButton( struct nk_image image, const Eldritch2::Utf8Char* const text, nk_flags textAlignment );
			bool	DoButton( struct nk_image image, const Eldritch2::Utf8Char* const text, int textOctetCount, nk_flags textAlignment );

		// ---------------------------------------------------

		public:
			bool	DoCheckbox( const Eldritch2::Utf8Char* const text, bool* active );
			bool	DoCheckbox( const Eldritch2::Utf8Char* const text, int textOctetCount, bool* active );

		// ---------------------------------------------------

		public:
			bool	DoFlagsCheckbox( const Eldritch2::Utf8Char* const text, unsigned int* flags, unsigned int value );
			bool	DoFlagsCheckbox( const Eldritch2::Utf8Char* const text, int textOctetCount, unsigned int* flags, unsigned int value );

		// ---------------------------------------------------

		public:
			bool	DoRadioButton( const Eldritch2::Utf8Char* const text, int* active );
			bool	DoRadioButton( const Eldritch2::Utf8Char* const text, int textOctetCount, int* active );

			bool	DoRadioLabel( const Eldritch2::Utf8Char* const text, nk_flags textAlignment, int* value );
			bool	DoRadioLabel( const Eldritch2::Utf8Char* const text, int textOctetCount, nk_flags textAlignment, int* value );

		// ---------------------------------------------------

		public:
			bool	DoSlider( float min, float* val, float max, float step );
			bool	DoSlider( int min, int* val, int max, int step );

		// ---------------------------------------------------

		public:
			bool	DoProgressBar( nk_size* cur, nk_size max, bool modifyable );

		// ---------------------------------------------------

		public:
			bool	DoColorPicker( struct nk_color* color, enum nk_color_format format );

		// ---------------------------------------------------

		public:
			void	DoProperty( const Eldritch2::Utf8Char* const text, float min, float* val, float max, float step, float incrementPerPixel );
			void	DoProperty( const Eldritch2::Utf8Char* const text, int min, int* val, int max, int step, int incrementPerPixel );

		// ---------------------------------------------------

		public:
			nk_flags	DoTextEditBox( nk_flags flags, Eldritch2::Utf8Char* const text, int* len, int max, nk_plugin_filter filter );

		// - DATA MEMBERS ------------------------------------

		protected:
			Context&		_context;
			struct nk_panel	_panel;
		};

	// ---

	public:
		class Window : public Panel {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
		//!	Constructs this @ref Window instance.
			Window( Context& context, const Eldritch2::Utf8Char* const name, nk_flags flags, struct nk_rect bounds );

			~Window();
		};

	// ---

	public:
		class Popup : public Panel {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
		//!	Constructs this @ref Popup instance.
			Popup( Context& context, enum nk_popup_type type, const Eldritch2::Utf8Char* const name, nk_flags flags, struct nk_rect bounds );

			~Popup();
		};

	// ---

	public:
		class Tooltip : public Panel {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
		//!	Constructs this @ref Tooltip instance.
			Tooltip( Context& context, float width );

			~Tooltip();
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref Context instance.
		Context();

		~Context();

	// ---------------------------------------------------

	public:
		operator	struct nk_context*();

	// - DATA MEMBERS ------------------------------------

	private:
		struct nk_buffer	_commandPool;
		struct nk_context	_context;
	};

}	// namespace Nuklear
}	// namespace UserInterface
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <UserInterface/Nuklear/Context.inl>
//------------------------------------------------------------------//