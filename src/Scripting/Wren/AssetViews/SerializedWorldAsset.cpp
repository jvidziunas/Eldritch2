/*==================================================================*\
  SerializedWorldAsset.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2019 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Wren/AssetViews/SerializedWorldAsset.hpp>
//------------------------------------------------------------------//
#include <Flatbuffers/SerializedWorld_generated.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Scripting { namespace Wren { namespace AssetViews {

	using namespace ::Eldritch2::Scripting::Wren::FlatBuffers;
	using namespace ::Eldritch2::Logging;
	using namespace ::Eldritch2::Core;
	using namespace ::flatbuffers;

	// ---------------------------------------------------

	SerializedWorldAsset::SerializedWorldAsset(StringSpan path) ETNoexceptHint : Asset(path) {}

	// ---------------------------------------------------

	Result SerializedWorldAsset::BindResources(Log& log, const AssetBuilder& builder) {
		return Result::Success;
	}

	// ---------------------------------------------------

	void SerializedWorldAsset::FreeResources() ETNoexceptHint {}

	// ---------------------------------------------------

	ETPureFunctionHint StringSpan SerializedWorldAsset::GetExtension() ETNoexceptHint {
		return StringSpan(WorldDescriptorExtension(), StringLength(WorldDescriptorExtension()));
	}

}}}} // namespace Eldritch2::Scripting::Wren::AssetViews
