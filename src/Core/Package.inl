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

namespace Eldritch2 { namespace Core {

	ETInlineHint ETForceInlineHint size_t Package::AddReference(MemoryOrder order) const ETNoexceptHint {
		return _referenceCount.fetch_add(1u, order);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint size_t Package::ReleaseReference(MemoryOrder order) const ETNoexceptHint {
		return _referenceCount.fetch_sub(1u, order);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint bool Package::ShouldCollect(MemoryOrder order) const ETNoexceptHint {
		return _referenceCount.load(order) == 0u;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint bool Package::IsReferenced(MemoryOrder order) const ETNoexceptHint {
		return _referenceCount.load(order) != 0u;
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint StringSpan Package::GetPath() const ETNoexceptHint {
		return StringSpan(_path, StringSpan::SizeType(StringLength(_path)));
	}

	// ---------------------------------------------------

	ETForceInlineHint FileTime Package::GetReadTime(MemoryOrder order) const ETNoexceptHint {
		return _readTime.load(order);
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint ETPureFunctionHint bool operator==(const Package& lhs, const Package& rhs) ETNoexceptHint {
		return lhs.GetPath() == rhs.GetPath();
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint ETPureFunctionHint bool operator!=(const Package& lhs, const Package& rhs) ETNoexceptHint {
		return lhs.GetPath() != rhs.GetPath();
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint ETPureFunctionHint size_t GetHashCode(const Package& package, size_t seed = 0u) ETNoexceptHint {
		return GetHashCode(package.GetPath(), seed);
	}

}} // namespace Eldritch2::Assets
