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
	ETInlineHint SymbolTable<Character, Allocator>::SymbolTable(
		const AllocatorType& allocator
	) : _allocator( allocator ),
		_symbols( ChildAllocator( _allocator, "Symbol Table Bucket Allocator" ) ) {}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint typename SymbolTable<Character, Allocator>::Symbol SymbolTable<Character, Allocator>::Find( const CharacterType* string ) const {
		auto	candidate( _symbols.Find( string, Hash<const CharacterType*>(), eastl::equal_to_2<StringType, const CharacterType*>() ) );

		if (candidate == _symbols.End()) {
			return nullptr;
		}

		return reinterpret_cast<Symbol>(candidate->AsCString());
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint typename SymbolTable<Character, Allocator>::Symbol SymbolTable<Character, Allocator>::Intern( const CharacterType* string ) {
		auto	candidate( _symbols.Find( string, Hash<const CharacterType*>(), eastl::equal_to_2<StringType, const CharacterType*>() ) );

		if (candidate == _symbols.End()) {
			candidate = _symbols.Emplace( string, FindTerminator( string ), ChildAllocator( _allocator, "Symbol String Allocator" ) ).first;
		}

		return reinterpret_cast<Symbol>(candidate->AsCString());
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint void SymbolTable<Character, Allocator>::Clear() {
		_symbols.Clear();
	}

}	// namespace Eldritch2