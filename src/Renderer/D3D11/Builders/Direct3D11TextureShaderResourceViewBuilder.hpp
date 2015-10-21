/*==================================================================*\
  Direct3D11TextureShaderResourceViewBuilder.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/D3D11/D3D11ForwardDeclarations.hpp>
#include <Utility/MPL/IntTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Utility {
		template <class Interface>
		class	COMPointer;
	}
}

namespace Eldritch2 {
namespace Renderer {

	namespace Renderer	= ::Eldritch2::Renderer;
	namespace Utility	= ::Eldritch2::Utility;

// ---------------------------------------------------

	class Direct3D11TextureShaderResourceViewBuilder {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref Direct3D11TextureShaderResourceViewBuilder instance.
		Direct3D11TextureShaderResourceViewBuilder();

		//!	Destroys this @ref Direct3D11TextureShaderResourceViewBuilder instance.
		~Direct3D11TextureShaderResourceViewBuilder() = default;

	// ---------------------------------------------------

		Utility::COMPointer<::ID3D11ShaderResourceView>	Compile( const Utility::COMPointer<::ID3D11Resource>& resource );
		Utility::COMPointer<::ID3D11ShaderResourceView>	Compile( ::ID3D11Resource* const resource );
		Utility::COMPointer<::ID3D11ShaderResourceView>	Compile( const Utility::COMPointer<::ID3D11Texture2D>& resource );
		Utility::COMPointer<::ID3D11ShaderResourceView>	Compile( ::ID3D11Texture2D* const resource );
		Utility::COMPointer<::ID3D11ShaderResourceView>	Compile( const Utility::COMPointer<::ID3D11Texture3D>& resource );
		Utility::COMPointer<::ID3D11ShaderResourceView>	Compile( ::ID3D11Texture3D* const resource );

	// - DATA MEMBERS -------------------------------------

	private:
		::DXGI_FORMAT	_formatOverride;
		::UINT			_initialSlice;
		::UINT			_sizeInSlices;
		::UINT			_firstMipIndex;
		::UINT			_lastMipIndex;

		const char*		_debugName;
	};

}	// namespace Renderer
}	// namespace Eldritch2