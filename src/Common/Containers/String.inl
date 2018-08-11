/*==================================================================*\
  String.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {

template <class Allocator>
template <typename AlternateCharacter>
ETInlineHint Path<Allocator>::Path(StringView<AlternateCharacter> string, const AllocatorType& allocator) :
	AbstractString<PlatformChar, Allocator>(string, allocator) {
}

// ---------------------------------------------------

template <class Allocator>
ETInlineHint Path<Allocator>::Path(StringView<CharacterType> string, const AllocatorType& allocator) :
	AbstractString<PlatformChar, Allocator>(string, allocator) {
}

// ---------------------------------------------------

template <class Allocator>
ETInlineHint Path<Allocator>::Path(SizeType reservedLength, const AllocatorType& allocator) :
	AbstractString<PlatformChar, Allocator>(reservedLength, allocator) {
}

// ---------------------------------------------------

template <class Allocator>
ETInlineHint Path<Allocator>::Path(const Path& string, const AllocatorType& allocator) :
	AbstractString<PlatformChar, Allocator>(string, allocator) {
}

// ---------------------------------------------------

template <class Allocator>
ETInlineHint Path<Allocator>::Path(const AllocatorType& allocator) :
	AbstractString<PlatformChar, Allocator>(allocator) {}

// ---------------------------------------------------

template <class Allocator>
template <typename... Arguments>
ETInlineHint Path<Allocator>& Path<Allocator>::Assign(KnownDirectory root, StringView<CharacterType> format, Arguments&&... arguments) {
	return Append(format, eastl::forward<Arguments>(arguments)...);
};

// ---------------------------------------------------

template <class Allocator>
ETInlineHint Path<Allocator>& Path<Allocator>::Assign(KnownDirectory root, StringView<CharacterType> path) {
	return Append(path);
}

} // namespace Eldritch2
