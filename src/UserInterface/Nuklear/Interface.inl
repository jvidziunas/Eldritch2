/*==================================================================*\
  Interface.inl
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
namespace {

	ETInlineHint struct nk_image ImageFromSource( const Image& image ) {
	//	const_cast required since this is a C API without a notion of const.
		return nk_subimage_ptr( const_cast<Graphics::ImageSource*>(image.source), 0, 0, { 0, 0 } );
	}

}	// anonymous namespace

	ETInlineHint Panel::Panel( Interface& context ) : _context( context ) {}

// ---------------------------------------------------

	template <typename UnaryConsumer>
	ETInlineHint void Panel::DoFixedLayoutRegion( float rowHeight, int columnWidth, int columns, UnaryConsumer commandGenerator ) {
		nk_layout_row_static( _context, rowHeight, columnWidth, columns );

		commandGenerator( *this );

		nk_layout_row_end( _context );
	}

// ---------------------------------------------------

	template <typename UnaryConsumer>
	ETInlineHint void Panel::DoDynamicLayoutRegion( float rowHeight, int columns, UnaryConsumer commandGenerator ) {
		nk_layout_row_dynamic( _context, rowHeight, columns );

		commandGenerator( *this );

		nk_layout_row_end( _context );
	}

// ---------------------------------------------------

	ETInlineHint void Panel::DoText( const Utf8Char* const text, nk_flags alignment, struct nk_color color ) {
		nk_label_colored( _context, text, alignment, color );
	}

// ---------------------------------------------------

	ETInlineHint void Panel::DoImage( const Image& image ) {
		nk_image( _context, ImageFromSource( image ) );
	}

// ---------------------------------------------------

	ETInlineHint bool Panel::DoButton( const Image& image, const Utf8Char* const text, nk_text_align textAlignment ) {
		return nk_button_image_label( _context, ImageFromSource( image ), text, textAlignment ) != 0;
	}

// ---------------------------------------------------

	ETInlineHint bool Panel::DoCheckbox( const Utf8Char* const text, bool* active ) {
		const bool	initialState( *active );
		const bool	result( 0 != nk_check_label( _context, text, initialState ) );

		if (result ^ initialState) {
			*active = result;
		}

		return result;
	}

// ---------------------------------------------------

	ETInlineHint bool Panel::DoCheckbox( const Utf8Char* const text, unsigned int* flags, unsigned int value ) {
		return nk_checkbox_flags_label( _context, text, flags, value ) != 0;
	}

// ---------------------------------------------------

	ETInlineHint bool Panel::DoRadioButton( const Utf8Char* const text, int* active ) {
		return nk_radio_label( _context, text, active ) != 0;
	}

// ---------------------------------------------------

	ETInlineHint bool Panel::DoRadioLabel( const Utf8Char* const text, nk_text_align textAlignment, int* value ) {
		return nk_selectable_label( _context, text, textAlignment, value ) != 0;
	}

// ---------------------------------------------------

	ETInlineHint bool Panel::DoSlider( float min, float* val, float max, float step ) {
		return nk_slider_float( _context, min, val, max, step ) != 0;
	}

// ---------------------------------------------------

	ETInlineHint bool Panel::DoSlider( int min, int* val, int max, int step ) {
		return nk_slider_int( _context, min, val, max, step ) != 0;
	}

// ---------------------------------------------------

	ETInlineHint bool Panel::DoProgressBar( nk_size* cur, nk_size max, bool isMutable ) {
		return nk_progress( _context, cur, max, isMutable ? 1 : 0 ) != 0;
	}

// ---------------------------------------------------

	ETInlineHint bool Panel::DoColorPicker( struct nk_colorf* color, enum nk_color_format format ) {
		return nk_color_pick( _context, color, format ) != 0;
	}

// ---------------------------------------------------

	ETInlineHint void Panel::DoProperty( const Utf8Char* const text, float min, float* val, float max, float step, float incrementPerPixel ) {
		nk_property_float( _context, text, min, val, max, step, incrementPerPixel );
	}

// ---------------------------------------------------

	ETInlineHint void Panel::DoProperty( const Utf8Char* const text, int min, int* val, int max, int step, int incrementPerPixel ) {
		nk_property_int( _context, text, min, val, max, step, incrementPerPixel );
	}

// ---------------------------------------------------

	ETInlineHint nk_flags Panel::DoTextEditBox( nk_flags flags, Utf8Char* const text, int* len, int max, nk_plugin_filter filter ) {
		return nk_edit_string( _context, flags, text, len, max, filter );
	}

// ---------------------------------------------------

	ETInlineHint Interface::operator struct nk_context*() {
		return &_context;
	}

// ---------------------------------------------------

	template <typename UnaryConsumer>
	ETInlineHint void Interface::DoWindow( const Utf8Char* const name, nk_flags flags, struct nk_rect bounds, UnaryConsumer commandGenerator ) {
		if (nk_begin( *this, name, bounds, flags ) == 0) {
			return;
		}

		Panel	panel( *this );

		commandGenerator( panel );

		nk_end( &_context );
	}

// ---------------------------------------------------

	template <typename UnaryConsumer>
	ETInlineHint void Interface::DoStaticPopup( const Utf8Char* const name, nk_flags flags, struct nk_rect bounds, UnaryConsumer commandGenerator ) {
		if (nk_popup_begin( *this, nk_popup_type::NK_POPUP_STATIC, name, flags, bounds ) == 0) {
			return;
		}

		Panel	panel( *this );

		commandGenerator( panel );

		nk_popup_end( &_context );
	}

// ---------------------------------------------------

	template <typename UnaryConsumer>
	ETInlineHint void Interface::DoDynamicPopup( const Utf8Char* const name, nk_flags flags, struct nk_rect bounds, UnaryConsumer commandGenerator ) {
		if (nk_popup_begin( *this, nk_popup_type::NK_POPUP_DYNAMIC, name, flags, bounds ) == 0) {
			return;
		}
		
		Panel	panel( *this );

		commandGenerator( panel );

		nk_popup_end( &_context );
	}

// ---------------------------------------------------

	template <typename UnaryConsumer>
	ETInlineHint void Interface::DoTooltip( float width, UnaryConsumer commandGenerator ) {
		if (nk_tooltip_begin( *this, width ) == 0) {
			return;
		}

		Panel	panel( *this );

		commandGenerator( panel );

		nk_tooltip_end( &_context );
	}

}	// namespace Nuklear
}	// namespace UserInterface
}	// namespace Eldritch2