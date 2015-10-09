/*==================================================================*\
  Eldritch2ShaderCompilerInclude.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Memory/Allocator.hpp>
#include <Utility/MPL/CharTypes.hpp>
#include <D3DCommon.h>
#include <string>
#include <map>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Tools {
		class	FileReadStrategyFactory;
	}
}

namespace Eldritch2 {
namespace Renderer {

	namespace Renderer	= ::Eldritch2::Renderer;

// ---------------------------------------------------

	class Eldritch2ShaderCompilerInclude : public ::ID3DInclude {
	public:
		typedef ::std::basic_string<::Eldritch2::SystemChar>	SystemString;

	// ---------------------------------------------------

		// Constructs this Eldritch2ShaderCompilerInclude instance.
		Eldritch2ShaderCompilerInclude( const ::Eldritch2::SystemChar* rootFile, Tools::FileReadStrategyFactory& fileStrategyFactory );

		// Destroys this Eldritch2ShaderCompilerInclude instance.
		~Eldritch2ShaderCompilerInclude();

	// ---------------------------------------------------

		::HRESULT	STDMETHODCALLTYPE	Open( ::D3D_INCLUDE_TYPE type, ::LPCSTR fileName, ::LPCVOID parentData, ::LPCVOID* data, ::UINT* dataSize ) override sealed;

		::HRESULT	STDMETHODCALLTYPE	Close( ::LPCVOID pData ) override sealed;

	// ---------------------------------------------------

	private:
		SystemString	GetFilePath( ::LPCSTR fileName, ::LPCVOID parentData );
		SystemString	GetFilePath( ::LPCSTR fileName );

	// ---------------------------------------------------

		Tools::FileReadStrategyFactory&		_fileStrategyFactory;
		::std::map<::LPCVOID, SystemString>	_directoryTable;
	};

}	// namespace Renderer
}	// namespace Eldritch2