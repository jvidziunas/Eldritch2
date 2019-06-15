/*==================================================================*\
  SymbolSet.hpp
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
#include <Common/Memory/MallocAllocator.hpp>
#include <Common/Memory/ChildAllocator.hpp>
#include <Common/Containers/HashSet.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

template <typename Character, class Allocator = MallocAllocator>
class SymbolSet {
	// - TYPE PUBLISHING ---------------------------------

public:
	enum Symbol : uintptr {
		InvalidSymbol,
	};

	// ---

public:
	using TableType     = CachingHashSet<AbstractString<Character, ChildAllocator>, Hash<AbstractString<Character, ChildAllocator>>, EqualTo<AbstractString<Character, ChildAllocator>>, Allocator>;
	using StringType    = typename TableType::ValueType;
	using AllocatorType = typename TableType::AllocatorType;
	using SliceType     = typename StringType::ConstSliceType;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref SymbolSet instance.
	SymbolSet(const AllocatorType& allocator) ETNoexceptHint;
	//!	Constructs this @ref SymbolSet instance.
	SymbolSet(const SymbolSet&) = default;
	//!	Constructs this @ref SymbolSet instance.
	SymbolSet(SymbolSet&&) ETNoexceptHint = default;

	~SymbolSet() = default;

	// ---------------------------------------------------

public:
	Symbol Find(SliceType string) const ETNoexceptHint;

	Symbol Intern(SliceType string);

	// ---------------------------------------------------

public:
	void Clear();

	// ---------------------------------------------------

public:
	ETConstexpr const AllocatorType& GetAllocator() const ETNoexceptHint;

	// - DATA MEMBERS ------------------------------------

private:
	TableType _symbols;

	// ---------------------------------------------------

	template <typename Character2, class Allocator2>
	friend void Swap(SymbolSet<Character2, Allocator2>&, SymbolSet<Character2, Allocator2>&) ETNoexceptHint;
};

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Containers/SymbolSet.inl>
//------------------------------------------------------------------//
