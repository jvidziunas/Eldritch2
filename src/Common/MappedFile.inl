/*==================================================================*\
  MappedFile.inl
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

namespace Eldritch2 {

template <typename Struct>
ETInlineHint ETForceInlineHint Range<Struct*> MappedFile::GetRange(size_t offsetInBytes, size_t lengthInElements) const ETNoexceptHint {
	//	Only allow const access unless we created the mapping with write permissions.
	if (!eastl::is_const<Struct>::value && !HasAccess(AccessType::Write)) {
		return Range<Struct*>();
	}

	Struct* const end(static_cast<Struct*>(Get(offsetInBytes + (sizeof(Struct) * lengthInElements))));
	return end ? Range<Struct*>(end - lengthInElements, end) : Range<Struct*>();
}

// ---------------------------------------------------

template <typename Struct>
ETInlineHint ETForceInlineHint Struct* MappedFile::Get(size_t offsetInBytes) const ETNoexceptHint {
	return Get(offsetInBytes + sizeof(Struct)) ? static_cast<Struct*>(Get(offsetInBytes)) : nullptr;
}

} // namespace Eldritch2
