/*==================================================================*\
  ScriptAsset.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Assets/Asset.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Scripting { namespace Wren { namespace AssetViews {

	class ScriptAsset : public AbstractString<char>, public Assets::Asset {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		ScriptAsset(const ScriptAsset&) = delete;
		//! Constructs this @ref ScriptAsset instance.
		ScriptAsset(StringView path);

		~ScriptAsset() override = default;

		// ---------------------------------------------------

	public:
		ErrorCode BindResources(const Builder& builder) override;

		void FreeResources() override;

		// ---------------------------------------------------

	public:
		static ETPureFunctionHint StringView GetExtension() ETNoexceptHint;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		ScriptAsset& operator=(const ScriptAsset&) = delete;
	};

}}}} // namespace Eldritch2::Scripting::Wren::AssetViews
