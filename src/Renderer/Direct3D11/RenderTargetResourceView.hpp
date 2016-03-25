/*==================================================================*\
  RenderTargetResourceView.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/Direct3D11/ImageResourceView.hpp>
#include <Utility/COMPointer.hpp>
//------------------------------------------------------------------//
#if( ET_COMPILER_IS_MSVC )
//	MSVC complains about macro redefinitions, since a few DirectX components separately
//	define some HRESULT values without an include guard. The definitions themselves are consistent,
//	so just disable the warning.
#	pragma warning( push )
#	pragma warning( disable : 4005 )
#endif
#include <D3D11.h>
#if( ET_COMPILER_IS_MSVC )
#	pragma warning( pop )
#endif
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Renderer {
namespace Direct3D11 {

	class RenderTargetResourceView : public Direct3D11::ImageResourceView {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref RenderTargetResourceView instance.
		RenderTargetResourceView( const ::Eldritch2::UTF8Char* const name );

		~RenderTargetResourceView() = default;

	// ---------------------------------------------------

		::Eldritch2::ErrorCode	AttachToPackage( const ::Eldritch2::Range<const char*> bytes, FileSystem::ContentPackage& package, FileSystem::ContentLibrary& library ) override;

	// - DATA MEMBERS ------------------------------------

	private:
		Utility::COMPointer<::ID3D11RenderTargetView>	_renderTargetView;
	};

}	// namespace Direct3D11
}	// namespace Renderer
}	// namespace Eldritch2