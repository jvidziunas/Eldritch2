/*==================================================================*\
  ScriptAsset.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Wren/AssetViews/ScriptAsset.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Scripting { namespace Wren { namespace AssetViews {

	using namespace ::Eldritch2::Logging;
	using namespace ::Eldritch2::Core;

	// ---------------------------------------------------

	ScriptAsset::ScriptAsset(StringSpan path) ETNoexceptHint : AbstractString<char>(MallocAllocator("Wren Script Asset Source Allocator")), Asset(path) {}

	// ---------------------------------------------------

	Result ScriptAsset::BindResources(Log& /*log*/, const AssetBuilder& asset) {
		String source(GetAllocator(), asset.bytes.Begin(), asset.bytes.End());

		Swap(*this, source);

		return Result::Success;
	}

	// ---------------------------------------------------

	void ScriptAsset::FreeResources() ETNoexceptHint {
		Clear();
	}

	// ---------------------------------------------------

	ETPureFunctionHint StringSpan ScriptAsset::GetExtension() ETNoexceptHint {
		return ".wren";
	}

}}}} // namespace Eldritch2::Scripting::Wren::AssetViews
