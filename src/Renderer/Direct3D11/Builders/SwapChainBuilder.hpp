/*==================================================================*\
  SwapChainBuilder.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/Direct3D11/ForwardDeclarations.hpp>
#include <Utility/MPL/IntTypes.hpp>
#include <Utility/COMPointer.hpp>
//------------------------------------------------------------------//
#include <Windows.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
	class	ErrorCode;
}

namespace Eldritch2 {
namespace Renderer {
namespace Direct3D11 {

	class SwapChainBuilder {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref SwapChainBuilder instance.
		SwapChainBuilder( const Utility::COMPointer<::IDXGIFactory1>& factory = Utility::COMPointer<::IDXGIFactory1>( nullptr ) );

		//!	Destroys this @ref SwapChainBuilder instance.
		~SwapChainBuilder() = default;

	// ---------------------------------------------------

		SwapChainBuilder&	SetTripleBufferingEnabled( bool enabled = true );

		SwapChainBuilder&	SetUserAltEnterAutoHandleEnabled( bool enabled = true );

		SwapChainBuilder&	SetNeedsRenderTargetView( bool enabled = true );

		SwapChainBuilder&	EnableResourceSharing( bool enabled = true );

		SwapChainBuilder&	SetWindow( const ::HWND windowHandle );

		size_t				GetBufferCount();

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

}	// namespace Direct3D11
}	// namespace Renderer
}	// namespace Eldritch2

