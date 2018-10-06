/*==================================================================*\
  Interface.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/MeshSource.hpp>
//------------------------------------------------------------------//
/*	(6011/28182) Possible null pointer dereference
 *	(6386) Possible buffer overrun */
ET_PUSH_MSVC_WARNING_STATE(disable : 6011 6386 28182)
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_BUTTON_TRIGGER_ON_RELEASE
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_COMMAND_USERDATA
#include <nuklear/nuklear.h>
ET_POP_MSVC_WARNING_STATE()
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics {
	class ImageSource;
}} // namespace Eldritch2::Graphics

namespace Eldritch2 { namespace Scripting { namespace Wren {

	class Interface {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		Interface(const Interface&) = delete;
		//!	Constructs this @ref Interface instance.
		Interface();

		~Interface();

		// ---------------------------------------------------

	public:
		template <typename UnaryConsumer>
		void DoStaticLayoutRegion(float rowHeight, int columnWidth, int columns, UnaryConsumer commandGenerator);

		template <typename UnaryConsumer>
		void DoDynamicLayoutRegion(float rowHeight, int columns, UnaryConsumer commandGenerator);

		// ---------------------------------------------------

	public:
		nk_flags DoTextEditBox(nk_flags flags, Utf8Char* const text, int* len, int max, nk_plugin_filter filter);

		bool DoButton(const Graphics::ImageSource& image, const Utf8Char* const text, nk_text_align alignment);

		bool DoCheckbox(const Utf8Char* text, unsigned int* flags, unsigned int value);
		bool DoCheckbox(const Utf8Char* text, bool* active);

		bool DoRadioButton(const Utf8Char* text, int* isActive);

		bool DoRadioLabel(const Utf8Char* text, nk_text_align alignment, int* value);

		bool DoSlider(double min, double* val, double max, double step);

		bool DoProgressBar(nk_size* cur, nk_size max, bool isMutable);

		bool DoColorPicker(nk_colorf* color, nk_color_format format);

		// ---------------------------------------------------

	public:
		void DoText(nk_flags alignment, struct nk_color color, const Utf8Char* text);

		void DoImage(const Graphics::ImageSource& image);

		void DoProperty(double min, double* val, double max, double step, double incrementPerPixel, const Utf8Char* text);

		// ---------------------------------------------------

	public:
		template <typename UnaryConsumer>
		void DoStaticPopup(const Utf8Char* name, nk_flags flags, struct nk_rect bounds, UnaryConsumer implementation);

		template <typename UnaryConsumer>
		void DoDynamicPopup(const Utf8Char* name, nk_flags flags, struct nk_rect bounds, UnaryConsumer implementation);

		template <typename UnaryConsumer>
		void DoWindow(const Utf8Char* name, nk_flags flags, struct nk_rect bounds, UnaryConsumer implementation);

		template <typename UnaryConsumer>
		void DoTooltip(float width, UnaryConsumer implementation);

		// ---------------------------------------------------

		ETConstexpr operator const nk_context*() const ETNoexceptHint;
		ETConstexpr operator nk_context*() ETNoexceptHint;

		// ---------------------------------------------------

	public:
		ErrorCode BindResources();

		void FreeResources();

		// - DATA MEMBERS ------------------------------------

	private:
		nk_buffer  _commandPool;
		nk_context _context;
	};

}}} // namespace Eldritch2::Scripting::Wren

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Scripting/Wren/Interface.inl>
//------------------------------------------------------------------//
