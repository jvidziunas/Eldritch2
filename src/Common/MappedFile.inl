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
ETInlineHint Range<Struct*> MappedFile::GetRangeAtOffset(size_t offsetInBytes, size_t lengthInElements) const {
	//	Only allow const access unless we created the mapping with write permissions.
	if (!eastl::is_const<Struct>::value && !HasAccessLevel(AccessMode::Write)) {
		return Range<Struct*>();
	}

	Struct* const end(static_cast<Struct*>(GetAddressForByteOffset(offsetInBytes + (sizeof(Struct) * lengthInElements))));

	return end ? Range<Struct*>(end - lengthInElements, end) : Range<Struct*>();
}

// ---------------------------------------------------

template <typename Struct>
ETInlineHint Struct* MappedFile::GetAtOffset(size_t offsetInBytes) const {
	return GetAddressForByteOffset(offsetInBytes + sizeof(Struct)) ? static_cast<Struct*>(GetAddressForByteOffset(offsetInBytes)) : nullptr;
}

} // namespace Eldritch2
