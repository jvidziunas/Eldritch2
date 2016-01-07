/*==================================================================*\
  DeviceBuilder.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/Direct3D11/Builders/DeviceBuilder.hpp>
#include <Utility/Memory/StandardLibrary.hpp>
#include <Utility/ErrorCode.hpp>
//------------------------------------------------------------------//
#if( ET_COMPILER_IS_MSVC )
//	MSVC complains about macro redefinitions, since a few DirectX components separately
// define some HRESULT values without an include guard. The definitions themselves are consistent,
// so just disable the warning.
#	pragma warning( push )
#	pragma warning( disable : 4005 )
#endif
#include <D3D11.h>
#if( ET_COMPILER_IS_MSVC )
#	pragma warning( pop )
#endif
//------------------------------------------------------------------//
#include <utility>
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
using namespace ::std;

namespace {

	static ETForceInlineHint ETNoAliasHint void UpdateFlag( ::UINT& flags, const ::UINT flag, bool enabled ) {
		const ::UINT newValue( ( flags | static_cast<::UINT>( enabled ? flag : 0u ) ) & ~( enabled ? flag : 0u ) );
		flags = newValue;
	}

}	// anonymous namespace

namespace Eldritch2 {
namespace Renderer {
namespace Direct3D11 {

	DeviceBuilder::DeviceBuilder( const COMPointer<::IDXGIFactory1>& factory ) : _factory( factory ), _adapterName( nullptr ), _deviceFlags( ::D3D11_CREATE_DEVICE_SINGLETHREADED ), _maximumFramesToRenderAhead( 1u ) {
		if( !_factory ) {
			::CreateDXGIFactory1( __uuidof(::IDXGIFactory1), reinterpret_cast<void**>(_factory.GetInterfacePointer()) );
		}
	}

// ---------------------------------------------------

	DeviceBuilder& DeviceBuilder::SetDriverThreadingOptimizationsEnabled( bool enabled ) {
		UpdateFlag( _deviceFlags, ::D3D11_CREATE_DEVICE_PREVENT_INTERNAL_THREADING_OPTIMIZATIONS, !enabled );
		return *this;
	}

// ---------------------------------------------------

	DeviceBuilder& DeviceBuilder::SetDebuggingEnabled( bool enabled ) {
		UpdateFlag( _deviceFlags, ::D3D11_CREATE_DEVICE_DEBUG, enabled );
		return *this;
	}

// ---------------------------------------------------

	DeviceBuilder& DeviceBuilder::SetBGRASupportEnabled( bool enabled ) {
		UpdateFlag( _deviceFlags, ::D3D11_CREATE_DEVICE_BGRA_SUPPORT, enabled );
		return *this;
	}

// ---------------------------------------------------

	DeviceBuilder& DeviceBuilder::SetDesiredAdapterName( const UTF8Char* const adapterName ) {
		_adapterName = adapterName;
		return *this;
	}

// ---------------------------------------------------

	DeviceBuilder& DeviceBuilder::SetFreeThreadedModeEnabled( bool enabled ) {
		UpdateFlag( _deviceFlags, ::D3D11_CREATE_DEVICE_SINGLETHREADED, enabled );
		return *this;
	}

// ---------------------------------------------------

	DeviceBuilder& DeviceBuilder::SetMaximumFramesToRenderAhead( const ::UINT frameCap ) {
		_maximumFramesToRenderAhead = frameCap;
		return *this;
	}

// ---------------------------------------------------

	COMPointer<::ID3D11Device> DeviceBuilder::Build() {
		static const ::D3D_FEATURE_LEVEL	featureLevels[] = {
			::D3D_FEATURE_LEVEL_11_0,
		};

		COMPointer<::IDXGIAdapter1>	adapter;
		COMPointer<::ID3D11Device>	device;

		if( (nullptr != _adapterName) && !EqualityCompareString( _adapterName, UTF8L("") ) ) {
			::DXGI_ADAPTER_DESC1		adapterDescription;
			wchar_t						wideAdapterName[128u];

			for( ::UINT adapterID( 0u ); ; ++adapterID ) {
				COMPointer<::IDXGIAdapter1>	temporaryAdapter;

				if( DXGI_ERROR_NOT_FOUND == _factory->EnumAdapters1( adapterID, temporaryAdapter.GetInterfacePointer() ) ) {
					break;
				}

				if( SUCCEEDED( adapter->GetDesc1( &adapterDescription ) ) && EqualityCompareString( adapterDescription.Description, wideAdapterName ) ) {
					adapter = move( temporaryAdapter );
					break;
				}
			}
		}

		if( SUCCEEDED( ::D3D11CreateDevice( adapter.GetUnadornedPointer(), ::D3D_DRIVER_TYPE_HARDWARE, nullptr, _deviceFlags, featureLevels, _countof(featureLevels), D3D11_SDK_VERSION, device.GetInterfacePointer(), nullptr, nullptr ) ) ) {
			COMPointer<::IDXGIDevice1>	infrastructureDevice;

			// If the device supports the command, attempt to cap the render-ahead to the previously-specified amount.
			device->QueryInterface( __uuidof(::IDXGIDevice1), reinterpret_cast<void**>(infrastructureDevice.GetInterfacePointer()) );

			if( infrastructureDevice ) {
				infrastructureDevice->SetMaximumFrameLatency( _maximumFramesToRenderAhead );
			}
		}

		return ::std::move( device );
	}

}	// namespace Direct3D11
}	// namespace Renderer
}	// namespace Eldritch2