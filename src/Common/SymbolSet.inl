/*==================================================================*\
  SymbolSet.inl
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
ETInlineHint ETForceInlineHint SymbolSet<Character, Allocator>::SymbolSet(const AllocatorType& allocator) ETNoexceptHint : _symbols(typename TableType::AllocatorType(_allocator, "Symbol Table Root Allocator")) {}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint typename SymbolSet<Character, Allocator>::Symbol SymbolSet<Character, Allocator>::Find(SliceType symbol) const ETNoexceptHint {
	auto candidate(_symbols.Find(symbol, _symbols.GetHash(), _symbols.GetEqualityPredicate()));
	return candidate != _symbols.End() ? Symbol(*candidate) : SymbolSet<Character, Allocator>::InvalidSymbol;
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint typename SymbolSet<Character, Allocator>::Symbol SymbolSet<Character, Allocator>::Intern(SliceType symbol) {
	auto candidate(_symbols.Find(symbol, _symbols.GetHash(), _symbols.GetEqualityPredicate()));
	if (candidate == _symbols.End()) {
		candidate = _symbols.Emplace(typename StringType::AllocatorType(GetAllocator(), "Symbol Allocator"), symbol).first;
	}

	return Symbol(*candidate);
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint void SymbolSet<Character, Allocator>::Clear() {
	_symbols.Clear();
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETConstexpr ETForceInlineHint typename const SymbolSet<Character, Allocator>::AllocatorType& SymbolSet<Character, Allocator>::GetAllocator() const ETNoexceptHint {
	return _symbols.GetAllocator();
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint void Swap(SymbolSet<Character, Allocator>& lhs, SymbolSet<Character, Allocator>& rhs) ETNoexceptHint {
	Swap(lhs._symbols, rhs._symbols);
}

} // namespace Eldritch2
