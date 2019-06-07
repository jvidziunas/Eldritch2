/*==================================================================*\
  TypeTraits.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Mpl/Compiler.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

template <bool Condition, typename TrueResult, typename FalseResult>
struct Conditional;
template <typename TrueResult, typename FalseResult>
struct Conditional<false, TrueResult, FalseResult> : public Identity<FalseResult> {};
template <typename TrueResult, typename FalseResult>
struct Conditional<true, TrueResult, FalseResult> : public Identity<TrueResult> {};

template <typename T, typename U>
struct IsSame : public FalseType {};
template <typename T>
struct IsSame<T, T> : public TrueType {};

template <typename T, typename U>
struct IsDistinct : public TrueType {};
template <typename T>
struct IsDistinct<T, T> : public FalseType {};

template <typename T>
struct IsEmpty : public IntegralConstant<bool, __is_empty(T)> {};

template <typename T>
struct IsStandardLayout : public IntegralConstant<bool, __is_standard_layout(T)> {};

template <typename T>
struct IsArray : public FalseType {};
template <typename T>
struct IsArray<T[]> : public TrueType {};
template <typename T, size_t ArrayBound>
struct IsArray<T[ArrayBound]> : public TrueType {};

template <typename T>
struct IsClass : public IntegralConstant<bool, __is_class(T)> {};

template <typename T>
struct IsUnion : public IntegralConstant<bool, __is_union(T)> {};

template <typename T>
struct IsTrivial : public IntegralConstant<bool, __is_trivial(T)> {};

template <typename T, typename... U>
struct IsConstructible : public IntegralConstant<bool, __is_constructible(T, U...)> {};

template <typename T>
struct IsDefaultConstructible : public IsConstructible<T> {};

template <typename T, typename... U>
struct IsTriviallyConstructible : public IntegralConstant<bool, __is_trivially_constructible(T, U...)> {};

template <typename T>
struct IsCopyConstructible : public IsConstructible<T, const T&> {};

template <typename T>
struct IsTriviallyCopyConstructible : public IsTriviallyConstructible<T, const T&> {};

template <typename T>
struct IsMoveConstructible : public IsConstructible<T, T&&> {};

template <typename T>
struct IsTriviallyMoveConstructible : public IsTriviallyConstructible<T, T&&> {};

template <typename T, typename... U>
struct IsNoThrowConstructible : public IntegralConstant<bool, __is_nothrow_constructible(T, U...)> {};

template <typename T>
struct IsNoThrowCopyConstructible : public IsNoThrowConstructible<T, const T&> {};

template <typename T>
struct IsNoThrowMoveConstructible : public IsNoThrowConstructible<T, T&&> {};

template <typename T, typename U>
struct IsAssignable : public IntegralConstant<bool, __is_assignable(T, U)> {};

template <typename T>
struct IsCopyAssignable : public IsAssignable<T, const T&> {};

template <typename T>
struct IsMoveAssignable : public IsAssignable<T, T&&> {};

template <typename T, typename U>
struct IsTriviallyAssignable : public IntegralConstant<bool, __is_trivially_assignable(T, U)> {};

template <typename T, typename U>
struct IsNoThrowAssignable : public IntegralConstant<bool, __is_nothrow_assignable(T, U)> {};

template <typename T>
struct IsNoThrowCopyAssignable : public IsNoThrowAssignable<T, const T&> {};

template <typename T>
struct IsNoThrowMoveAssignable : public IsNoThrowAssignable<T, T&&> {};

template <typename T>
struct IsNoThrowAddRef : public IntegralConstant<bool, noexcept(Declval<T&>().AddRef())> {};

template <typename T>
struct IsNoThrowRelease : public IntegralConstant<bool, noexcept(Declval<T&>().Release())> {};

template <typename T, typename... U>
struct IsNoThrowBindResources : public IntegralConstant<bool, noexcept(Declval<T&>().BindResources(Declval<U>()...))> {};

template <typename T, typename... U>
struct IsNoThrowFreeResources : public IntegralConstant<bool, noexcept(Declval<T&>().FreeResources(Declval<U>()...))> {};

template <typename T>
struct IsNoThrowSwappable : public IntegralConstant<bool, noexcept(Swap(Declval<T&>(), Declval<T&>()))> {};

template <typename, typename = Void_t<>>
struct HasMemberBegin : public FalseType {};
template <typename T>
struct HasMemberBegin<T, Void_t<decltype(Declval<T&>().Begin())>> : public TrueType {};

template <typename, typename = Void_t<>>
struct HasMemberConstBegin : public FalseType {};
template <typename T>
struct HasMemberConstBegin<T, Void_t<decltype(Declval<T&>().ConstBegin())>> : public TrueType {};

template <typename, typename = Void_t<>>
struct HasMemberEnd : public FalseType {};
template <typename T>
struct HasMemberEnd<T, Void_t<decltype(Declval<T&>().End())>> : public TrueType {};

template <typename, typename = Void_t<>>
struct HasMemberConstEnd : public FalseType {};
template <typename T>
struct HasMemberConstEnd<T, Void_t<decltype(Declval<T&>().ConstEnd())>> : public TrueType {};

template <typename T>
struct HasNoThrowMemberBegin : public IntegralConstant<bool, noexcept(Declval<T&>().Begin())> {};
template <typename T>
struct HasNoThrowMemberEnd : public IntegralConstant<bool, noexcept(Declval<T&>().End())> {};

template <typename T, typename... Args>
struct IsNoThrowCallable : public IntegralConstant<bool, noexcept(Declval<T&>()(Declval<Args>()...))> {};

template <typename T, typename U>
struct DifferenceType : public Identity<decltype(Declval<T&>() - Declval<U&>())> {};

template <typename T, typename U>
struct SumType : public Identity<decltype(Declval<T&>() + Declval<U&>())> {};

template <typename T>
struct DereferenceType : public Identity<decltype(*Declval<T&>())> {};

} // namespace Eldritch2
