/*==================================================================*\
  SerializedWorldAsset.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2019 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Core/Asset.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Scripting { namespace Wren { namespace AssetViews {

	class SerializedWorldAsset : public Core::Asset {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		SerializedWorldAsset(const SerializedWorldAsset&) = delete;
		//! Constructs this @ref SerializedWorldAsset instance.
		SerializedWorldAsset(StringSpan path) ETNoexceptHint;

		~SerializedWorldAsset() override = default;

		// ---------------------------------------------------

	public:
		Result BindResources(Logging::Log& log, const Core::AssetBuilder& builder) override;

		void FreeResources() ETNoexceptHint override;

		// ---------------------------------------------------

	public:
		static ETPureFunctionHint StringSpan GetExtension() ETNoexceptHint;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		SerializedWorldAsset& operator=(const SerializedWorldAsset&) = delete;
	};

}}}} // namespace Eldritch2::Scripting::Wren::AssetViews
