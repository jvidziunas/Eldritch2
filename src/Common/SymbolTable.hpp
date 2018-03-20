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

	private:
		struct	Symbol_				{};

	public:
		using	StringType			= AbstractString<Character, ChildAllocator>;
		using	CharacterType		= typename StringType::CharacterType;
		using	AllocatorType		= Allocator;
		using	TableType			= CachingHashSet<StringType, Hash<StringType>, EqualTo<StringType>, ChildAllocator>;
		using	Symbol				= const Symbol_*;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref SymbolTable instance.
		SymbolTable( const AllocatorType& allocator );
	//!	Constructs this @ref SymbolTable instance.
		SymbolTable( const SymbolTable& ) = default;

		~SymbolTable() = default;

	// ---------------------------------------------------

	public:
		Symbol	Find( const CharacterType* string ) const;

		Symbol	Intern( const CharacterType* string );

	// ---------------------------------------------------

	public:
		void	Clear();

	// - DATA MEMBERS ------------------------------------

	private:
		AllocatorType	_allocator;
		TableType		_symbols;
	};

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/SymbolTable.inl>
//------------------------------------------------------------------//