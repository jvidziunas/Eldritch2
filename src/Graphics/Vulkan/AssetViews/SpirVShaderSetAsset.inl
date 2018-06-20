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

	ETInlineHint ETPureFunctionHint bool SpirVShaderSetAsset::UsageEqual::operator()(const UsageAsset& usage, const Utf8Char* name) const {
		return StringsEqual(usage.name, name);
	}

	// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint bool SpirVShaderSetAsset::UsageEqual::operator()(const UsageAsset& usage0, const UsageAsset& usage1) const {
		return (*this)(usage0, usage1.name);
	}

	// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint size_t SpirVShaderSetAsset::UsageHash::operator()(const Utf8Char* name, size_t seed) const {
		return HashMemory(name, StringLength(name), seed);
	}

	// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint size_t SpirVShaderSetAsset::UsageHash::operator()(const UsageAsset& usage, size_t seed) const {
		return (*this)(usage.name, seed);
	}

	// ---------------------------------------------------

	ETInlineHint const uint32* SpirVShaderSetAsset::GetBytecode(uint32 offset) const {
		return _bytecode.GetData() + offset;
	}

	// ---------------------------------------------------

	ETInlineHint uint32 SpirVShaderSetAsset::GetBytecodeSize() const {
		// Vulkan wants this value in bytes, not 32-bit opcodes.
		return static_cast<uint32>(_bytecode.GetSize() * sizeof(uint32));
	}

	// ---------------------------------------------------

	ETInlineHint typename SpirVShaderSetAsset::UsageIterator SpirVShaderSetAsset::Find(const Utf8Char* name) const {
		return _usages.Find(name, _usages.GetHashPredicate(), _usages.GetEqualityPredicate());
	}

}}}} // namespace Eldritch2::Graphics::Vulkan::AssetViews
