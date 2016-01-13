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
#include <Utility/Containers/String.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <class Allocator = ::Eldritch2::ChildAllocator>
	class UTF8String : public String<::Eldritch2::UTF8Char, Allocator> {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
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
		//!	Constructs this @ref UTF8String instance.
		ETInlineHint explicit UTF8String( const ::Eldritch2::UTF8String<Allocator>& sourceString ) = delete;

		ETInlineHint ~UTF8String() = default;
	};

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/Containers/UTF8String.inl>
//------------------------------------------------------------------//