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
#include <Utility/Memory/EaStlAllocatorAdapterMixin.hpp>
#include <Utility/Containers/RangeAdapters.hpp>
#include <Utility/Memory/ChildAllocator.hpp>
//------------------------------------------------------------------//
#if ET_COMPILER_IS_MSVC && !defined( EA_COMPILER_HAS_C99_FORMAT_MACROS )
//	MSVC complains about *lots* of macro redefinitions in eabase/inttypes.h.
#	define EA_COMPILER_HAS_C99_FORMAT_MACROS
#endif
#include <eastl/string.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Detail {

	struct StringBase {
		struct ReturnEndOfNeedleSemantics {};
	};

}	// namespace Detail

	template <typename Character, class Allocator = Eldritch2::ChildAllocator>
	class String : public Detail::StringBase {
	// - TYPE PUBLISHING ---------------------------------

	private:
		using UnderlyingContainer	= eastl::basic_string<Character, Detail::EaStlAllocatorAdapterMixin<Allocator>>;

	public:
		using Iterator				= typename UnderlyingContainer::iterator;
		using ConstIterator			= typename UnderlyingContainer::const_iterator;
		using SizeType				= typename UnderlyingContainer::size_type;
		using CharacterType			= typename UnderlyingContainer::value_type;
		using AllocatorType			= typename UnderlyingContainer::allocator_type::PublicType;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref String instance.
		String( const CharacterType* const string, const CharacterType* const stringEnd, const AllocatorType& allocator );
	//!	Constructs this @ref String instance.
		explicit String( const CharacterType* const string, const AllocatorType& allocator );
	//!	Constructs this @ref String instance.
		explicit String( SizeType reservedLength, const AllocatorType& allocator );
	//!	Constructs this @ref String instance.
		String( const String& string, const AllocatorType& allocator );
	//!	Constructs this @ref String instance.
		explicit String( const AllocatorType& allocator );
	//!	Constructs this @ref String instance.
		String( const String& );
	//!	Constructs this @ref String instance.
		String( String&& );

		~String() = default;

	// - ALGORITHMS --------------------------------------
		
	public:
		ConstIterator	FindFirstInstance( ConstIterator startPosition, const CharacterType* const needle, const ReturnEndOfNeedleSemantics ) const;
		ConstIterator	FindFirstInstance( ConstIterator startPosition, const CharacterType* const needle ) const;
		ConstIterator	FindFirstInstance( const CharacterType* const needle, const ReturnEndOfNeedleSemantics ) const;
		ConstIterator	FindFirstInstance( const CharacterType* const needle ) const;
		ConstIterator	FindFirstInstance( ConstIterator startPosition, CharacterType character ) const;
		ConstIterator	FindFirstInstance( CharacterType character ) const;
		
		ConstIterator	FindLastInstance( ConstIterator startPosition, const CharacterType* const needle, const ReturnEndOfNeedleSemantics ) const;
		ConstIterator	FindLastInstance( ConstIterator startPosition, const CharacterType* const needle ) const;
		ConstIterator	FindLastInstance( const CharacterType* const needle, const ReturnEndOfNeedleSemantics ) const;
		ConstIterator	FindLastInstance( const CharacterType* const needle ) const;
		ConstIterator	FindLastInstance( ConstIterator startPosition, CharacterType character ) const;
		ConstIterator	FindLastInstance( CharacterType character ) const;

		bool			Contains( const CharacterType* const needle ) const;
		bool			Contains( const CharacterType character ) const;

		bool			BeginsWith( const CharacterType* const needle ) const;
		bool			BeginsWith( const CharacterType character ) const;
		
		bool			EndsWith( const CharacterType* const needle ) const;
		bool			EndsWith( const CharacterType character ) const;

	// ---------------------------------------------------

	public:
		void	MakeLowerCase();

		void	MakeUpperCase();

	// ---------------------------------------------------

	public:
		ConstIterator	Erase( ConstIterator position, ConstIterator end );
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
		String&	Assign( const AlternateCharacter* const begin, const AlternateCharacter* const end );
		String&	Assign( const CharacterType* const begin, const CharacterType* const end );
		template <typename AlternateCharacter>
		String&	Assign( const AlternateCharacter* const string );
		String&	Assign( const CharacterType* const string );
		template <typename AlternateCharacter, typename AlternateAllocator>
		String&	Assign( const String<AlternateCharacter, AlternateAllocator>& string );
		String&	Assign( const String& );
		String&	Assign( String&& );

		String&	operator=( const CharacterType* const string );
		String&	operator=( const String& );
		String&	operator=( String&& );

		void	Swap( String& other );

	// - CONTAINER MANIPULATION --------------------------

	public:
		String&	EnsureEndsWith( const CharacterType* const string );
		String&	EnsureEndsWith( CharacterType character );

		template <typename AlternateCharacter>
		String&	Append( const AlternateCharacter* const begin, const AlternateCharacter* const end );
		String&	Append( const CharacterType* const begin, const CharacterType* const end );
		template <typename AlternateCharacter>
		String&	Append( const AlternateCharacter* const string );
		String&	Append( const CharacterType* const string );
		template <typename AlternateCharacter, typename AlternateAllocator>
		String&	Append( const String<AlternateCharacter, AlternateAllocator>& string );
		String&	Append( const String& string );
		String&	Append( CharacterType character );

		template <typename AlternateCharacter>
		String&	operator+=( const AlternateCharacter* const string );
		String&	operator+=( const CharacterType* const string );
		template <typename AlternateCharacter, typename AlternateAllocator>
		String&	operator+=( const String<AlternateCharacter, AlternateAllocator>& string );
		String&	operator+=( const String& rhs );
		String&	operator+=( CharacterType character );

	// ---------------------------------------------------

	public:
		void	Resize( SizeType size );

		void	Clear();

	// - CONTAINER COMPARISON ----------------------------

	public:
		int	Compare( const CharacterType* const string ) const;
		int	Compare( const String& rhs ) const;

	// - ELEMENT ACCESS ----------------------------------

	public:
		operator				const CharacterType*() const;

		const CharacterType*	AsCString() const;

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
		UnderlyingContainer	_underlyingContainer;
	};

// ---

	extern const Detail::StringBase::ReturnEndOfNeedleSemantics	ReturnEndOfNeedleSemantics;

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/Containers/String.inl>
//------------------------------------------------------------------//