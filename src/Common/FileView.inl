/*==================================================================*\
  FileView.inl
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
ETInlineHint ETForceInlineHint Span<Struct*> FileView::Slice(size_t byteOffset, size_t length) const ETNoexceptHint {
	//	Only allow const access unless we created the mapping with write permissions.
	if (!eastl::is_const_v<Struct> && !HasAccess(FileAccess::Write)) {
		return Span<Struct*>();
	}

	const auto end(static_cast<Struct*>(Get(byteOffset + sizeof(Struct) * length)));
	return end ? Span<Struct*>(end - length, end) : Span<Struct*>();
}

// ---------------------------------------------------

ETInlineHint Span<const byte*> FileView::Slice(size_t byteOffset, size_t byteLength) const ETNoexceptHint {
	return Slice<const byte>(byteOffset, byteLength);
}

// ---------------------------------------------------

template <typename Character>
ETInlineHint AbstractStringSpan<Character> FileView::StringSlice(size_t byteOffset, typename AbstractStringSpan<Character>::SizeType length) const ETNoexceptHint {
	using CharacterType = typename AbstractStringSpan<Character>::CharacterType;

	const auto end(static_cast<const CharacterType*>(Get(byteOffset + sizeof(CharacterType) * length)));
	return end ? AbstractStringSpan<Character>(static_cast<CharacterType*>(Get(byteOffset)), length) : AbstractStringSpan<Character>();
}

// ---------------------------------------------------

template <typename Struct>
ETInlineHint ETForceInlineHint Struct* FileView::Get(size_t byteOffset) const ETNoexceptHint {
	return Get(byteOffset + sizeof(Struct)) ? static_cast<Struct*>(Get(byteOffset)) : nullptr;
}

} // namespace Eldritch2
