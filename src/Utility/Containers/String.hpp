/*==================================================================*\
  String.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Memory/RDESTLAllocatorAdapterMixin.hpp>
#include <Utility/Memory/ChildAllocator.hpp>
//------------------------------------------------------------------//
#include <rdestl/basic_string.h>
//------------------------------------------------------------------//
#include <iterator>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Detail {

	struct StringBase {
		struct ReturnEndOfNeedleSemantics {};
	};

}	// namespace Detail

	template <typename Character, class Allocator = ::Eldritch2::ChildAllocator>
	class String : public Detail::StringBase {
	// - TYPE PUBLISHING ---------------------------------

	private:
		using PrivateAllocator		= Detail::RDESTLAllocatorAdapterMixin<Allocator>;
		using UnderlyingContainer	= ::rde::basic_string<Character, PrivateAllocator>;

	public:
		using ConstIterator			= typename UnderlyingContainer::const_iterator;
		using SizeType				= typename UnderlyingContainer::size_type;
		using CharacterType			= typename UnderlyingContainer::value_type;
		using AllocatorType			= Allocator;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		//!	Constructs this @ref String instance.
		ETInlineHint explicit String( AllocatorType&& allocator = AllocatorType() );
		//!	Constructs this @ref String instance.
		ETInlineHint String( const CharacterType* const string, const CharacterType* const stringEnd, AllocatorType&& allocator = AllocatorType() );
		//!	Constructs this @ref String instance.
		template <size_t literalLength>
		ETInlineHint String( const CharacterType (&stringLiteral)[literalLength], AllocatorType&& allocator = AllocatorType() );
		//!	Constructs this @ref String instance.
		template <class AlternateAllocator>
		ETInlineHint String( const String<Character, AlternateAllocator>& string, AllocatorType&& allocator = AllocatorType() );
		//!	Constructs this @ref String instance.
		ETInlineHint String( String<Character, Allocator>&& sourceString );
		//!	Constructs this @ref String instance.
		ETInlineHint String( const String<Character, Allocator>& sourceString ) = delete;

		ETInlineHint ~String() = default;

	// - ALGORITHMS --------------------------------------

		ETInlineHint ConstIterator	IteratorToFirstInstance( const CharacterType character ) const;
		ETInlineHint ConstIterator	IteratorToFirstInstance( const CharacterType* const needle ) const;
		ETInlineHint ConstIterator	IteratorToFirstInstance( const CharacterType* const needle, const ReturnEndOfNeedleSemantics ) const;

		ETInlineHint ConstIterator	IteratorToLastInstance( const CharacterType character ) const;
		ETInlineHint ConstIterator	IteratorToLastInstance( const CharacterType* const needle ) const;
		ETInlineHint ConstIterator	IteratorToLastInstance( const CharacterType* const needle, const ReturnEndOfNeedleSemantics ) const;

		ETInlineHint bool	Contains( const CharacterType character ) const;
		ETInlineHint bool	Contains( const CharacterType* const needle ) const;

		ETInlineHint void	MakeLower();
		ETInlineHint void	MakeUpper();

		ETInlineHint bool	BeginsWith( const CharacterType character ) const;
		ETInlineHint bool	BeginsWith( const CharacterType* const needle ) const;

		ETInlineHint bool	EndsWith( const CharacterType character ) const;
		ETInlineHint bool	EndsWith( const CharacterType* const needle ) const;

		template <typename Value>
		bool	ParseInto( Value&& value ) const;

	// - ELEMENT ITERATION -------------------------------

		ETInlineHint ConstIterator	Begin() const;

		ETInlineHint ConstIterator	End() const;

	// - CONTAINER DUPLICATION ---------------------------

		template <class AlternateAllocator>
		ETInlineHint String<Character, Allocator>&	operator=( const String<Character, AlternateAllocator>& rhs );
		ETInlineHint String<Character, Allocator>&	operator=( const CharacterType* const string );

	// - CONTAINER MANIPULATION --------------------------

		template <class AlternateAllocator>
		ETInlineHint String<Character, AlternateAllocator>	CreateSubstring( AlternateAllocator&& allocator, const ConstIterator begin, const ConstIterator end );
		template <class AlternateAllocator>
		ETInlineHint String<Character, AlternateAllocator>	CreateSubstring( AlternateAllocator&& allocator, const ConstIterator begin );

		ETInlineHint String<Character, Allocator>&	Assign( const CharacterType* const string );
		ETInlineHint String<Character, Allocator>&	Assign( const CharacterType* const begin, const CharacterType* const end );

		ETInlineHint String<Character, Allocator>&	Append( const CharacterType character );
		
		ETInlineHint String<Character, Allocator>&	Append( const CharacterType* const string );
		ETInlineHint String<Character, Allocator>&	Append( const CharacterType* const begin, const CharacterType* const end );
		template <class AlternateAllocator>
		ETInlineHint String<Character, Allocator>&	Append( const String<Character, AlternateAllocator>& string );

		ETInlineHint String<Character, Allocator>&	EnsureEndsWith( const CharacterType character );
		ETInlineHint String<Character, Allocator>&	EnsureEndsWith( const CharacterType* const string );

		template <class AlternateAllocator>
		ETInlineHint String<Character, Allocator>&	operator+=( const String<Character, AlternateAllocator>& rhs );

		ETInlineHint void	Clear();

		ETInlineHint void	Resize( const SizeType size );

	// - CONTAINER COMPARISON ----------------------------

		ETInlineHint int	Compare( const CharacterType* const string ) const;
		template <class AlternateAllocator>
		ETInlineHint int	Compare( const String<Character, AlternateAllocator>& rhs ) const;

		ETInlineHint size_t	GetHashCode( const size_t hashSeed = static_cast<size_t>(0) ) const;

	// - ELEMENT ACCESS ----------------------------------

		ETInlineHint CharacterType	operator[]( const SizeType indexInBytes ) const;

		ETInlineHint const CharacterType*	GetCharacterArray() const;

	// - CONTENT QUERY -----------------------------------

		ETInlineHint SizeType	Length() const;

		ETInlineHint bool		Empty() const;

		ETInlineHint operator bool();

	// - CAPACITY QUERY ----------------------------------

		ETInlineHint void		Reserve( const SizeType capacityHintInBytes );

		ETInlineHint SizeType	CapacityInBytes() const;

	// - ALLOCATOR ACCESS --------------------------------

		ETInlineHint const AllocatorType&	GetAllocator() const;

	// - DATA MEMBERS ------------------------------------

	private:
		UnderlyingContainer	_underlyingContainer;
	};

// ---

	template <typename Character, class StringAllocator0, class StringAllocator1 = StringAllocator0>
	ETNoAliasHint bool	operator==( const String<Character, StringAllocator0>& string0, const String<Character, StringAllocator1>& string1 );
	template <typename Character, class StringAllocator>
	ETNoAliasHint bool	operator==( const String<Character, StringAllocator>& string0, const Character* const string1 );
	template <typename Character, class StringAllocator>
	ETNoAliasHint bool	operator==( const Character* const string0, const String<Character, StringAllocator>& string1 );

// ---

	extern const Detail::StringBase::ReturnEndOfNeedleSemantics	ReturnEndOfNeedleSemantics;

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/Containers/String.inl>
//------------------------------------------------------------------//