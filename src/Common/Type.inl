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

ETInlineHint Type::Type(const std::type_info& type) :
	_type(eastl::addressof(type)) {}

// ---------------------------------------------------

ETInlineHint const char* Type::GetName() const {
	return _type->name();
}

// ---------------------------------------------------

ETPureFunctionHint ETInlineHint size_t GetHashCode(const Type& type, size_t seed) {
	return type._type->hash_code() + seed;
}

// ---------------------------------------------------

ETPureFunctionHint ETInlineHint bool operator==(const Type& lhs, const Type& rhs) {
	return (*lhs._type == *rhs._type);
}

// ---------------------------------------------------

ETPureFunctionHint ETInlineHint bool operator!=(const Type& lhs, const Type& rhs) {
	return (*lhs._type != *rhs._type);
}

// ---------------------------------------------------

ETPureFunctionHint ETInlineHint bool operator<(const Type& lhs, const Type& rhs) {
	return lhs._type->before(*rhs._type);
}

// ---------------------------------------------------

ETPureFunctionHint ETInlineHint bool operator>=(const Type& lhs, const Type& rhs) {
	return !(rhs < lhs);
}

// ---------------------------------------------------

ETPureFunctionHint ETInlineHint bool operator>(const Type& lhs, const Type& rhs) {
	return (rhs < lhs);
}

// ---------------------------------------------------

ETPureFunctionHint ETInlineHint bool operator<=(const Type& lhs, const Type& rhs) {
	return !(lhs < rhs);
}

} // namespace Eldritch2
