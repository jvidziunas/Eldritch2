/*==================================================================*\
  DialogueAsset.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Wren/AssetViews/DialogueSetAsset.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scripting {
namespace Wren {
namespace AssetViews {

	using namespace ::Eldritch2::Assets;

	DialogueSetAsset::DialogueSetAsset( const Utf8Char* const filePath ) : Asset( filePath ) {}

// ---------------------------------------------------

	ETPureFunctionHint Utf8Literal DialogueSetAsset::GetExtension() {
		return ".e2dialogueset";
	}

// ---------------------------------------------------

	void DialogueSetAsset::LoadResponses( DialogueSet& /*database*/ ) const {}

// ---------------------------------------------------

	void DialogueSetAsset::DetachResponses( DialogueSet& /*database*/ ) const {}

// ---------------------------------------------------

	ErrorCode DialogueSetAsset::BindResources( const Builder& /*builder*/ ) {
		return Error::None;
	}

// ---------------------------------------------------

	void DialogueSetAsset::FreeResources() {}

}	// namespace AssetViews
}	// namespace Wren
}	// namespace Scripting
}	// namespace Eldritch2