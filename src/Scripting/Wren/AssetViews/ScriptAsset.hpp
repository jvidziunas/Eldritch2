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
#include <Core/Asset.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Scripting { namespace Wren { namespace AssetViews {

	class ScriptAsset : public AbstractString<char>, public Core::Asset {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		ScriptAsset(const ScriptAsset&) = delete;
		//! Constructs this @ref ScriptAsset instance.
		ScriptAsset(StringSpan path) ETNoexceptHint;

		~ScriptAsset() override = default;

		// ---------------------------------------------------

	public:
		Result BindResources(Logging::Log& log, const Core::AssetBuilder& builder) override;

		void FreeResources() ETNoexceptHint override;

		// ---------------------------------------------------

	public:
		static ETPureFunctionHint StringSpan GetExtension() ETNoexceptHint;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		ScriptAsset& operator=(const ScriptAsset&) = delete;
	};

}}}} // namespace Eldritch2::Scripting::Wren::AssetViews
