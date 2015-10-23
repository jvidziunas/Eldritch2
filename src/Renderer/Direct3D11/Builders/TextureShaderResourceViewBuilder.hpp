/*==================================================================*\
  TextureShaderResourceViewBuilder.hpp
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
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Utility {
		template <class Interface>
		class	COMPointer;
	}
}

namespace Eldritch2 {
namespace Renderer {
namespace Direct3D11 {

	class TextureShaderResourceViewBuilder {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref TextureShaderResourceViewBuilder instance.
		TextureShaderResourceViewBuilder();

		//!	Destroys this @ref TextureShaderResourceViewBuilder instance.
		~TextureShaderResourceViewBuilder() = default;

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

}	// namespace Direct3D11
}	// namespace Renderer
}	// namespace Eldritch2