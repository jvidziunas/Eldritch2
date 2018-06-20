/*==================================================================*\
  Package.inl
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 { namespace Assets {

	ETInlineHint size_t Package::AddReference(MemoryOrder order) const {
		return _referenceCount.fetch_add(1u, order);
	}

	// ---------------------------------------------------

	ETInlineHint size_t Package::ReleaseReference(MemoryOrder order) const {
		return _referenceCount.fetch_sub(1u, order);
	}

	// ---------------------------------------------------

	ETInlineHint bool Package::IsReferenced(MemoryOrder order) const {
		return _referenceCount.load(order) != 0u;
	}

	// ---------------------------------------------------

	ETInlineHint const Utf8Char* Package::GetPath() const {
		return _path;
	}

	// ---------------------------------------------------

	ETInlineHint bool Package::IsLoaded(MemoryOrder order) const {
		return _isLoaded.load(order);
	}

	// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint bool operator==(const Package& lhs, const Package& rhs) {
		return StringsEqual(lhs.GetPath(), rhs.GetPath());
	}

	// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint bool operator!=(const Package& lhs, const Package& rhs) {
		return !StringsEqual(lhs.GetPath(), rhs.GetPath());
	}

	// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint size_t GetHashCode(const Package& package, size_t seed = 0u) {
		return HashMemory(package.GetPath(), StringLength(package.GetPath()), seed);
	}

}} // namespace Eldritch2::Assets
