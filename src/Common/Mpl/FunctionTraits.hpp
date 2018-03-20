/*==================================================================*\
  FunctionTraits.hpp
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
#include <eastl/type_traits.h>
#include <eastl/tuple.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename Function>
	struct FunctionTraits;

// ---

	template <typename Return, typename... Arguments>
	struct FunctionTraits<Return ( Arguments... )> {
		enum : size_t {
			Arity = typename eastl::integral_constant<size_t, sizeof...( Arguments )>::type::value
		};

		template <size_t index>
		using ArgumentType	= typename eastl::tuple_element<index, eastl::tuple<Arguments...>>::type;
		using ReturnType	= Return;

		static const bool	isConstMethod = false;
	};

// ---

	template <typename Return, typename... Arguments>
	struct FunctionTraits<Return (*)( Arguments... )> : public FunctionTraits<Return ( Arguments... )> {
		using NativeSignatureType = Return (*)( Arguments... );
	};

#if ET_COMPILER_IS_MSVC
//	Specialization for SIMDCall functions.
	template <typename Return, typename... Arguments>
	struct FunctionTraits<Return (ETSimdCall*)( Arguments... )> : public FunctionTraits<Return ( Arguments... )> {
		using NativeSignatureType = Return (ETSimdCall*)( Arguments... );
	};
#endif

//	Specialization for member functions.
	template <typename Return, class Class, typename... Arguments>
	struct FunctionTraits<Return (Class::*)( Arguments... )> : public FunctionTraits<Return ( Arguments... )> {
		using NativeSignatureType	= Return (Class::*)( Arguments... );
		using ClassType				= Class;
	};

#if ET_COMPILER_IS_MSVC
//	Specialization for SIMDCall member functions.
	template <typename Return, class Class, typename... Arguments>
	struct FunctionTraits<Return (ETSimdCall Class::*)( Arguments... )> : public FunctionTraits<Return (Class::*)( Arguments... )> {
		using NativeSignatureType	= Return (ETSimdCall Class::*)( Arguments... );
		using ClassType				= Class;
	};
#endif

//	Specialization for const member functions.
	template <typename Return, class Class, typename... Arguments>
	struct FunctionTraits<Return (Class::*)( Arguments... ) const> : public FunctionTraits<Return (Class::*)( Arguments... )> {
		using NativeSignatureType	= Return (Class::*)( Arguments... ) const;
		using ClassType				= Class;

		static const bool	isConstMethod = true;
	};

#if ET_COMPILER_IS_MSVC
//	Specialization for SIMDCall const member functions.
	template <typename Return, class Class, typename... Arguments>
	struct FunctionTraits<Return (ETSimdCall Class::*)( Arguments... ) const> : public FunctionTraits<Return (Class::*)( Arguments... ) const> {
		using NativeSignatureType	= Return (ETSimdCall Class::*)( Arguments... ) const;
		using ClassType				= Class;

		static const bool	isConstMethod = true;
	};
#endif	// if ET_COMPILER_IS_MSVC

}	// namespace Eldritch2