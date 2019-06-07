/*==================================================================*\
  ArmatureDefinitionAsset.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Animation/AnimationApi.hpp>
#include <Core/Asset.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Physics { namespace PhysX { namespace AssetViews {

	class ArmatureDefinitionAsset : public Animation::ArmatureDefinition, public Core::Asset {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		ArmatureDefinitionAsset(const ArmatureDefinitionAsset&) = delete;
		//!	Constructs this @ref ArmatureDefinitionAsset instance.
		ArmatureDefinitionAsset(StringSpan path);

		~ArmatureDefinitionAsset() override = default;

		// ---------------------------------------------------

	public:
		Result BindResources(Logging::Log& log, const Core::AssetBuilder& builder) override;

		void FreeResources() ETNoexceptHint override;

		// ---------------------------------------------------

	public:
		static ETPureFunctionHint StringSpan GetExtension() ETNoexceptHint;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		ArmatureDefinitionAsset& operator=(const ArmatureDefinitionAsset&) = delete;
	};

}}}} // namespace Eldritch2::Physics::PhysX::AssetViews
