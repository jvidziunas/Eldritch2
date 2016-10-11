/*==================================================================*\
  TypeHandle.inl
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

	ETInlineHint TypeHandle::TypeHandle( const std::type_info& typeInfo ) : _typeInfo( eastl::addressof(typeInfo) ) {}

// ---------------------------------------------------

	ETInlineHint const char* TypeHandle::GetName() const {
		return _typeInfo->name();
	}

// ---------------------------------------------------

	ETPureFunctionHint ETForceInlineHint size_t GetHashCode( const TypeHandle& typeHandle, size_t seed ) {
		return typeHandle._typeInfo->hash_code() + seed;
	}

// ---------------------------------------------------

	ETPureFunctionHint ETForceInlineHint bool operator==( const TypeHandle& left, const TypeHandle& right ) {
		return ( *left._typeInfo == *right._typeInfo );
	}

// ---------------------------------------------------

	ETPureFunctionHint ETForceInlineHint bool operator!=( const TypeHandle& left, const TypeHandle& right ) {
		return ( *left._typeInfo != *right._typeInfo );
	}

// ---------------------------------------------------

	ETPureFunctionHint ETForceInlineHint bool operator<( const TypeHandle& left, const TypeHandle& right ) {
		return left._typeInfo->before( *right._typeInfo );
	}

// ---------------------------------------------------

	ETPureFunctionHint ETForceInlineHint bool operator>=( const TypeHandle& left, const TypeHandle& right ) {
		return !( right < left );
	}

// ---------------------------------------------------

	ETPureFunctionHint ETForceInlineHint bool operator>( const TypeHandle& left, const TypeHandle& right ) {
		return (right < left);
	}

// ---------------------------------------------------

	ETPureFunctionHint ETForceInlineHint bool operator<=( const TypeHandle& left, const TypeHandle& right ) {
		return !( left < right );
	}

}	// namespace Eldritch2

