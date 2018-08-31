/*==================================================================*\
  DialogueSetAsset.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Wren/DialogueSet.hpp>
#include <Assets/Asset.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Scripting { namespace Wren { namespace AssetViews {

	class DialogueSetAsset : public Wren::DialogueSet, public Assets::Asset {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		DialogueSetAsset(const DialogueSetAsset&) = delete;
		//!	Constructs this @ref DialogueAsset instance.
		DialogueSetAsset(StringView path);

		~DialogueSetAsset() override = default;

		// ---------------------------------------------------

	public:
		ErrorCode BindResources(const Builder& builder) override;

		void FreeResources() override;

		// ---------------------------------------------------

	public:
		static ETPureFunctionHint StringView GetExtension();

		// ---------------------------------------------------

		//!	Disable copy assignment.
		DialogueSetAsset& operator=(const DialogueSetAsset&) = delete;
	};

}}}} // namespace Eldritch2::Scripting::Wren::AssetViews
