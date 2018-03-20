/*==================================================================*\
  AbstractString.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Memory/EaStlAllocatorMixin.hpp>
#include <Common/Containers/RangeAdapters.hpp>
#include <Common/Memory/MallocAllocator.hpp>
//------------------------------------------------------------------//
#include <eastl/string.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

	struct ReturnEndOfNeedleSemantics {};

	template <typename Character, class Allocator = MallocAllocator>
	class AbstractString {
	// - TYPE PUBLISHING ---------------------------------

	private:
		using UnderlyingContainer	= eastl::basic_string<Character, EaStlAllocatorMixin<Allocator>>;

	public:
		using Iterator				= typename UnderlyingContainer::iterator;
		using ConstIterator			= typename UnderlyingContainer::const_iterator;
		using SizeType				= typename UnderlyingContainer::size_type;
		using CharacterType			= typename UnderlyingContainer::value_type;
		using AllocatorType			= typename UnderlyingContainer::allocator_type::PublicType;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref AbstractString instance.
		template <typename AlternateCharacter>
		AbstractString( const AlternateCharacter* const begin, const AlternateCharacter* const end, const AllocatorType& allocator );
	//!	Constructs this @ref AbstractString instance.
		AbstractString( const CharacterType* const begin, const CharacterType* const end, const AllocatorType& allocator );
	//!	Constructs this @ref AbstractString instance.
		template <typename AlternateCharacter>
		AbstractString( const AlternateCharacter* const string, const AllocatorType& allocator );
	//!	Constructs this @ref AbstractString instance.
		AbstractString( const CharacterType* const string, const AllocatorType& allocator );
	//!	Constructs this @ref AbstractString instance.
		explicit AbstractString( SizeType reservedLength, const AllocatorType& allocator );
	//!	Constructs this @ref AbstractString instance.
		AbstractString( const AbstractString& string, const AllocatorType& allocator );
	//!	Constructs this @ref AbstractString instance.
		explicit AbstractString( const AllocatorType& allocator );
	//!	Constructs this @ref AbstractString instance.
		AbstractString( const AbstractString& );
	//!	Constructs this @ref AbstractString instance.
		AbstractString( AbstractString&& );

		~AbstractString() = default;

	// - ALGORITHMS --------------------------------------
		
	public:
		ConstIterator	FindFirstInstance( ConstIterator start, const CharacterType* const needle, const ReturnEndOfNeedleSemantics ) const;
		ConstIterator	FindFirstInstance( ConstIterator start, const CharacterType* const needle ) const;
		ConstIterator	FindFirstInstance( const CharacterType* const needle, const ReturnEndOfNeedleSemantics ) const;
		ConstIterator	FindFirstInstance( const CharacterType* const needle ) const;
		ConstIterator	FindFirstInstance( ConstIterator start, CharacterType character ) const;
		ConstIterator	FindFirstInstance( CharacterType character ) const;
		
		ConstIterator	FindLastInstance( ConstIterator start, const CharacterType* const needle, const ReturnEndOfNeedleSemantics ) const;
		ConstIterator	FindLastInstance( ConstIterator start, const CharacterType* const needle ) const;
		ConstIterator	FindLastInstance( const CharacterType* const needle, const ReturnEndOfNeedleSemantics ) const;
		ConstIterator	FindLastInstance( const CharacterType* const needle ) const;
		ConstIterator	FindLastInstance( ConstIterator start, CharacterType character ) const;
		ConstIterator	FindLastInstance( CharacterType character ) const;

		bool			Contains( const CharacterType* const needle ) const;
		bool			Contains( const CharacterType character ) const;

		bool			HasPrefix( const CharacterType* const needle ) const;
		bool			HasPrefix( const CharacterType character ) const;
		
		bool			HasSuffix( const CharacterType* const needle ) const;
		bool			HasSuffix( const CharacterType character ) const;

	// ---------------------------------------------------

	public:
		void	MakeLowerCase();

		void	MakeUpperCase();

	// ---------------------------------------------------

	public:
		void	Replace( CharacterType source, CharacterType replacement );

	// ---------------------------------------------------

	public:
		ConstIterator	Erase( ConstIterator begin, ConstIterator end );
		ConstIterator	Erase( ConstIterator position );

		void			Trim( SizeType charactersToAdvanceBeginning, SizeType charactersToRemoveFromEnd );
		void			Trim( ConstIterator newBegin, ConstIterator newEnd );

	// - ELEMENT ITERATION -------------------------------

	public:
		ConstIterator	Begin() const;

		ConstIterator	End() const;

	// - CONTAINER DUPLICATION ---------------------------

	public:
		template <typename AlternateCharacter>
		AbstractString&	Assign( const AlternateCharacter* const begin, const AlternateCharacter* const end );
		AbstractString&	Assign( const CharacterType* const begin, const CharacterType* const end );
		template <typename AlternateCharacter>
		AbstractString&	Assign( const AlternateCharacter* const string );
		AbstractString&	Assign( const CharacterType* const string );
		template <typename AlternateCharacter, typename AlternateAllocator>
		AbstractString&	Assign( const AbstractString<AlternateCharacter, AlternateAllocator>& string );

		AbstractString&	operator=( const CharacterType* const string );
		AbstractString&	operator=( const AbstractString& );
		AbstractString&	operator=( AbstractString&& );

	// - CONTAINER MANIPULATION --------------------------

	public:
		AbstractString&	EnsureSuffix( const CharacterType* const string );
		AbstractString&	EnsureSuffix( CharacterType character );

		template <typename AlternateCharacter>
		AbstractString&	Append( const AlternateCharacter* const begin, const AlternateCharacter* const end );
		AbstractString&	Append( const CharacterType* const begin, const CharacterType* const end );
		template <typename AlternateCharacter>
		AbstractString&	Append( const AlternateCharacter* const string );
		AbstractString&	Append( const CharacterType* const string );
		template <typename AlternateCharacter, typename AlternateAllocator>
		AbstractString&	Append( const AbstractString<AlternateCharacter, AlternateAllocator>& string );
		AbstractString&	Append( const AbstractString& string );
		AbstractString&	Append( CharacterType character );

		template <typename AlternateCharacter>
		AbstractString&	operator+=( const AlternateCharacter* const string );
		AbstractString&	operator+=( const CharacterType* const string );
		template <typename AlternateCharacter, typename AlternateAllocator>
		AbstractString&	operator+=( const AbstractString<AlternateCharacter, AlternateAllocator>& string );
		AbstractString&	operator+=( const AbstractString& rhs );
		AbstractString&	operator+=( CharacterType character );

	// ---------------------------------------------------

	public:
		template <size_t formatCount, typename... Arguments>
		void	Format( const Character (&formatString)[formatCount], Arguments&&... arguments );

	// ---------------------------------------------------

	public:
		void	Resize( SizeType size );

		void	Clear();

	// - CONTAINER COMPARISON ----------------------------

	public:
		int	Compare( const CharacterType* const string ) const;
		int	Compare( const AbstractString& string ) const;

	// - ELEMENT ACCESS ----------------------------------

	public:
		operator				const CharacterType*() const;

		const CharacterType*	AsCString() const;

		const CharacterType*	GetData();

		CharacterType			operator[]( SizeType indexInCharacters ) const;

	// - CONTENT QUERY -----------------------------------

	public:
		SizeType			GetLength() const;

		bool				IsEmpty() const;

		explicit operator	bool() const;

	// - CAPACITY QUERY ----------------------------------

	public:
		SizeType	GetCapacityInCharacters() const;

		void		Reserve( SizeType capacityInCharacters );

	// - ALLOCATOR ACCESS --------------------------------

	public:
		const AllocatorType&	GetAllocator() const;

	// - DATA MEMBERS ------------------------------------

	private:
		UnderlyingContainer	_container;

	// ---------------------------------------------------

		template <typename Character, class Allocator>
		friend void	Swap( AbstractString<Character, Allocator>&, AbstractString<Character, Allocator>& );
	};

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Containers/AbstractString.inl>
//------------------------------------------------------------------//