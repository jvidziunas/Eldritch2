/*==================================================================*\
  InterfaceAsset.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <UserInterface/Nuklear/AssetViews/InterfaceAsset.hpp>
#include <UserInterface/Nuklear/Interface.hpp>
#include <Assets/AssetDatabase.hpp>
//------------------------------------------------------------------//
#include <Flatbuffers/NuklearUi_generated.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace UserInterface {
namespace Nuklear {
namespace AssetViews {
namespace {

	class TextWidget : public InterfaceAsset::Widget {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref TextWidget instance.
		TextWidget() {}

		~TextWidget() = default;

	// ---------------------------------------------------

	public:
		void Draw( Panel& panel ) const override {
			panel.DoText( "", _flags, _color );
		}

	// - DATA MEMBERS ------------------------------------

	private:
		nk_flags		_flags;
		struct nk_color	_color;
	};

// ---

	class ImageWidget : public InterfaceAsset::Widget {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref ImageWidget instance.
		ImageWidget( const Image& image ) : _image( image ) {}

		~ImageWidget() = default;

	// ---------------------------------------------------

	public:
		void Draw( Panel& panel ) const override {
			panel.DoImage( _image );
		}

	// - DATA MEMBERS ------------------------------------

	private:
		const Image& _image;
	};

// ---

	class TextButtonWidget : public InterfaceAsset::Widget {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref TextButtonWidget instance.
		TextButtonWidget( const Image& image ) : _image( image ) {}

		~TextButtonWidget() = default;

	// ---------------------------------------------------

	public:
		void Draw( Panel& panel ) const override {
			panel.DoButton( _image, "", NK_TEXT_ALIGN_CENTERED );
		}

	// - DATA MEMBERS ------------------------------------

	private:
		enum nk_button_behavior	_behavior;
		const Image&			_image;
	};

// ---

	class ImageTextButtonWidget : public InterfaceAsset::Widget {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref ImageTextButtonWidget instance.
		ImageTextButtonWidget( nk_button_behavior behavior, const Image& image ) : _behavior( behavior ), _image( image ) {}

		~ImageTextButtonWidget() = default;

	// ---------------------------------------------------

	public:
		void Draw( Panel& panel ) const override {
			panel.DoButton( _image, "", NK_TEXT_ALIGN_CENTERED );
		}

	// - DATA MEMBERS ------------------------------------

	private:
		nk_button_behavior	_behavior;
		const Image&		_image;
	};

// ---

	class CheckboxWidget : public InterfaceAsset::Widget {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref CheckboxWidget instance.
		CheckboxWidget() {}

		~CheckboxWidget() = default;

	// ---------------------------------------------------

	public:
		void Draw( Panel& panel ) const override {
			panel.DoCheckbox( "", nullptr );
		}

	// - DATA MEMBERS ------------------------------------

	private:

	};

// ---

	class RadioButtonWidget : public InterfaceAsset::Widget {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref RadioButtonWidget instance.
		RadioButtonWidget() {}

		~RadioButtonWidget() = default;

	// ---------------------------------------------------

	public:
		void Draw( Panel& panel ) const override {
			panel.DoRadioButton( "", nullptr );
		}

	// - DATA MEMBERS ------------------------------------

	private:
		nk_flags	_textAlignment;
	};

// ---

	template <typename Value>
	class SliderWidget : public InterfaceAsset::Widget {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref SliderWidget instance.
		SliderWidget() {}

		~SliderWidget() = default;

	// ---------------------------------------------------

	public:
		void Draw( Panel& panel ) const override {
			Value	value;

			panel.DoSlider( _min, &value, _max, _step );
		}

	// - DATA MEMBERS ------------------------------------

	private:
		Value	_min;
		Value	_max;
		Value	_step;
	};

// ---

	class ProgressBarWidget : public InterfaceAsset::Widget {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref ProgressBarWidget instance.
		ProgressBarWidget() {}

		~ProgressBarWidget() = default;

	// ---------------------------------------------------

	public:
		void Draw( Panel& panel ) const override {
			panel.DoProgressBar( nullptr, _max, false );
		}

	// - DATA MEMBERS ------------------------------------

	private:
		nk_size	_max;
	};

// ---

	class ColorPickerWidget : public InterfaceAsset::Widget {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref ColorPickerWidget instance.
		ColorPickerWidget() {}

		~ColorPickerWidget() = default;

	// ---------------------------------------------------

	public:
		void Draw( Panel& panel ) const override {
			panel.DoColorPicker( nullptr, _allowAlpha ? NK_RGBA : NK_RGB );
		}

	// - DATA MEMBERS ------------------------------------

	private:
		bool	_allowAlpha;
	};

// ---

	template <typename Value>
	class PropertyWidget : public InterfaceAsset::Widget {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref PropertyWidget instance.
		PropertyWidget() {}

		~PropertyWidget() = default;

	// ---------------------------------------------------

	public:
		void Draw( Panel& panel ) const override {
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

	using namespace ::Eldritch2::UserInterface::Nuklear::FlatBuffers;
	using namespace ::Eldritch2::Logging;
	using namespace ::Eldritch2::Assets;
	using namespace ::flatbuffers;

	InterfaceAsset::InterfaceAsset(
		const Utf8Char* const path
	) : Asset( path ),
		_allocator( "Nuklear User Interface Root Allocator" ),
		_widgets( MallocAllocator( "" ) ) {}

// ---------------------------------------------------

	ErrorCode InterfaceAsset::BindResources( const Builder& builder ) {
		const Range<const char*>&	bytes( builder.GetRawBytes() );

	//	Verify the data we are working with can plausibly represent a Nuklear UI.
		Verifier verifier( reinterpret_cast<const uint8_t*>(bytes.Begin()), bytes.GetSize() );
		if (!VerifyNuklearUiBuffer( verifier )) {
			builder.WriteLog( MessageType::Error, "{} is malformed!" UTF8_NEWLINE, GetPath() );
			return Error::InvalidParameter;
		}

		ArrayList<UniquePointer<Widget>> widgets( _widgets.GetAllocator() );

		Swap( _widgets, widgets );

		return Error::None;
	}

// ---------------------------------------------------

	void InterfaceAsset::FreeResources() {
		_widgets.Clear( ReleaseMemorySemantics() );
	}

// ---------------------------------------------------

	void InterfaceAsset::Draw( Interface& nuklear ) const {
		nuklear.DoWindow( "_root", NK_WINDOW_BORDER, { 0, 0, 1024, 768 }, [this] ( Panel& panel ) {
			for (const auto& widget : _widgets) {
				widget->Draw( panel );
			}
		} );
	}

}	// namespace AssetViews
}	// namespace Nuklear
}	// namespace UserInterface
}	// namespace Eldritch2