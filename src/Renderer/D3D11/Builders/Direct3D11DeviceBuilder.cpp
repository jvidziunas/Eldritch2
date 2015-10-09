/*==================================================================*\
  Direct3D11DeviceBuilder.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/D3D11/Builders/Direct3D11DeviceBuilder.hpp>
#include <Utility/ErrorCode.hpp>
#include <D3D11.h>
//------------------------------------------------------------------//

//==================================================================//
// LIBRARIES
//==================================================================//
ET_LINK_LIBRARY( "DXGUID.lib" )
ET_LINK_LIBRARY( "D3D11.lib" )
ET_LINK_LIBRARY( "DXGI.lib" )
//------------------------------------------------------------------//

using namespace ::Eldritch2::Renderer;
using namespace ::Eldritch2::Utility;
using namespace ::Eldritch2;

namespace {

	static ETForceInlineHint ETNoAliasHint void UpdateFlag( ::UINT& flags, const ::UINT flag, bool enabled ) {
		const ::UINT newValue( ( flags | static_cast<::UINT>( enabled ? flag : 0u ) ) & ~( enabled ? flag : 0u ) );
		flags = newValue;
	}

}	// anonymous namespace

namespace Eldritch2 {
namespace Renderer {

	Direct3D11DeviceBuilder::Direct3D11DeviceBuilder() : _adapterID( 0u ), _deviceFlags( 0u ), _maximumFramesToRenderAhead ( 1u ) {}

// ---------------------------------------------------

	Direct3D11DeviceBuilder::~Direct3D11DeviceBuilder() {}

// ---------------------------------------------------

	Direct3D11DeviceBuilder& Direct3D11DeviceBuilder::SetDebuggingEnabled( bool enabled ) {
		UpdateFlag( _deviceFlags, D3D11_CREATE_DEVICE_DEBUG, enabled );
		return *this;
	}

// ---------------------------------------------------

	Direct3D11DeviceBuilder& Direct3D11DeviceBuilder::SetBGRASupportEnabled( bool enabled ) {
		UpdateFlag( _deviceFlags, D3D11_CREATE_DEVICE_BGRA_SUPPORT, enabled );
		return *this;
	}

// ---------------------------------------------------

	Direct3D11DeviceBuilder& Direct3D11DeviceBuilder::SetDesiredAdapterID( ::UINT adapterID ) {
		_adapterID = adapterID;
		return *this;
	}

// ---------------------------------------------------

	Direct3D11DeviceBuilder& Direct3D11DeviceBuilder::SetFreeThreadedModeEnabled( bool enabled ) {
		UpdateFlag( _deviceFlags, D3D11_CREATE_DEVICE_SINGLETHREADED, enabled );
		return *this;
	}

// ---------------------------------------------------

	Direct3D11DeviceBuilder& Direct3D11DeviceBuilder::SetMaximumFramesToRenderAhead( ::UINT frameCap ) {
		_maximumFramesToRenderAhead = frameCap;
		return *this;
	}

// ---------------------------------------------------

	ErrorCode Direct3D11DeviceBuilder::Build() {
		static const ::D3D_FEATURE_LEVEL	attemptedFeatureLevels[] = {
			::D3D_FEATURE_LEVEL_11_0,
		};

		COMPointer<::IDXGIAdapter1>	adapter;
		::HRESULT					nativeResult;
		ErrorCode					result( Errors::NONE );

		if( FAILED( ::CreateDXGIFactory1( __uuidof(::IDXGIFactory1), reinterpret_cast<void**>(_factory.GetInterfacePointer()) ) ) ) {
			result = Errors::UNSPECIFIED;
			goto End;
		}

		// Fancy mechanism at play-- count backwards from the user desired adapter ID and keep it if it actually exists.
		for( ::UINT i( _adapterID ); DXGI_ERROR_NOT_FOUND == _factory->EnumAdapters1( i, adapter.GetInterfacePointer() ); --i );

		if( !adapter ) {
			result = Errors::UNSPECIFIED;
			goto End;
		}

		nativeResult = ::D3D11CreateDevice( adapter.GetUnadornedPointer(),
											::D3D_DRIVER_TYPE_HARDWARE,
											nullptr,
											_deviceFlags,
											attemptedFeatureLevels,
											_countof(attemptedFeatureLevels),
											D3D11_SDK_VERSION,
											_device.GetInterfacePointer(),
											nullptr,
											_immediateContext.GetInterfacePointer() );

		if( FAILED( nativeResult ) ) {
			result = Errors::UNSPECIFIED;
			goto End;
		}

		{ // If the device supports the command, attempt to cap the render-ahead to the previously-specified amount.
			COMPointer<::IDXGIDevice1>	infrastructureDevice;
			_device->QueryInterface( __uuidof(::IDXGIDevice1), reinterpret_cast<void**>(infrastructureDevice.GetInterfacePointer()) );

			if( infrastructureDevice ) {
				infrastructureDevice->SetMaximumFrameLatency( _maximumFramesToRenderAhead );
			}
		}
		
	End:
		if( !result ) {
			_immediateContext.Reset();
			_device.Reset();
			_factory.Reset();
		}

		return result;
	}

// ---------------------------------------------------

	const COMPointer<::ID3D11Device>& Direct3D11DeviceBuilder::GetDevice() const {
		return _device;
	}

// ---------------------------------------------------

	const COMPointer<::IDXGIFactory1>& Direct3D11DeviceBuilder::GetFactory() const {
		return _factory;
	}

// ---------------------------------------------------

	const COMPointer<::ID3D11DeviceContext>& Direct3D11DeviceBuilder::GetImmediateContext() const {
		return _immediateContext;
	}

}	// namespace Renderer
}	// namespace Eldritch2