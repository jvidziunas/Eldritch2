/*==================================================================*\
  DialogueAsset.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Assets/Asset.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Scripting { namespace Wren {
	class DialogueSet;
}}} // namespace Eldritch2::Scripting::Wren

namespace Eldritch2 { namespace Scripting { namespace Wren { namespace AssetViews {

	class DialogueSetAsset : public Assets::Asset {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref DialogueAsset instance.
		DialogueSetAsset(const Utf8Char* const filePath);
		//!	Disable copy construction.
		DialogueSetAsset(const DialogueSetAsset&) = delete;

		~DialogueSetAsset() override = default;

		// ---------------------------------------------------

	public:
		static ETPureFunctionHint Utf8Literal GetExtension();

		// ---------------------------------------------------

	public:
		ErrorCode BindResources(const Builder& builder) override;

		void FreeResources() override;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		DialogueSetAsset& operator=(const DialogueSetAsset&) = delete;
	};

}}}} // namespace Eldritch2::Scripting::Wren::AssetViews
