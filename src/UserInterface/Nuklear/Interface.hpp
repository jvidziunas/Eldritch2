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
#include <Graphics/GeometrySource.hpp>
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
			class	Interface;
		}
	}

	namespace Graphics {
		class	ImageSource;
	}

	namespace Input {
		class	Keyboard;
		class	Mouse;
	}
}

namespace Eldritch2 {
namespace UserInterface {
namespace Nuklear {

	struct Image {
		const Graphics::ImageSource* source;
	};

// ---

	class Panel {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref Panel instance.
		Panel( Interface& owner );
	//!	Disable copy construction.
		Panel( const Panel& ) = delete;

		~Panel() = default;

	// ---------------------------------------------------

	public:
		template <typename UnaryConsumer>
		void	DoFixedLayoutRegion( float rowHeight, int columnWidth, int columns, UnaryConsumer commandGenerator );

		template <typename UnaryConsumer>
		void	DoDynamicLayoutRegion( float rowHeight, int columns, UnaryConsumer commandGenerator );

	// ---------------------------------------------------

	public:
		nk_flags	DoTextEditBox( nk_flags flags, Utf8Char* const text, int* len, int max, nk_plugin_filter filter );

		void		DoText( const Utf8Char* const text, nk_flags alignment, nk_color textColor );

		void		DoImage( const Image& image );

		bool		DoButton( const Image& image, const Utf8Char* const text, nk_text_align textAlignment );

		bool		DoCheckbox( const Utf8Char* const text, unsigned int* flags, unsigned int value );
		bool		DoCheckbox( const Utf8Char* const text, bool* active );

		bool		DoRadioButton( const Utf8Char* const text, int* isActive );

		bool		DoRadioLabel( const Utf8Char* const text, nk_text_align textAlignment, int* value );

		bool		DoSlider( float min, float* val, float max, float step );
		bool		DoSlider( int min, int* val, int max, int step );

		bool		DoProgressBar( nk_size* cur, nk_size max, bool isMutable );

		bool		DoColorPicker( nk_colorf* color, nk_color_format format );

		void		DoProperty( const Utf8Char* const text, float min, float* val, float max, float step, float incrementPerPixel );
		void		DoProperty( const Utf8Char* const text, int min, int* val, int max, int step, int incrementPerPixel );

	// - DATA MEMBERS ------------------------------------

	private:
		nk_context*	_context;
		nk_panel	_panel;
	};

// ---

	class Interface : public Graphics::GeometrySource {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Disable copy construction.
		Interface( const Interface& ) = delete;
	//!	Constructs this @ref UiContext instance.
		Interface();

		~Interface();

	// ---------------------------------------------------

	public:
		template <typename UnaryConsumer>
		void	DoStaticPopup( const Utf8Char* const name, nk_flags flags, struct nk_rect bounds, UnaryConsumer bodyGenerator );

		template <typename UnaryConsumer>
		void	DoDynamicPopup( const Utf8Char* const name, nk_flags flags, struct nk_rect bounds, UnaryConsumer bodyGenerator );

		template <typename UnaryConsumer>
		void	DoWindow( const Utf8Char* const name, nk_flags flags, struct nk_rect bounds, UnaryConsumer bodyGenerator );

		template <typename UnaryConsumer>
		void	DoTooltip( float width, UnaryConsumer bodyGenerator );

	// ---------------------------------------------------

	public:
		void	ConsumeInput( const Input::Keyboard& keyboard );
		void	ConsumeInput( const Input::Mouse& mouse );

	// ---------------------------------------------------

	public:
		void	Stream( const VertexStreamRequest& vertices, const IndexStreamRequest& indices ) const override;
		
	// ---------------------------------------------------

	public:
		void	Clear();

	// ---------------------------------------------------

	public:
		operator	nk_context*();

	// - DATA MEMBERS ------------------------------------

	private:
		mutable nk_buffer	_commandPool;
		mutable nk_context	_context;
	};

}	// namespace Nuklear
}	// namespace UserInterface
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <UserInterface/Nuklear/Interface.inl>
//------------------------------------------------------------------//