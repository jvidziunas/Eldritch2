/*==================================================================*\
  DeviceBuilder.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/Noncopyable.hpp>
#include <Utility/MPL/CharTypes.hpp>
#include <Utility/COMPointer.hpp>
//------------------------------------------------------------------//
#include <D3D11.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
	class	ErrorCode;
}

using UINT	= unsigned int;

namespace Eldritch2 {
namespace Renderer {
namespace Direct3D11 {

	class DeviceBuilder : public Utility::Noncopyable {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref DeviceBuilder instance.
		DeviceBuilder( const Utility::COMPointer<::IDXGIFactory1>& factory = nullptr );

		//!	Constructs this @ref DeviceBuilder instance.
		~DeviceBuilder() = default;

	// ---------------------------------------------------

		DeviceBuilder&	SetDriverThreadingOptimizationsEnabled( bool enabled = true );

		DeviceBuilder&	SetDebuggingEnabled( bool enabled = true );

		DeviceBuilder&	SetBGRASupportEnabled( bool enabled = true );

		DeviceBuilder&	SetDesiredAdapterName( const ::Eldritch2::UTF8Char* const adapterName );

		DeviceBuilder&	SetFreeThreadedModeEnabled( bool enabled = true );

		DeviceBuilder&	SetMaximumFramesToRenderAhead( const ::UINT frameCap );

	// ---------------------------------------------------

		Utility::COMPointer<::ID3D11Device>	Build();

	// - DATA MEMBERS ------------------------------------

	private:
		Utility::COMPointer<::IDXGIFactory1>	_factory;

		const ::Eldritch2::UTF8Char*			_adapterName;
		::UINT									_deviceFlags;
		::UINT									_maximumFramesToRenderAhead;
	};

}	// namespace Direct3D11
}	// namespace Renderer
}	// namespace Eldritch2
