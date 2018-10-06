/*==================================================================*\
  ArmatureResourceView.hpp
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
#include <Assets/Asset.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Physics { namespace PhysX { namespace AssetViews {

	class ArmatureAsset : public Animation::ArmatureDefinition, public Assets::Asset {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		ArmatureAsset(const ArmatureAsset&) = delete;
		//!	Constructs this @ref ArmatureAsset instance.
		ArmatureAsset(StringView path);

		~ArmatureAsset() override = default;

		// ---------------------------------------------------

	public:
		ErrorCode BindResources(const Builder& builder) override;

		void FreeResources() override;

		// ---------------------------------------------------

	public:
		static ETPureFunctionHint StringView GetExtension() ETNoexceptHint;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		ArmatureAsset& operator=(const ArmatureAsset&) = delete;
	};

}}}} // namespace Eldritch2::Physics::PhysX::AssetViews
