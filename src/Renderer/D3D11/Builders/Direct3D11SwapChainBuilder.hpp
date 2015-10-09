/*==================================================================*\
  Direct3D11SwapChainBuilder.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/D3D11/D3D11ForwardDeclarations.hpp>
#include <Utility/MPL/IntTypes.hpp>
#include <Utility/COMPointer.hpp>
//------------------------------------------------------------------//
#include <Windows.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
	class ErrorCode;
}

namespace Eldritch2 {
namespace Renderer {

	namespace Renderer	= ::Eldritch2::Renderer;
	namespace Utility	= ::Eldritch2::Utility;

// ---------------------------------------------------

	class Direct3D11SwapChainBuilder {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		// Constructs this Direct3D11SwapChainBuilder instance.
		Direct3D11SwapChainBuilder( const Utility::COMPointer<::IDXGIFactory1>& factory = nullptr );

		// Destroys this Direct3D11SwapChainBuilder instance.
		~Direct3D11SwapChainBuilder();

	// ---------------------------------------------------

		Renderer::Direct3D11SwapChainBuilder&	SetTripleBufferingEnabled( bool enabled = true );

		Renderer::Direct3D11SwapChainBuilder&	SetUserAltEnterAutoHandleEnabled( bool enabled = true );

		Renderer::Direct3D11SwapChainBuilder&	SetNeedsRenderTargetView( bool enabled = true );

		Renderer::Direct3D11SwapChainBuilder&	EnableResourceSharing( bool enabled = true );

		Renderer::Direct3D11SwapChainBuilder&	SetWindow( const ::HWND windowHandle );

		size_t									GetBufferCount();

	// ---------------------------------------------------

		::Eldritch2::ErrorCode	Build( ::ID3D11Device* const device );

	// ---------------------------------------------------

		Utility::COMPointer<::IDXGISwapChain>	GetSwapChain() const;

		Utility::COMPointer<::IDXGIFactory1>	GetFactory() const;

	// ---------------------------------------------------
	
	private:
		Utility::COMPointer<::IDXGIFactory1>	_factory;
		Utility::COMPointer<::IDXGISwapChain>	_swapChain;
		::HWND									_windowHandle;
		const char*								_debugName;
		::UINT									_widthInPixels;
		::UINT									_heightInPixels;
		::UINT									_bufferCount;
		::UINT									_usageFlags;
		::UINT									_associationFlags;
	};

}	// namespace Renderer
}	// namespace Eldritch2

