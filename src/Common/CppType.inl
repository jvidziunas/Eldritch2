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

ETConstexpr ETForceInlineHint CppType::CppType(TypeId id) ETNoexceptHint : _id(id) {}

// ---------------------------------------------------

ETConstexpr ETPureFunctionHint ETForceInlineHint size_t GetHashCode(const CppType& type, size_t seed) ETNoexceptHint {
	return size_t(type._id) + seed;
}

// ---------------------------------------------------

ETConstexpr ETPureFunctionHint ETForceInlineHint bool operator==(const CppType& lhs, const CppType& rhs) ETNoexceptHint {
	return lhs._id == rhs._id;
}

// ---------------------------------------------------

ETConstexpr ETPureFunctionHint ETForceInlineHint bool operator!=(const CppType& lhs, const CppType& rhs) ETNoexceptHint {
	return !(lhs == rhs);
}

// ---------------------------------------------------

ETConstexpr ETPureFunctionHint ETForceInlineHint bool operator<(const CppType& lhs, const CppType& rhs) ETNoexceptHint {
	return lhs._id < rhs._id;
}

// ---------------------------------------------------

ETConstexpr ETPureFunctionHint ETForceInlineHint bool operator<=(const CppType& lhs, const CppType& rhs) ETNoexceptHint {
	return !(rhs < lhs);
}

// ---------------------------------------------------

template <typename T>
ETConstexpr ETPureFunctionHint CppType GetType() ETNoexceptHint {
	ETConstexpr auto dummy([]() {});
	return CppType(dummy);
}

} // namespace Eldritch2
