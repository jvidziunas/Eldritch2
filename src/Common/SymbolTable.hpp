/*==================================================================*\
  SymbolTable.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Containers/AbstractString.hpp>
#include <Common/Memory/ChildAllocator.hpp>
#include <Common/Containers/HashSet.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

template <typename Character, class Allocator = MallocAllocator>
class SymbolTable {
	// - TYPE PUBLISHING ---------------------------------

public:
	using Symbol        = const Character*;
	using CharacterType = Character;
	using AllocatorType = Allocator;
	using StringType    = AbstractString<CharacterType, ChildAllocator>;
	using TableType     = CachingHashSet<StringType, Hash<StringType>, EqualTo<StringType>, ChildAllocator>;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref SymbolTable instance.
	SymbolTable(const AllocatorType& allocator);
	//!	Constructs this @ref SymbolTable instance.
	SymbolTable(const SymbolTable&) = default;

	~SymbolTable() = default;

	// ---------------------------------------------------

public:
	Symbol Find(AbstractStringView<CharacterType> string) const;

	Symbol Intern(AbstractStringView<CharacterType> string);

	// ---------------------------------------------------

public:
	void Clear();

	ETConstexpr const AllocatorType& GetAllocator() const ETNoexceptHint;

	// - DATA MEMBERS ------------------------------------

private:
	AllocatorType _allocator;
	TableType     _symbols;
};

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/SymbolTable.inl>
//------------------------------------------------------------------//
