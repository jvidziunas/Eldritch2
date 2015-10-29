/*==================================================================*\
  SwapChain.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/ScriptAPIRegistrationInitializationVisitor.hpp>
#include <Renderer/Direct3D11/Builders/SwapChainBuilder.hpp>
#include <Renderer/Direct3D11/SwapChain.hpp>
#include <Utility/Win32ApplicationHelpers.hpp>
#include <Utility/Memory/ArenaAllocator.hpp>
#include <utfcpp/v2_0/source/utf8/checked.h>
#include <Scripting/ScriptMarshalTypes.hpp>
#include <Resources/Eldritch2.Resources.h>
#include <Scripting/ObjectHandle.hpp>
#include <Utility/ErrorCode.hpp>
//------------------------------------------------------------------//
#include <D3D11.h>
//------------------------------------------------------------------//

//==================================================================//
// LIBRARIES
//==================================================================//
ET_LINK_LIBRARY( "DXGI.lib" )
//------------------------------------------------------------------//

using namespace ::Eldritch2::Foundation;
using namespace ::Eldritch2::Scheduler;
using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Renderer;
using namespace ::Eldritch2::Utility;
using namespace ::Eldritch2;
using namespace ::std;

namespace {

	static ETNoAliasHint ETInlineHint ::DWORD WindowStyleFromPresentationMode( const Window::PresentationParameters::Mode presentationMode ) {
		using PresentationMode = Window::PresentationParameters::Mode;

	// ---

		switch( presentationMode ) {
			case PresentationMode::WINDOWED: {
				return WS_OVERLAPPEDWINDOW;
			}	// case PresentationMode::WINDOWED

		// ---

			case PresentationMode::WINDOWED_FULLSCREEN: {
				return WS_MAXIMIZE;
			}	// case PresentationMode::WINDOWED_FULLSCREEN

		// ---

			case PresentationMode::FULLSCREEN: {
				return WS_MAXIMIZE;
			}	// case PresentationMode::FULLSCREEN
		};

		return 0;
	}

}	// anonymous namespace

namespace Eldritch2 {
namespace Renderer {
namespace Direct3D11 {

	const char* const SwapChain::scriptTypeName = "SwapChain";

// ---------------------------------------------------

	const UTF8Char* const SwapChain::GetHumanReadableName() const {
		return UTF8L("Direct3D11 Swap Chain Window Message Pump Thread");
	}

// ---------------------------------------------------

	ErrorCode SwapChain::Run() {
		enum : ::DWORD {
			NO_EXTENDED_STYLES = static_cast<::DWORD>(0)
		};

	// ---

		ObjectHandle<SwapChain>	thisReference( this );
		const auto				windowHandle( ::CreateWindowEx( NO_EXTENDED_STYLES,
																nullptr,
																nullptr,
																WindowStyleFromPresentationMode( _presentationParameters.presentationMode ),
																CW_USEDEFAULT,
																CW_USEDEFAULT,
																static_cast<int>(_presentationParameters.widthInPixels),
																static_cast<int>(_presentationParameters.heightInPixels),
																nullptr,
																nullptr,
																GetInstance(),
																this ) );

		if( nullptr == windowHandle ) {
			return Error::UNSPECIFIED;
		}

		::ShowWindow( windowHandle, GetCmdShow() );

		for( ::MSG receivedMessage; 0 != ::GetMessage( &receivedMessage, windowHandle, static_cast<::UINT>(0), static_cast<::UINT>(0xFFFFFFFF) ); ) {
			::TranslateMessage( &receivedMessage );
			::DispatchMessage( &receivedMessage );
		}

		return Error::NONE;
	}

// ---------------------------------------------------

	void SwapChain::RequestGracefulShutdown() {
		if( const auto windowHandle = _windowHandle.load( memory_order_acquire ) ) {
			::PostMessage( windowHandle, WM_CLOSE, 0, 0 );
		}
	}

// ---------------------------------------------------

	SwapChain::SwapChain( const PresentationParameters& presentationParameters ) : _presentationParameters( presentationParameters ), _windowHandle( nullptr ), _drawingCounter( 0u ), _displayedFrameCounter( 0u ) {}

// ---------------------------------------------------

	SwapChain::~SwapChain() {}

// ---------------------------------------------------

	void SwapChain::ExposeScriptAPI( ScriptAPIRegistrationInitializationVisitor& typeRegistrar ) {
		struct FunctionHelper {
			static void ETScriptAPICall SetCaption( SwapChain* thisPointer, const StringMarshal& newCaption ) {
				thisPointer->SetCaption( newCaption.GetCharacterArray() );
			}
		};

	// ---

		FixedStackAllocator<16u>	temporaryAllocator( UTF8L("SwapChain::ExposeScriptAPI() Temporary Allocator") );

		if( const auto registerResult = typeRegistrar.RegisterUserDefinedReferenceType<SwapChain>( temporaryAllocator ) ) {
			auto&	typeBuilder( *registerResult.object );

			typeRegistrar.EnsureValueTypeDeclared<StringMarshal>();

			typeBuilder.ExposeVirtualProperty( "Caption", &FunctionHelper::SetCaption );

			temporaryAllocator.Delete( typeBuilder );
		}
	}

// ---------------------------------------------------

	void SwapChain::Dispose() {}

// ---------------------------------------------------

	Window::PresentationParameters SwapChain::GetPresentationParameters() const {
		return _presentationParameters;
	}

// ---------------------------------------------------

	void SwapChain::SetCaption( const UTF8Char* const captionString ) {
#	if( ETIsBuildUnicode() )
		const UTF8Char* const	endOfString( FindEndOfString( captionString ) );
		wchar_t* const			newText( static_cast<wchar_t*>(_alloca( sizeof(wchar_t) * (endOfString - captionString) )) );

		::SetWindowTextW( _windowHandle, (::utf8::utf8to16( captionString, endOfString, newText ), newText) );
#	else
		::SetWindowTextA( _windowHandle, captionString );
#	endif
	}

// ---------------------------------------------------

	COMPointer<::ID3D11RenderTargetView> SwapChain::GetRenderTargetViewAndIncrementPresentCounter() {
		::ID3D11RenderTargetView*	view( nullptr );

		_drawingCounter.fetch_add( 1u, memory_order_acquire );

		return { view, ::Eldritch2::PassthroughReferenceCountingSemantics };
	}

// ---------------------------------------------------

	void SwapChain::DecrementPresentCounter() {
		if( 1u == _drawingCounter.fetch_sub( 1u, memory_order_release ) ) {
			_swapChain->Present( 1u, 0u );
		}
	}

// ---------------------------------------------------

	::ATOM SwapChain::RegisterWindowClass() {
		const ::HINSTANCE	applicationInstance( ::GetModuleHandle( nullptr ) );
		const ::WNDCLASSEX	windowClass = {
			static_cast<::DWORD>(sizeof( windowClass )),
			static_cast<::UINT>(0),
			[] ( ::HWND windowHandle, ::UINT messageID, ::WPARAM wParam, ::LPARAM lParam ) -> ::LRESULT {
				if( ETBranchLikelyHint( WM_NCCREATE == messageID ) ) {
					auto	swapChain( static_cast<SwapChain*>(reinterpret_cast<::LPCREATESTRUCT>(lParam)->lpCreateParams) );

					swapChain->_windowHandle.store( windowHandle, memory_order_release );
					::SetWindowLongPtr( windowHandle, GWLP_USERDATA, reinterpret_cast<::LONG_PTR>(swapChain) );
				}

				if( auto swapChain = reinterpret_cast<SwapChain*>(::GetWindowLongPtr( windowHandle, GWLP_USERDATA )) ) {
					// We've successfully attached, skip the branch for all subsequent messages
					::SetWindowLongPtr( windowHandle, GWLP_WNDPROC, reinterpret_cast<::LONG_PTR>(static_cast<::WNDPROC>([] ( ::HWND windowHandle, ::UINT messageID, ::WPARAM wParam, ::LPARAM lParam ) -> ::LRESULT {
						return reinterpret_cast<SwapChain*>(::GetWindowLongPtr( windowHandle, GWLP_USERDATA ))->MessageProc( messageID, wParam, lParam );
					})) );
					return swapChain->MessageProc( messageID, wParam, lParam );
				}

				return ::DefWindowProc( windowHandle, messageID, wParam, lParam );
			},
			0,
			0,
			GetModuleHandle( nullptr ),
			static_cast<::HICON>(::LoadImage( applicationInstance, MAKEINTRESOURCE( IDI_ICON1 ), IMAGE_ICON, ::GetSystemMetrics( SM_CXICON ), ::GetSystemMetrics( SM_CYICON ), LR_DEFAULTSIZE )),
			nullptr,
			nullptr,
			nullptr,
			SL("Eldritch2Window"),
			static_cast<::HICON>(::LoadImage( applicationInstance, MAKEINTRESOURCE( IDI_ICON1 ), IMAGE_ICON, ::GetSystemMetrics( SM_CXSMICON ), ::GetSystemMetrics( SM_CYSMICON ), LR_DEFAULTSIZE ))
		};

		return ::RegisterClassEx( &windowClass );
	}

// ---------------------------------------------------

	::LRESULT SwapChain::MessageProc( ::UINT messageID, ::WPARAM wParam, ::LPARAM lParam ) {
		switch( messageID ) {
			case WM_SIZE: {
				_presentationParameters.widthInPixels	= static_cast<uint32>(LOWORD( lParam ));
				_presentationParameters.heightInPixels	= static_cast<uint32>(HIWORD( lParam ));

				return 0;
			} // case WM_SIZE

		// ---

			case WM_CLOSE: {
				::DestroyWindow( _windowHandle );

				return 0;
			}	// case WM_CLOSE

		// ---
	
			case WM_DESTROY: {
				_windowHandle.store( nullptr, memory_order_release );
				
				::PostQuitMessage( 0 );
			}	// case WM_DESTROY

		// ---

			default: {
				return ::DefWindowProc( _windowHandle, messageID, wParam, lParam );
			}
		}	// switch( msgID )
	}

}	// namespace Direct3D11
}	// namespace Renderer
}	// namespace Eldritch2