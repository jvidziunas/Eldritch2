/*==================================================================*\
  SymbolTable.inl
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint SymbolTable<Character, Allocator>::SymbolTable(const AllocatorType& allocator) :
	_allocator(allocator),
	_symbols(ChildAllocator(_allocator, "Symbol Table Bucket Allocator")) {
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint typename SymbolTable<Character, Allocator>::Symbol SymbolTable<Character, Allocator>::Find(AbstractStringView<CharacterType> string) const {
	auto candidate(_symbols.Find(string, _symbols.GetHash(), _symbols.GetEqualityPredicate()));
	return candidate != _symbols.End() ? Symbol(*candidate) : nullptr;
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint typename SymbolTable<Character, Allocator>::Symbol SymbolTable<Character, Allocator>::Intern(AbstractStringView<CharacterType> string) {
	auto candidate(_symbols.Find(string, _symbols.GetHash(), _symbols.GetEqualityPredicate()));
	if (candidate == _symbols.End()) {
		candidate = _symbols.Emplace(ChildAllocator(_allocator, "Symbol String Allocator"), string).first;
	}

	return Symbol(*candidate);
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint void SymbolTable<Character, Allocator>::Clear() {
	_symbols.Clear();
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETConstexpr ETInlineHint ETForceInlineHint typename const SymbolTable<Character, Allocator>::AllocatorType& SymbolTable<Character, Allocator>::GetAllocator() const ETNoexceptHint {
	return _allocator;
}

} // namespace Eldritch2
