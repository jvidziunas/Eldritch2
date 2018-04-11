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
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_BUTTON_TRIGGER_ON_RELEASE
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_FONT_BAKING
#include <nuklear/nuklear.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Graphics {
		class	ImageSource;
	}
}

namespace Eldritch2 {
namespace Scripting {
namespace Wren {

	class Interface : public Graphics::MeshSource {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Disable copy construction.
		Interface( const Interface& ) = delete;
	//!	Constructs this @ref Interface instance.
		Interface();

		~Interface();

	// ---------------------------------------------------

	public:
		template <typename UnaryConsumer>
		void	DoStaticLayoutRegion( float rowHeight, int columnWidth, int columns, UnaryConsumer commandGenerator );

		template <typename UnaryConsumer>
		void	DoDynamicLayoutRegion( float rowHeight, int columns, UnaryConsumer commandGenerator );

	// ---------------------------------------------------

	public:
		nk_flags	DoTextEditBox( nk_flags flags, Utf8Char* const text, int* len, int max, nk_plugin_filter filter );

		bool		DoButton( const Graphics::ImageSource& image, const Utf8Char* const text, nk_text_align alignment );

		bool		DoCheckbox( const Utf8Char* const text, unsigned int* flags, unsigned int value );
		bool		DoCheckbox( const Utf8Char* const text, bool* active );

		bool		DoRadioButton( const Utf8Char* const text, int* isActive );

		bool		DoRadioLabel( const Utf8Char* const text, nk_text_align alignment, int* value );

		bool		DoSlider( double min, double* val, double max, double step );

		bool		DoProgressBar( nk_size* cur, nk_size max, bool isMutable );

		bool		DoColorPicker( nk_colorf* color, nk_color_format format );

	// ---------------------------------------------------

	public:
		void	DoText( const Utf8Char* const text, nk_flags alignment, nk_color textColor );

		void	DoImage( const Graphics::ImageSource& image );

		void	DoProperty( const Utf8Char* const text, double min, double* val, double max, double step, double incrementPerPixel );

	// ---------------------------------------------------

	public:
		template <typename UnaryConsumer>
		void	DoStaticPopup( const Utf8Char* const name, nk_flags flags, struct nk_rect bounds, UnaryConsumer implementation );

		template <typename UnaryConsumer>
		void	DoDynamicPopup( const Utf8Char* const name, nk_flags flags, struct nk_rect bounds, UnaryConsumer implementation );

		template <typename UnaryConsumer>
		void	DoWindow( const Utf8Char* const name, nk_flags flags, struct nk_rect bounds, UnaryConsumer implementation );

		template <typename UnaryConsumer>
		void	DoTooltip( float width, UnaryConsumer implementation );

	// ---------------------------------------------------

	public:
		void	Stream( const VertexStreamRequest& vertices, const IndexStreamRequest& indices ) const override;
		
	// ---------------------------------------------------

	public:
		void	Clear();

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
#include <Scripting/Wren/Interface.inl>
//------------------------------------------------------------------//