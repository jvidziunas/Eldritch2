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

//------------------------------------------------------------------//

namespace Eldritch2 {

ETInlineHint ETForceInlineHint CppType::CppType(const std::type_info& type) :
	_type(ETAddressOf(type)) {}

// ---------------------------------------------------

ETInlineHint ETForceInlineHint const char* CppType::GetName() const {
	return _type->name();
}

// ---------------------------------------------------

ETInlineHint ETPureFunctionHint ETForceInlineHint size_t GetHashCode(const CppType& type, size_t seed) ETNoexceptHint {
	return type._type->hash_code() + seed;
}

// ---------------------------------------------------

ETInlineHint ETPureFunctionHint ETForceInlineHint bool operator==(const CppType& lhs, const CppType& rhs) ETNoexceptHint {
	return (*lhs._type == *rhs._type);
}

// ---------------------------------------------------

ETInlineHint ETPureFunctionHint ETForceInlineHint bool operator!=(const CppType& lhs, const CppType& rhs) ETNoexceptHint {
	return !(lhs == rhs);
}

// ---------------------------------------------------

ETInlineHint ETPureFunctionHint ETForceInlineHint bool operator<(const CppType& lhs, const CppType& rhs) ETNoexceptHint {
	return lhs._type->before(*rhs._type);
}

// ---------------------------------------------------

ETInlineHint ETPureFunctionHint ETForceInlineHint bool operator<=(const CppType& lhs, const CppType& rhs) ETNoexceptHint {
	return !(rhs < lhs);
}

} // namespace Eldritch2
