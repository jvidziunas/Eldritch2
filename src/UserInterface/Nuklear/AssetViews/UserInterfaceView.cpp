/*==================================================================*\
  UserInterfaceView.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <UserInterface/Nuklear/AssetViews/UserInterfaceView.hpp>
#include <Utility/Memory/InstanceDeleter.hpp>
#include <UserInterface/Nuklear/Context.hpp>
#include <Assets/AssetLibrary.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Assets;

namespace Eldritch2 {
namespace UserInterface {
namespace Nuklear {
namespace AssetViews {
namespace {

	class TextWidget : public UserInterfaceView::Widget {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref TextWidget instance.
		TextWidget() {}

		~TextWidget() = default;

	// ---------------------------------------------------

	public:
		void Draw( Context::Panel& panel ) const override {
			panel.DoText( "", _flags, _color );
		}

	// - DATA MEMBERS ------------------------------------

	private:
		nk_flags		_flags;
		struct nk_color	_color;
	};

// ---

	class ImageWidget : public UserInterfaceView::Widget {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref ImageWidget instance.
		ImageWidget( struct nk_image image ) : _image( image ) {}

		~ImageWidget() = default;

	// ---------------------------------------------------

	public:
		void Draw( Context::Panel& panel ) const override {
			panel.DoImage( _image );
		}

	// - DATA MEMBERS ------------------------------------

	private:
		struct nk_image	_image;
	};

// ---

	class TextButtonWidget : public UserInterfaceView::Widget {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref TextButtonWidget instance.
		TextButtonWidget() {}

		~TextButtonWidget() = default;

	// ---------------------------------------------------

	public:
		void Draw( Context::Panel& panel ) const override {
			panel.DoButton( "", _behavior );
		}

	// - DATA MEMBERS ------------------------------------

	private:
		enum nk_button_behavior	_behavior;
	};

// ---

	class ImageTextButtonWidget : public UserInterfaceView::Widget {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref ImageTextButtonWidget instance.
		ImageTextButtonWidget() {}

		~ImageTextButtonWidget() = default;

	// ---------------------------------------------------

	public:
		void Draw( Context::Panel& panel ) const override {
			panel.DoButton( _image, "", _textAlignment, _behavior );
		}

	// - DATA MEMBERS ------------------------------------

	private:
		struct nk_image			_image;
		nk_flags				_textAlignment;
		enum nk_button_behavior	_behavior;
	};

// ---

	class CheckboxWidget : public UserInterfaceView::Widget {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref CheckboxWidget instance.
		CheckboxWidget() {}

		~CheckboxWidget() = default;

	// ---------------------------------------------------

	public:
		void Draw( Nuklear::Context::Panel& panel ) const override {
			panel.DoCheckbox( "", nullptr );
		}

	// - DATA MEMBERS ------------------------------------

	private:

	};

// ---

	class RadioButtonWidget : public UserInterfaceView::Widget {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref RadioButtonWidget instance.
		RadioButtonWidget() {}

		~RadioButtonWidget() = default;

	// ---------------------------------------------------

	public:
		void Draw( Context::Panel& panel ) const override {
			panel.DoRadioButton( "", nullptr );
		//	context.DoRadioLabel( "", _textAlignment, int* value );
		}

	// - DATA MEMBERS ------------------------------------

	private:
		nk_flags	_textAlignment;
	};

// ---

	template <typename Value>
	class SliderWidget : public UserInterfaceView::Widget {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref SliderWidget instance.
		SliderWidget() {}

		~SliderWidget() = default;

	// ---------------------------------------------------

	public:
		void	Draw( Context::Panel& panel ) const override;

	// - DATA MEMBERS ------------------------------------

	private:
		Value	_min;
		Value	_max;
		Value	_step;
	};

// ---

	class ProgressBarWidget : public UserInterfaceView::Widget {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref ProgressBarWidget instance.
		ProgressBarWidget() {}

		~ProgressBarWidget() = default;

	// ---------------------------------------------------

	public:
		void Draw( Context::Panel& panel ) const override {
			panel.DoProgressBar( nullptr, _max, false );
		}

	// - DATA MEMBERS ------------------------------------

	private:
		nk_size	_max;
	};

// ---

	class ColorPickerWidget : public UserInterfaceView::Widget {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref ColorPickerWidget instance.
		ColorPickerWidget() {}

		~ColorPickerWidget() = default;

	// ---------------------------------------------------

	public:
		void Draw( Context::Panel& panel ) const override {
			panel.DoColorPicker( nullptr, _allowAlpha ? NK_RGBA : NK_RGB );
		}

	// - DATA MEMBERS ------------------------------------

	private:
		bool	_allowAlpha;
	};

// ---

	template <typename Value>
	class PropertyWidget : public UserInterfaceView::Widget {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref PropertyWidget instance.
		PropertyWidget() {}

		~PropertyWidget() = default;

	// ---------------------------------------------------

	public:
		void Draw( Context::Panel& panel ) const override {
			panel.DoProperty( "", nullptr, _min, _max, _step, _increment );
		}

	// - DATA MEMBERS ------------------------------------

	private:
		Value	_min;
		Value	_max;
		Value	_step;
		Value	_increment;
	};

}	// anonymous namespace

// ---------------------------------------------------

	UserInterfaceView::UserInterfaceView( const Utf8Char* const name, Allocator& allocator ) : AssetView( name ), _allocator( allocator, "Nuklear User Interface Asset View Allocator" ) {}

// ---------------------------------------------------

	void UserInterfaceView::Draw( Context& context ) const {
		Context::Window	window( context, "", NK_WINDOW_BORDER, { 0, 0, 1024, 768 } );

		for( const auto& widget : _widgets ) {
			widget->Draw( window );
		}
	}

}	// namespace AssetViews
}	// namespace Nuklear
}	// namespace UserInterface
}	// namespace Eldritch2
