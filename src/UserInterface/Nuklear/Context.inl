/*==================================================================*\
  Context.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
namespace UserInterface {
namespace Nuklear {

	ETInlineHint Context::Panel::Panel( Context& context ) : _context( context ) {}

// ---------------------------------------------------

	ETInlineHint void Context::Panel::DoText( const Eldritch2::Utf8Char* const text, int textOctetCount, nk_flags textAlignment ) {
		nk_text( _context, text, textOctetCount, textAlignment );
	}

// ---------------------------------------------------

	ETInlineHint void Context::Panel::DoText( const Eldritch2::Utf8Char* const text, int textOctetCount, nk_flags textAlignment, struct nk_color color ) {
		nk_text_colored( _context, text, textOctetCount, textAlignment, color );
	}

// ---------------------------------------------------

	ETInlineHint void Context::Panel::DoText( const Eldritch2::Utf8Char* const text, int textOctetCount ) {
		nk_text_wrap( _context, text, textOctetCount );
	}

// ---------------------------------------------------

	ETInlineHint void Context::Panel::DoText( const Eldritch2::Utf8Char* const text, int textOctetCount, struct nk_color color ) {
		nk_text_wrap_colored( _context, text, textOctetCount, color );
	}

// ---------------------------------------------------

	ETInlineHint void Context::Panel::DoText( const Eldritch2::Utf8Char* const text, nk_flags alignment ) {
		nk_label( _context, text, alignment );
	}

// ---------------------------------------------------

	ETInlineHint void Context::Panel::DoText( const Eldritch2::Utf8Char* const text, nk_flags alignment, struct nk_color color ) {
		nk_label_colored( _context, text, alignment, color );
	}

// ---------------------------------------------------

	ETInlineHint void Context::Panel::DoText( const Eldritch2::Utf8Char* const text ) {
		nk_label_wrap( _context, text );
	}

// ---------------------------------------------------

	ETInlineHint void Context::Panel::DoText( const Eldritch2::Utf8Char* const text, struct nk_color color ) {
		nk_label_colored_wrap( _context, text, color );
	}

// ---------------------------------------------------

	ETInlineHint void Context::Panel::DoImage( struct nk_image image ) {
		nk_image( _context, image );
	}

// ---------------------------------------------------

	ETInlineHint bool Context::Panel::DoButton( const Eldritch2::Utf8Char* const text, int textOctetCount ) {
		return 0 != nk_button_text( _context, text, textOctetCount );
	}

// ---------------------------------------------------

	ETInlineHint bool Context::Panel::DoButton( const Eldritch2::Utf8Char* const text ) {
		return 0 != nk_button_label( _context, text );
	}

// ---------------------------------------------------

	ETInlineHint bool Context::Panel::DoButton( struct nk_color color ) {
		return 0 != nk_button_color( _context, color );
	}

// ---------------------------------------------------

	ETInlineHint bool Context::Panel::DoButton( struct nk_image image ) {
		return 0 != nk_button_image( _context, image );
	}

// ---------------------------------------------------

	ETInlineHint bool Context::Panel::DoButton( struct nk_image image, const Eldritch2::Utf8Char* const text, nk_flags textAlignment ) {
		return 0 != nk_button_image_label( _context, image, text, textAlignment );
	}

// ---------------------------------------------------

	ETInlineHint bool Context::Panel::DoButton( struct nk_image image, const Eldritch2::Utf8Char* const text, int textOctetCount, nk_flags textAlignment ) {
		return 0 != nk_button_image_text( _context, image, text, textOctetCount, textAlignment );
	}

// ---------------------------------------------------

	ETInlineHint bool Context::Panel::DoCheckbox( const Eldritch2::Utf8Char* const text, bool* active ) {
		const bool	initialState( *active );
		const bool	result( 0 != nk_check_label( _context, text, initialState ) );

		if( result ^ initialState ) {
			*active = result;
		}

		return result;
	}

// ---------------------------------------------------

	ETInlineHint bool Context::Panel::DoCheckbox( const Eldritch2::Utf8Char* const text, int textOctetCount, bool* active ) {
		const bool	initialState( *active );
		const bool	result( 0 != nk_check_text( _context, text, textOctetCount, initialState ) );

		if( result ^ initialState ) {
			*active = result;
		}

		return result;
	}

// ---------------------------------------------------

	ETInlineHint bool Context::Panel::DoFlagsCheckbox( const Eldritch2::Utf8Char* const text, unsigned int* flags, unsigned int value ) {
		return 0 != nk_checkbox_flags_label( _context, text, flags, value );
	}

// ---------------------------------------------------

	ETInlineHint bool Context::Panel::DoFlagsCheckbox( const Eldritch2::Utf8Char* const text, int textOctetCount, unsigned int* flags, unsigned int value ) {
		return 0 != nk_checkbox_flags_text( _context, text, textOctetCount, flags, value );
	}

// ---------------------------------------------------

	ETInlineHint bool Context::Panel::DoRadioButton( const Eldritch2::Utf8Char* const text, int* active ) {
		return 0 != nk_radio_label( _context, text, active );
	}

// ---------------------------------------------------

	ETInlineHint bool Context::Panel::DoRadioButton( const Eldritch2::Utf8Char* const text, int textOctetCount, int* active ) {
		return 0 != nk_radio_text( _context, text, textOctetCount, active );
	}

// ---------------------------------------------------

	ETInlineHint bool Context::Panel::DoRadioLabel( const Eldritch2::Utf8Char* const text, nk_flags textAlignment, int* value ) {
		return 0 != nk_selectable_label( _context, text, textAlignment, value );
	}

// ---------------------------------------------------

	ETInlineHint bool Context::Panel::DoRadioLabel( const Eldritch2::Utf8Char* const text, int textOctetCount, nk_flags textAlignment, int* value ) {
		return 0 != nk_selectable_text( _context, text, textOctetCount, textAlignment, value );
	}

// ---------------------------------------------------

	ETInlineHint bool Context::Panel::DoSlider( float min, float* val, float max, float step ) {
		return 0 != nk_slider_float( _context, min, val, max, step );
	}

// ---------------------------------------------------

	ETInlineHint bool Context::Panel::DoSlider( int min, int* val, int max, int step ) {
		return 0 != nk_slider_int( _context, min, val, max, step );
	}

// ---------------------------------------------------

	ETInlineHint bool Context::Panel::DoProgressBar( nk_size* cur, nk_size max, bool modifyable ) {
		return 0 != nk_progress( _context, cur, max, modifyable ? 1 : 0 );
	}

// ---------------------------------------------------

	ETInlineHint bool Context::Panel::DoColorPicker( struct nk_color* color, enum nk_color_format format ) {
		return 0 != nk_color_pick( _context, color, format );
	}

// ---------------------------------------------------

	ETInlineHint void Context::Panel::DoProperty( const Eldritch2::Utf8Char* const text, float min, float* val, float max, float step, float incrementPerPixel ) {
		nk_property_float( _context, text, min, val, max, step, incrementPerPixel );
	}

// ---------------------------------------------------

	ETInlineHint void Context::Panel::DoProperty( const Eldritch2::Utf8Char* const text, int min, int* val, int max, int step, int incrementPerPixel ) {
		nk_property_int( _context, text, min, val, max, step, incrementPerPixel );
	}

// ---------------------------------------------------

	ETInlineHint nk_flags Context::Panel::DoTextEditBox( nk_flags flags, Eldritch2::Utf8Char* const text, int* len, int max, nk_plugin_filter filter ) {
		nk_edit_string( _context, flags, text, len, max, filter );
	}

// ---------------------------------------------------

	ETInlineHint Context::operator struct nk_context*() {
		return &_context;
	}

// ---------------------------------------------------

	ETInlineHint Context::Window::Window( Context& context, const Eldritch2::Utf8Char* const name, nk_flags flags, struct nk_rect bounds ) : Panel( context ) {
		nk_begin( context, &_panel, name, bounds, flags );
	}

// ---------------------------------------------------

	ETInlineHint Context::Window::~Window() {
		nk_end( _context );
	}

// ---------------------------------------------------

	ETInlineHint Context::Popup::Popup( Context& context, enum nk_popup_type type, const Eldritch2::Utf8Char* const name, nk_flags flags, struct nk_rect bounds ) : Panel( context ) {
		nk_popup_begin( context, &_panel, type, name, flags, bounds );
	}

// ---------------------------------------------------

	ETInlineHint Context::Popup::~Popup() {
		nk_popup_end( _context );
	}

// ---------------------------------------------------

	ETInlineHint Context::Tooltip::Tooltip( Context& context, float width ) : Panel( context ) {
		nk_tooltip_begin( context, &_panel, width );
	}

// ---------------------------------------------------

	ETInlineHint Context::Tooltip::~Tooltip() {
		nk_tooltip_end( _context );
	}

}	// namespace Nuklear
}	// namespace UserInterface
}	// namespace Eldritch2