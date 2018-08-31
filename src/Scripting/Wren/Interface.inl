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

namespace Eldritch2 { namespace Scripting { namespace Wren {

	template <typename UnaryConsumer>
	ETInlineHint ETForceInlineHint void Interface::DoStaticLayoutRegion(float rowHeight, int columnWidth, int columns, UnaryConsumer commandGenerator) {
		nk_layout_row_static(&_context, rowHeight, columnWidth, columns);

		commandGenerator(*this);

		nk_layout_row_end(&_context);
	}

	// ---------------------------------------------------

	template <typename UnaryConsumer>
	ETInlineHint ETForceInlineHint void Interface::DoDynamicLayoutRegion(float rowHeight, int columns, UnaryConsumer commandGenerator) {
		nk_layout_row_dynamic(&_context, rowHeight, columns);

		commandGenerator(*this);

		nk_layout_row_end(&_context);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void Interface::DoText(const Utf8Char* const text, nk_flags alignment, struct nk_color color) {
		nk_label_colored(&_context, text, alignment, color);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void Interface::DoImage(const Graphics::ImageSource& image) {
		nk_image(&_context, nk_image_ptr(const_cast<Graphics::ImageSource*>(ETAddressOf(image))));
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint bool Interface::DoButton(const Graphics::ImageSource& image, const Utf8Char* const text, nk_text_align textAlignment) {
		return nk_button_image_label(&_context, nk_image_ptr(const_cast<Graphics::ImageSource*>(ETAddressOf(image))), text, textAlignment) != 0;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint bool Interface::DoCheckbox(const Utf8Char* const text, bool* active) {
		const bool initialState(*active);
		const bool result(0 != nk_check_label(&_context, text, initialState));

		if (result ^ initialState) {
			*active = result;
		}

		return result;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint bool Interface::DoCheckbox(const Utf8Char* const text, unsigned int* flags, unsigned int value) {
		return nk_checkbox_flags_label(&_context, text, flags, value) != 0;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint bool Interface::DoRadioButton(const Utf8Char* const text, int* active) {
		return nk_radio_label(&_context, text, active) != 0;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint bool Interface::DoRadioLabel(const Utf8Char* const text, nk_text_align textAlignment, int* value) {
		return nk_selectable_label(&_context, text, textAlignment, value) != 0;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint bool Interface::DoSlider(double min, double* val, double max, double step) {
		float value(*val);
		int   result(nk_slider_float(&_context, min, &value, max, step));

		return result != 0;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint bool Interface::DoProgressBar(nk_size* cur, nk_size max, bool isMutable) {
		return nk_progress(&_context, cur, max, isMutable ? 1 : 0) != 0;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint bool Interface::DoColorPicker(struct nk_colorf* color, enum nk_color_format format) {
		return nk_color_pick(&_context, color, format) != 0;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void Interface::DoProperty(const Utf8Char* const text, double min, double* val, double max, double step, double incrementPerPixel) {
		nk_property_double(&_context, text, min, val, max, step, incrementPerPixel);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint nk_flags Interface::DoTextEditBox(nk_flags flags, Utf8Char* const text, int* len, int max, nk_plugin_filter filter) {
		return nk_edit_string(&_context, flags, text, len, max, filter);
	}

	// ---------------------------------------------------

	template <typename UnaryConsumer>
	ETInlineHint ETForceInlineHint void Interface::DoWindow(const Utf8Char* const name, nk_flags flags, struct nk_rect bounds, UnaryConsumer implementation) {
		if (nk_begin(&_context, name, bounds, flags)) {
			implementation(*this);
		}

		nk_end(&_context);
	}

	// ---------------------------------------------------

	template <typename UnaryConsumer>
	ETInlineHint ETForceInlineHint void Interface::DoStaticPopup(const Utf8Char* const name, nk_flags flags, struct nk_rect bounds, UnaryConsumer implementation) {
		if (nk_popup_begin(&_context, nk_popup_type::NK_POPUP_STATIC, name, flags, bounds)) {
			implementation(*this);
		}

		nk_popup_end(&_context);
	}

	// ---------------------------------------------------

	template <typename UnaryConsumer>
	ETInlineHint ETForceInlineHint void Interface::DoDynamicPopup(const Utf8Char* const name, nk_flags flags, struct nk_rect bounds, UnaryConsumer implementation) {
		if (nk_popup_begin(&_context, nk_popup_type::NK_POPUP_DYNAMIC, name, flags, bounds)) {
			implementation(*this);
		}

		nk_popup_end(&_context);
	}

	// ---------------------------------------------------

	template <typename UnaryConsumer>
	ETInlineHint ETForceInlineHint void Interface::DoTooltip(float width, UnaryConsumer implementation) {
		if (nk_tooltip_begin(&_context, width)) {
			implementation(*this);
		}

		nk_tooltip_end(&_context);
	}

}}} // namespace Eldritch2::Scripting::Wren
