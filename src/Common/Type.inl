/*==================================================================*\
  Type.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <EASTL/memory.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

	ETInlineHint Type::Type( const std::type_info& type ) : _type( eastl::addressof(type) ) {}

// ---------------------------------------------------

	ETInlineHint const char* Type::GetName() const {
		return _type->name();
	}

// ---------------------------------------------------

	ETPureFunctionHint ETInlineHint size_t GetHashCode( const Type& type, size_t seed ) {
		return type._type->hash_code() + seed;
	}

// ---------------------------------------------------

	ETPureFunctionHint ETInlineHint bool operator==( const Type& left, const Type& right ) {
		return ( *left._type == *right._type );
	}

// ---------------------------------------------------

	ETPureFunctionHint ETInlineHint bool operator!=( const Type& left, const Type& right ) {
		return ( *left._type != *right._type );
	}

// ---------------------------------------------------

	ETPureFunctionHint ETInlineHint bool operator<( const Type& left, const Type& right ) {
		return left._type->before( *right._type );
	}

// ---------------------------------------------------

	ETPureFunctionHint ETInlineHint bool operator>=( const Type& left, const Type& right ) {
		return !( right < left );
	}

// ---------------------------------------------------

	ETPureFunctionHint ETInlineHint bool operator>( const Type& left, const Type& right ) {
		return (right < left);
	}

// ---------------------------------------------------

	ETPureFunctionHint ETInlineHint bool operator<=( const Type& left, const Type& right ) {
		return !( left < right );
	}

}	// namespace Eldritch2