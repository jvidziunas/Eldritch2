/*==================================================================*\
  SpirVShaderSetAsset.inl
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan { namespace AssetViews {

	ETInlineHint const uint32* SpirVShaderSetAsset::Usage::GetBytecode() const {
		return bytecode;
	}

	// ---------------------------------------------------

	ETInlineHint uint32 SpirVShaderSetAsset::Usage::GetBytecodeSizeInBytes() const {
		return uint32(eastl::distance(bytecode, bytecodeEnd) * sizeof(uint32));
	}

	// ---------------------------------------------------

	ETInlineHint uint32 SpirVShaderSetAsset::Find(StringView name) const {
		const auto candidate(_indexByName.Find(name, _indexByName.GetHash(), _indexByName.GetEqualityPredicate()));
		return candidate != _indexByName.End() ? candidate->second : InvalidUsageId;
	}

	// ---------------------------------------------------

	ETInlineHint const SpirVShaderSetAsset::Usage& SpirVShaderSetAsset::operator[](uint32 usage) const {
		return _usages[usage];
	}

}}}} // namespace Eldritch2::Graphics::Vulkan::AssetViews
