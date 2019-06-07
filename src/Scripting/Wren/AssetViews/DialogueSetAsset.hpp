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
#include <Core/Asset.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Scripting { namespace Wren { namespace AssetViews {

	class DialogueSetAsset : public DialogueSet, public Core::Asset {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		DialogueSetAsset(const DialogueSetAsset&) = delete;
		//!	Constructs this @ref DialogueSetAsset instance.
		DialogueSetAsset(StringSpan path) ETNoexceptHint;

		~DialogueSetAsset() override = default;

		// ---------------------------------------------------

	public:
		Result BindResources(Logging::Log& log, const Core::AssetBuilder& builder) override;

		void FreeResources() ETNoexceptHint override;

		// ---------------------------------------------------

	public:
		static ETPureFunctionHint StringSpan GetExtension() ETNoexceptHint;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		DialogueSetAsset& operator=(const DialogueSetAsset&) = delete;
	};

}}}} // namespace Eldritch2::Scripting::Wren::AssetViews
