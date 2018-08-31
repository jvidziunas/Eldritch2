/*==================================================================*\
  ScriptAsset.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Wren/AssetViews/ScriptAsset.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Scripting { namespace Wren { namespace AssetViews {

	using namespace ::Eldritch2::Assets;

	ScriptAsset::ScriptAsset(StringView path) :
		AbstractString<char>(MallocAllocator("Wren Script Asset Source Allocator")),
		Asset(path) {
	}

	// ---------------------------------------------------

	ErrorCode ScriptAsset::BindResources(const Builder& asset) {
		if (asset.GetSize() == 0u) {
			return Error::InvalidParameter;
		}

		String source(GetAllocator(), asset.Begin(), asset.End());

		Swap(*this, source);

		return Error::None;
	}

	// ---------------------------------------------------

	void ScriptAsset::FreeResources() {
		Clear();
	}

	// ---------------------------------------------------

	ETPureFunctionHint StringView ScriptAsset::GetExtension() ETNoexceptHint {
		return ".wren";
	}

}}}} // namespace Eldritch2::Scripting::Wren::AssetViews
