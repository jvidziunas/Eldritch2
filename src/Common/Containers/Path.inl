/*==================================================================*\
  Path.inl
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
template <typename... Arguments>
ETInlineHint AbstractPath<Allocator>::AbstractPath(const AllocatorType& allocator, KnownDirectory root, AbstractStringView<CharacterType> format, Arguments&&... arguments) :
	AbstractPath(allocator) {
	Assign(root, format, eastl::forward<Arguments>(arguments)...);
}

// ---------------------------------------------------

template <class Allocator>
template <typename Character>
ETInlineHint AbstractPath<Allocator>::AbstractPath(const AllocatorType& allocator, KnownDirectory root, AbstractStringView<Character> path) :
	AbstractPath(allocator) {
	Assign(root, path);
}

// ---------------------------------------------------

template <class Allocator>
ETInlineHint AbstractPath<Allocator>::AbstractPath(const AllocatorType& allocator, AbstractStringView<CharacterType> path) :
	AbstractString<PlatformChar, Allocator>(allocator, path) {
}

// ---------------------------------------------------

template <class Allocator>
ETInlineHint AbstractPath<Allocator>::AbstractPath(const AllocatorType& allocator, SizeType reservedLength) :
	AbstractString<PlatformChar, Allocator>(allocator, reservedLength) {
}

// ---------------------------------------------------

template <class Allocator>
ETInlineHint AbstractPath<Allocator>::AbstractPath(const AllocatorType& allocator) :
	AbstractString<PlatformChar, Allocator>(allocator) {}

// ---------------------------------------------------

template <class Allocator>
template <typename... Arguments>
ETInlineHint AbstractPath<Allocator>& AbstractPath<Allocator>::Assign(KnownDirectory root, AbstractStringView<CharacterType> format, Arguments&&... arguments) {
	Assign(GetPath(root)).Append(format, eastl::forward<Arguments>(arguments)...);
	return *this;
};

// ---------------------------------------------------

template <class Allocator>
template <typename Character>
ETInlineHint AbstractPath<Allocator>& AbstractPath<Allocator>::Assign(KnownDirectory root, AbstractStringView<Character> path) {
	Assign(GetPath(root)).Append(path);
	return *this;
}

// ---------------------------------------------------

template <class Allocator>
ETInlineHint void Swap(AbstractPath<Allocator>& lhs, AbstractPath<Allocator>& rhs) {
	lhs._container.swap(rhs._container);
}

} // namespace Eldritch2

template <class Allocator, typename OutCharacter>
struct ::fmt::v5::formatter<::Eldritch2::AbstractPath<Allocator>, OutCharacter> : public ::fmt::v5::formatter<::Eldritch2::AbstractString<typename ::Eldritch2::AbstractPath<Allocator>::ValueType, Allocator>, OutCharacter> {};
