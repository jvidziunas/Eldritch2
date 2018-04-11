/*==================================================================*\
  Interface.WrenScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Wren/ApiBuilder.hpp>
#include <Scripting/Wren/Interface.hpp>
#include <Graphics/RgbColor.hpp>
//------------------------------------------------------------------//
#include <wren.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scripting {
namespace Wren {

	using namespace ::Eldritch2::Scripting;
	using namespace ::Eldritch2::Graphics;

namespace {

	static ETInlineHint nk_flags GetSlotFlags( WrenVM* vm, int slot ) {
		return static_cast<nk_flags>( AsIntBits( wrenGetSlotDouble( vm, slot ) ));
	}

// ---------------------------------------------------

	static ETInlineHint nk_text_align GetSlotTextAlignment( WrenVM* vm, int slot ) {
		return static_cast<nk_text_align>( AsIntBits( wrenGetSlotDouble( vm, slot ) ) );
	}

// ---------------------------------------------------

	static ETInlineHint nk_color GetSlotColor( WrenVM* vm, int slot ) {
		const RgbColor& color( GetSlotAs<RgbColor>( vm, slot ) );

		return nk_rgb_f( color.GetRed(), color.GetGreen(), color.GetBlue() );
	}

}	// anonymous namespace

	ET_IMPLEMENT_WREN_CLASS( Interface ) {
		api.CreateClass<Interface>( ET_BUILTIN_WREN_MODULE_NAME( Interface ), "Interface",
			{/* Constructors */},
			{/*	Properties */},
			{/*	Methods */
				DefineMethod<void ( const Utf8Char*, double, WrenHandle* )>( "doWindow", [] ( WrenVM* vm ) {
					GetSlotAs<Interface>( vm, 0 ).DoWindow( wrenGetSlotString( vm, 1 ), GetSlotFlags( vm, 2 ), {}, [vm] ( Interface& /*interface*/ ) {
						wrenSwapSlots( vm, 0, 3 );
						wrenCall( vm, AsContext( vm ).GetUnaryCallStub() );
					} );
				} ),
				DefineMethod<void ( const Utf8Char*, double, WrenHandle* )>( "doStaticPopup", [] ( WrenVM* vm ) {
					GetSlotAs<Interface>( vm, 0 ).DoStaticPopup( wrenGetSlotString( vm, 1 ), GetSlotFlags( vm, 2 ), {}, [vm] ( Interface& /*interface*/ ) {
						wrenSwapSlots( vm, 0, 3 );
						wrenCall( vm, AsContext( vm ).GetUnaryCallStub() );
					} );
				} ),
				DefineMethod<void ( const Utf8Char*, double, WrenHandle* )>( "doDynamicPopup", [] ( WrenVM* vm ) {
					GetSlotAs<Interface>( vm, 0 ).DoDynamicPopup( wrenGetSlotString( vm, 1 ), GetSlotFlags( vm, 2 ), {}, [vm] ( Interface& /*interface*/ ) {
						wrenSwapSlots( vm, 0, 3 );
						wrenCall( vm, AsContext( vm ).GetUnaryCallStub() );
					} );

				} ),
				DefineMethod<void ( double, WrenHandle* )>( "doTooltip", [] ( WrenVM* vm ) {
					GetSlotAs<Interface>( vm, 0 ).DoTooltip( wrenGetSlotDouble( vm, 1 ), [vm] ( Interface& /*interface*/ ) {
						wrenSwapSlots( vm, 0, 3 );
						wrenCall( vm, AsContext( vm ).GetUnaryCallStub() );
					} );
				} ),
			/*	To consider: Strings are immutable in Wren.
				DefineMethod<void ( double, const Utf8Char*, WrenHandle* )>( "doTextEditBox", [] ( WrenVM* vm ) {
					GetSlotAs<Interface>( vm, 0 ).DoTextEditBox( wrenGetSlotDouble( vm, 1 ), wrenGetSlotString( vm, 2 ) )
				} ),
				DefineMethod<bool ( double, const Utf8Char*, double )>( "doButton", [] ( WrenVM* vm ) {
				//	imageId = wrenGetSlotDouble( vm, 1 );
					wrenSetSlotBool( vm, 0, GetSlotAs<Interface>( vm, 0 ).DoButton( nullptr, wrenGetSlotString( vm, 2 ), GetSlotTextAlignment( vm, 3 ) ) );
				} ) */
			//	bool	DoCheckbox( const Utf8Char* const text, unsigned int* flags, unsigned int value );
			//	bool	DoCheckbox( const Utf8Char* const text, bool* active );
			//	bool	DoRadioButton( const Utf8Char* const text, int* isActive );
			//	bool	DoRadioLabel( const Utf8Char* const text, nk_text_align alignment, int* value );
			//	bool	DoSlider( float min, float* val, float max, float step );
			//	bool	DoSlider( int min, int* val, int max, int step );
			//	bool	DoProgressBar( nk_size* cur, nk_size max, bool isMutable );
			//	bool	DoColorPicker( nk_colorf* color, nk_color_format format );
				DefineMethod<void ( const Utf8Char*, double, RgbColor )>( "doText", [] ( WrenVM* vm ) {
					GetSlotAs<Interface>( vm, 0 ).DoText( wrenGetSlotString( vm, 1 ), GetSlotFlags( vm, 2 ), GetSlotColor( vm, 3 ) );
				} ),
			//	void	DoImage( const Graphics::ImageSource& image );
				DefineMethod<double ( const Utf8Char*, double, double, double, double, double )>( "doProperty", [] ( WrenVM* vm ) {
					double value( wrenGetSlotDouble( vm, 3 ) );
					GetSlotAs<Interface>( vm, 0 ).DoProperty( wrenGetSlotString( vm, 1 ), wrenGetSlotDouble( vm, 2 ), &value, wrenGetSlotDouble( vm, 4 ), wrenGetSlotDouble( vm, 5 ), wrenGetSlotDouble( vm, 6 ) );
					wrenSetSlotDouble( vm, 0, value );
				} ),
			},
			{/*	Static methods */},
			{/*	Operators */}
		);
	}

}	// namespace Nuklear
}	// namespace UserInterface
}	// namespace Eldritch2