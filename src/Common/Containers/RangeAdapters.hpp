/*==================================================================*\
  RangeAdapters.hpp
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
//------------------------------------------------------------------//

namespace Eldritch2 {

template <typename Container>
auto begin(Container&& container) -> decltype(eastl::declval<Container>().Begin()) {
	return container.Begin();
}

// ---------------------------------------------------

template <typename Container>
auto begin(const Container& container) -> decltype(eastl::declval<const Container>().Begin()) {
	return container.Begin();
}

// ---------------------------------------------------

template <typename Container>
auto rbegin(Container&& container) -> decltype(eastl::declval<Container>().ReverseBegin()) {
	return container.Begin();
}

// ---------------------------------------------------

template <typename Container>
auto rbegin(const Container& container) -> decltype(eastl::declval<const Container>().ReverseBegin()) {
	return container.Begin();
}

// ---------------------------------------------------

template <typename Container>
auto cbegin(const Container& container) -> decltype(eastl::declval<const Container>().ConstBegin()) {
	return container.ConstBegin();
}

// ---------------------------------------------------

template <typename Container>
auto crbegin(Container&& container) -> decltype(eastl::declval<Container>().ConstReverseBegin()) {
	return container.Begin();
}

// ---------------------------------------------------

template <typename Container>
auto crbegin(const Container& container) -> decltype(eastl::declval<const Container>().ConstReverseBegin()) {
	return container.Begin();
}

// ---------------------------------------------------

template <typename Container>
auto end(Container&& container) -> decltype(eastl::declval<Container>().End()) {
	return container.End();
}

// ---------------------------------------------------

template <typename Container>
auto end(const Container& container) -> decltype(eastl::declval<const Container>().End()) {
	return container.End();
}

// ---------------------------------------------------

template <typename Container>
auto rend(Container&& container) -> decltype(eastl::declval<Container>().ReverseEnd()) {
	return container.End();
}

// ---------------------------------------------------

template <typename Container>
auto rend(const Container& container) -> decltype(eastl::declval<const Container>().ReverseEnd()) {
	return container.End();
}

// ---------------------------------------------------

template <typename Container>
auto cend(const Container& container) -> decltype(eastl::declval<const Container>().ConstEnd()) {
	return container.ConstEnd();
}

// ---------------------------------------------------

template <typename Container>
auto crend(Container&& container) -> decltype(eastl::declval<Container>().ConstReverseEnd()) {
	return container.End();
}

// ---------------------------------------------------

template <typename Container>
auto crend(const Container& container) -> decltype(eastl::declval<const Container>().ConstReverseEnd()) {
	return container.End();
}

} // namespace Eldritch2
