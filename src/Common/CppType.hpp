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
#include <typeinfo>
//------------------------------------------------------------------//

namespace Eldritch2 {

class CppType {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref CppType instance.
	CppType(const std::type_info& type);
	//!	Constructs this @ref CppType instance.
	CppType(const CppType&) = default;
	//!	Constructs this @ref CppType instance.
	CppType();

	~CppType() = default;

	// ---------------------------------------------------

public:
	const char* GetName() const;

	// - DATA MEMBERS ------------------------------------

private:
	const std::type_info* _type;

	// ---------------------------------------------------

public:
	friend ETPureFunctionHint size_t GetHashCode(const CppType& type, size_t seed) ETNoexceptHint;

	friend ETPureFunctionHint bool operator==(const CppType& left, const CppType& right) ETNoexceptHint;

	friend ETPureFunctionHint bool operator!=(const CppType& left, const CppType& right) ETNoexceptHint;

	friend ETPureFunctionHint bool operator<(const CppType& left, const CppType& right) ETNoexceptHint;

	friend ETPureFunctionHint bool operator<=(const CppType& left, const CppType& right) ETNoexceptHint;
};

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/CppType.inl>
//------------------------------------------------------------------//
