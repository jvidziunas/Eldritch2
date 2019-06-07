/*==================================================================*\
  CppType.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Mpl/Compiler.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

class CppType {
	// - TYPE PUBLISHING ---------------------------------

private:
	using TypeId = void(*)();

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref CppType instance.
	ETConstexpr CppType(const CppType&) ETNoexceptHint = default;
	//!	Constructs this @ref CppType instance.
	ETConstexpr CppType(TypeId) ETNoexceptHint;
	//!	Constructs this @ref CppType instance.
	ETConstexpr CppType() ETNoexceptHint;

	~CppType() = default;

	// - DATA MEMBERS ------------------------------------

private:
	TypeId _id;

	// ---------------------------------------------------

public:
	friend ETConstexpr ETPureFunctionHint size_t GetHashCode(const CppType&, size_t seed) ETNoexceptHint;

	friend ETConstexpr ETPureFunctionHint bool operator==(const CppType&, const CppType&) ETNoexceptHint;

	friend ETConstexpr ETPureFunctionHint bool operator!=(const CppType&, const CppType&) ETNoexceptHint;

	friend ETConstexpr ETPureFunctionHint bool operator<(const CppType&, const CppType&) ETNoexceptHint;

	friend ETConstexpr ETPureFunctionHint bool operator<=(const CppType&, const CppType&) ETNoexceptHint;

	template <typename T>
	friend ETConstexpr ETPureFunctionHint CppType GetType() ETNoexceptHint;
};

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/CppType.inl>
//------------------------------------------------------------------//
