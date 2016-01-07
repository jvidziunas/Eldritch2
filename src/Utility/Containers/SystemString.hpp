/*==================================================================*\
  SystemString.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/String.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <class Allocator = ::Eldritch2::ChildAllocator>
	class SystemString : public String<::Eldritch2::SystemChar, Allocator> {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref SystemString instance.
		ETInlineHint explicit SystemString( AllocatorType&& allocator = AllocatorType() );
		//!	Constructs this @ref SystemString instance.
		ETInlineHint SystemString( const CharacterType* const string, const CharacterType* const stringEnd, AllocatorType&& allocator = AllocatorType() );
		//!	Constructs this @ref SystemString instance.
		template <size_t literalLength>
		ETInlineHint SystemString( const CharacterType (&stringLiteral)[literalLength], AllocatorType&& allocator = AllocatorType() );
		//!	Constructs this @ref SystemString instance.
		template <class AlternateAllocator>
		ETInlineHint SystemString( const ::Eldritch2::SystemString<AlternateAllocator>& string, AllocatorType&& allocator = AllocatorType() );
		//!	Constructs this @ref SystemString instance.
		ETInlineHint explicit SystemString( ::Eldritch2::SystemString<Allocator>&& sourceString );

		ETInlineHint ~SystemString() = default;
	};

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/Containers/SystemString.inl>
//------------------------------------------------------------------//