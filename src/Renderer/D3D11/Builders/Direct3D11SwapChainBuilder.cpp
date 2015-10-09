/*==================================================================*\
  Direct3D11SwapChainBuilder.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/D3D11/Builders/Direct3D11SwapChainBuilder.hpp>
#include <Foundation/Window.hpp>
#include <Utility/ErrorCode.hpp>
#include <Utility/Assert.hpp>
#ifndef D3D11_NO_HELPERS
#	define D3D11_NO_HELPERS
#endif
#ifndef WIN32_LEAN_AND_MEAN
#	define WIN32_LEAN_AND_MEAN
#endif
#include <D3D11.h>
#include <DXGI.h>
//------------------------------------------------------------------//

//==================================================================//
// LIBRARIES
//==================================================================//
ET_LINK_LIBRARY( "D3D11.lib" )
ET_LINK_LIBRARY( "DXGI.lib" )
//------------------------------------------------------------------//

using namespace ::Eldritch2::Foundation;
using namespace ::Eldritch2::Renderer;
using namespace ::Eldritch2::Utility;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Renderer {

	Direct3D11SwapChainBuilder::Direct3D11SwapChainBuilder( const COMPointer<::IDXGIFactory1>& factory ) : _debugName( nullptr ), _bufferCount( 2u ), _usageFlags( DXGI_USAGE_BACK_BUFFER ), _associationFlags( 0u ), _factory( factory ) {
		if( !factory ) {
			ETRuntimeVerification( SUCCEEDED( ::CreateDXGIFactory1( __uuidof(::IDXGIFactory1), reinterpret_cast<void**>(&_factory) ) ) );
		}
	}

// ---------------------------------------------------

	Direct3D11SwapChainBuilder::~Direct3D11SwapChainBuilder() {}

// ---------------------------------------------------

	Direct3D11SwapChainBuilder& Direct3D11SwapChainBuilder::SetTripleBufferingEnabled( bool enabled ) {
		_bufferCount = enabled ? 2u : 1u;
		return( *this );
	}

// ---------------------------------------------------

	size_t Direct3D11SwapChainBuilder::GetBufferCount() {
		return _bufferCount;
	}

// ---------------------------------------------------

	Direct3D11SwapChainBuilder& Direct3D11SwapChainBuilder::SetNeedsRenderTargetView( bool enabled ) {
		_usageFlags |= enabled ? DXGI_USAGE_RENDER_TARGET_OUTPUT : 0u;
		_usageFlags &= ~(enabled ? 0u : DXGI_USAGE_RENDER_TARGET_OUTPUT);

		return( *this );
	}

// ---------------------------------------------------

	Direct3D11SwapChainBuilder& Direct3D11SwapChainBuilder::SetUserAltEnterAutoHandleEnabled( bool enabled ) {
		_associationFlags |= enabled ? DXGI_MWA_NO_ALT_ENTER : 0u;
		_associationFlags &= ~(enabled ? 0u : DXGI_MWA_NO_ALT_ENTER);

		return( *this );
	}

// ---------------------------------------------------

	Direct3D11SwapChainBuilder& Direct3D11SwapChainBuilder::EnableResourceSharing( bool enabled ) {
		_usageFlags |= enabled ? DXGI_USAGE_SHARED : 0u;
		_usageFlags &= ~(enabled ? 0u : DXGI_USAGE_SHARED);

		return( *this );
	}

// ---------------------------------------------------

	Direct3D11SwapChainBuilder& Direct3D11SwapChainBuilder::SetWindow( const ::HWND handle ) {
		_windowHandle = handle;

		return *this;
	}

// ---------------------------------------------------

	COMPointer<IDXGIFactory1> Direct3D11SwapChainBuilder::GetFactory() const {
		return _factory;
	}

// ---------------------------------------------------

	COMPointer<IDXGISwapChain> Direct3D11SwapChainBuilder::GetSwapChain() const {
		return _swapChain;
	}

// ---------------------------------------------------

	ErrorCode Direct3D11SwapChainBuilder::Build( ::ID3D11Device* const device ) {
		::IDXGISwapChain*		tempChain;
		::DXGI_SWAP_CHAIN_DESC	desc = {
			{
				_widthInPixels,
				_heightInPixels,
				{ 0, 0 },
				(_usageFlags & DXGI_USAGE_SHARED) ? ::DXGI_FORMAT_R8G8B8A8_UNORM : ::DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
				::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED,
				::DXGI_MODE_SCALING_UNSPECIFIED
			},
			{ 1u, 0u },
			_usageFlags,
			_bufferCount,
			_windowHandle,
			TRUE,
			::DXGI_SWAP_EFFECT_DISCARD,
			::DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH
		};

		if( FAILED( _factory->CreateSwapChain( device, &desc, &tempChain ) ) ) {
			return Errors::UNSPECIFIED;
		}

		_swapChain.Acquire( tempChain );

		if( FAILED( _factory->MakeWindowAssociation( _windowHandle, _associationFlags ) ) ) {
			return Errors::UNSPECIFIED;
		}

		return Errors::NONE;
	}

// ---------------------------------------------------

#if 0
	void D3D11SwapChainBuilder::SetFullscreen( uint32 widthInPixels, uint32 heightInPixels, ::IDXGIOutput* output ) {
		::DXGI_MODE_DESC	desc = {
			widthInPixels,
			heightInPixels,
			{ 0, 0 },
			::DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
			DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED,
			DXGI_MODE_SCALING_UNSPECIFIED
		};

		_swapChain->ResizeTarget( &desc );
		_swapChain->SetFullscreenState( TRUE, output );
		::SetForegroundWindow( static_cast<::HWND>(_window) );
	}
#endif

}	// namespace Renderer
}	// namespace Eldritch2