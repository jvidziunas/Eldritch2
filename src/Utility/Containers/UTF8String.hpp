/*==================================================================*\
  UTF8String.hpp
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
		struct UTF32CodepointSemantics {};
		struct ReturnEndOfNeedleSemantics {};

		class UTF32CodepointIterator : public ::std::iterator<::std::input_iterator_tag, ::Eldritch2::UTF32Char> {};
	};

}	// namespace Detail

	template <class Allocator = ::Eldritch2::ChildAllocator>
	class UTF8String : public Detail::StringBase {
	// - TYPE PUBLISHING ---------------------------------

	private:
		using PrivateAllocator		= Detail::RDESTLAllocatorAdapterMixin<Allocator>;
		using UnderlyingContainer	= ::rde::basic_string<::Eldritch2::UTF8Char, PrivateAllocator>;

	public:
		using ConstIterator			= typename UnderlyingContainer::const_iterator;
		using SizeType				= typename UnderlyingContainer::size_type;
		using CharacterType			= typename UnderlyingContainer::value_type;
		using AllocatorType			= Allocator;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		//!	Constructs this @ref UTF8String instance.
		ETInlineHint UTF8String( AllocatorType&& allocator = AllocatorType() );
		//!	Constructs this @ref UTF8String instance.
		ETInlineHint UTF8String( const CharacterType* const string, const CharacterType* const stringEnd, AllocatorType&& allocator = AllocatorType() );
		//!	Constructs this @ref UTF8String instance.
		template <size_t literalLength>
		ETInlineHint UTF8String( const CharacterType (&stringLiteral)[literalLength], AllocatorType&& allocator = AllocatorType() );
		//!	Constructs this @ref UTF8String instance.
		template <class AlternateAllocator>
		ETInlineHint UTF8String( const ::Eldritch2::UTF8String<AlternateAllocator>& string, AllocatorType&& allocator = AllocatorType() );
		//!	Constructs this @ref UTF8String instance.
		ETInlineHint UTF8String( ::Eldritch2::UTF8String<Allocator>&& sourceString );

		//!	Destroys this @ref UTF8String instance.
		ETInlineHint ~UTF8String() = default;

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

	// - ELEMENT ITERATION -------------------------------

		ETInlineHint ConstIterator			Begin() const;
		ETInlineHint UTF32CodepointIterator	Begin( const UTF32CodepointSemantics ) const;

		ETInlineHint ConstIterator			End() const;
		ETInlineHint UTF32CodepointIterator	End( const UTF32CodepointSemantics ) const;

	// - CONTAINER DUPLICATION ---------------------------

		template <class AlternateAllocator>
		ETInlineHint ::Eldritch2::UTF8String<Allocator>&	operator=( const ::Eldritch2::UTF8String<AlternateAllocator>& rhs );
		ETInlineHint ::Eldritch2::UTF8String<Allocator>&	operator=( const CharacterType* const string );

	// - CONTAINER MANIPULATION --------------------------

		template <class AlternateAllocator>
		ETInlineHint ::Eldritch2::UTF8String<AlternateAllocator>	Substring( AlternateAllocator& allocator, const ConstIterator begin, const ConstIterator end );
		template <class AlternateAllocator>
		ETInlineHint ::Eldritch2::UTF8String<AlternateAllocator>	Substring( AlternateAllocator& allocator, const ConstIterator begin );

		ETInlineHint::Eldritch2::UTF8String<Allocator>&	Assign( const CharacterType* const str );
		ETInlineHint::Eldritch2::UTF8String<Allocator>&	Assign( const CharacterType* const str, const SizeType lengthInBytes );

		ETInlineHint::Eldritch2::UTF8String<Allocator>&		Append( const CharacterType character );
		
		ETInlineHint ::Eldritch2::UTF8String<Allocator>&	Append( const CharacterType* const string );
		ETInlineHint::Eldritch2::UTF8String<Allocator>&		Append( const CharacterType* const stringBegin, const SizeType lengthInBytes );
		template <class AlternateAllocator>
		ETInlineHint ::Eldritch2::UTF8String<Allocator>&	Append( const ::Eldritch2::UTF8String<AlternateAllocator>& string );

		ETInlineHint ::Eldritch2::UTF8String<Allocator>&	EnsureEndsWith( const CharacterType character );
		ETInlineHint ::Eldritch2::UTF8String<Allocator>&	EnsureEndsWith( const CharacterType* const string );

		template <class AlternateAllocator>
		ETInlineHint ::Eldritch2::UTF8String<Allocator>&	operator+=( const ::Eldritch2::UTF8String<AlternateAllocator>& rhs );

		ETInlineHint void	Clear();

		ETInlineHint void	Resize( const SizeType size );

	// - CONTAINER COMPARISON ----------------------------

		ETInlineHint int	Compare( const CharacterType* const string ) const;
		template <class AlternateAllocator>
		ETInlineHint int	Compare( const ::Eldritch2::UTF8String<AlternateAllocator>& rhs ) const;

	// - ELEMENT ACCESS ----------------------------------

		ETInlineHint CharacterType	operator[]( const SizeType indexInBytes ) const;

		ETInlineHint const CharacterType*	GetCharacterArray() const;

	// - CONTENT QUERY -----------------------------------

		ETInlineHint SizeType	Length() const;
		ETInlineHint SizeType	Length( const UTF32CodepointSemantics ) const;

		ETInlineHint bool	Empty() const;

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

	extern const Detail::StringBase::UTF32CodepointSemantics	UTF32CodepointSemantics;
	extern const Detail::StringBase::ReturnEndOfNeedleSemantics	ReturnEndOfNeedleSemantics;

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/Containers/UTF8String.inl>
//------------------------------------------------------------------//
