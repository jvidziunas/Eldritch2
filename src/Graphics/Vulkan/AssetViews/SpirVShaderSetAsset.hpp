/*==================================================================*\
  SpirVShaderSetAsset.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/SpirVShaderSet.hpp>
#include <Assets/Asset.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan { namespace AssetViews {

	class SpirVShaderSetAsset : public Vulkan::SpirVShaderSet, public Assets::Asset {
		// - TYPE PUBLISHING ---------------------------------

	public:
		using ContainerType = Vulkan::SpirVShaderSet;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		SpirVShaderSetAsset(const SpirVShaderSetAsset&) = delete;
		//!	Constructs this @ref SpirVShaderSetAsset instance.
		SpirVShaderSetAsset(StringView path);

		~SpirVShaderSetAsset() override = default;

		// ---------------------------------------------------

	public:
		ErrorCode BindResources(const Builder& builder) override;

		void FreeResources() override;

		// ---------------------------------------------------

	public:
		static ETPureFunctionHint StringView GetExtension() ETNoexceptHint;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		SpirVShaderSetAsset& operator=(const SpirVShaderSetAsset&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		ArrayList<uint32> _bytecode;
	};

}}}} // namespace Eldritch2::Graphics::Vulkan::AssetViews
