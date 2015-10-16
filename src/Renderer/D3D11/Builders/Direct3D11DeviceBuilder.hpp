/*==================================================================*\
  Direct3D11DeviceBuilder.hpp
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

typedef unsigned int UINT;

namespace Eldritch2 {
namespace Renderer {

	namespace Renderer	= ::Eldritch2::Renderer;
	namespace Utility	= ::Eldritch2::Utility;

// ---------------------------------------------------

	class Direct3D11DeviceBuilder : public Utility::Noncopyable {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref Direct3D11DeviceBuilder instance.
		Direct3D11DeviceBuilder();

		//!	Constructs this @ref Direct3D11DeviceBuilder instance.
		~Direct3D11DeviceBuilder() = default;

	// ---------------------------------------------------

		Renderer::Direct3D11DeviceBuilder&	SetDriverThreadingOptimizationsEnabled( bool enabled = true );

		Renderer::Direct3D11DeviceBuilder&	SetDebuggingEnabled( bool enabled = true );

		Renderer::Direct3D11DeviceBuilder&	SetBGRASupportEnabled( bool enabled = true );

		Renderer::Direct3D11DeviceBuilder&	SetDesiredAdapterName( const ::Eldritch2::UTF8Char* const adapterName );

		Renderer::Direct3D11DeviceBuilder&	SetFreeThreadedModeEnabled( bool enabled = true );

		Renderer::Direct3D11DeviceBuilder&	SetMaximumFramesToRenderAhead( ::UINT frameCap );

	// ---------------------------------------------------

		::Eldritch2::ErrorCode	Build();

	// ---------------------------------------------------

		const Utility::COMPointer<::ID3D11Device>&			GetDevice() const;

		const Utility::COMPointer<::IDXGIFactory1>&			GetFactory() const;

		const Utility::COMPointer<::ID3D11DeviceContext>&	GetImmediateContext() const;

	// - DATA MEMBERS ------------------------------------

	private:
		const ::Eldritch2::UTF8Char*				_adapterName;
		::UINT										_deviceFlags;
		::UINT										_maximumFramesToRenderAhead;
		Utility::COMPointer<::ID3D11Device>			_device;
		Utility::COMPointer<::IDXGIFactory1>		_factory;
		Utility::COMPointer<::ID3D11DeviceContext>	_immediateContext;
	};

}	// namespace Renderer
}	// namespace Eldritch2
