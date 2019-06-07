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
#include <Common/Mpl/TypeTraits.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

template <typename Container>
auto begin(Container&& container) ETNoexceptHintIf(HasNoThrowMemberBegin<Container>()) -> decltype(Declval<Container&>().Begin()) {
	return container.Begin();
}

// ---------------------------------------------------

template <typename Container>
auto begin(const Container& container) ETNoexceptHintIf(HasNoThrowMemberBegin<const Container>()) -> decltype(Declval<const Container&>().Begin()) {
	return container.Begin();
}

// ---------------------------------------------------

template <typename Container>
auto rbegin(Container&& container) -> decltype(Declval<Container&>().ReverseBegin()) {
	return container.ReverseBegin();
}

// ---------------------------------------------------

template <typename Container>
auto rbegin(const Container& container) -> decltype(Declval<const Container&>().ReverseBegin()) {
	return container.ReverseBegin();
}

// ---------------------------------------------------

template <typename Container>
auto cbegin(const Container& container) -> decltype(Declval<const Container&>().ConstBegin()) {
	return container.ConstBegin();
}

// ---------------------------------------------------

template <typename Container>
auto crbegin(const Container& container) -> decltype(Declval<const Container&>().ConstReverseBegin()) {
	return container.ConstReverseBegin();
}

// ---------------------------------------------------

template <typename Container>
auto end(Container&& container) ETNoexceptHintIf(HasNoThrowMemberEnd<Container>()) -> decltype(Declval<Container&>().End()) {
	return container.End();
}

// ---------------------------------------------------

template <typename Container>
auto end(const Container& container) ETNoexceptHintIf(HasNoThrowMemberEnd<const Container>()) -> decltype(Declval<const Container&>().End()) {
	return container.End();
}

// ---------------------------------------------------

template <typename Container>
auto rend(Container&& container) -> decltype(Declval<Container&>().ReverseEnd()) {
	return container.ReverseEnd();
}

// ---------------------------------------------------

template <typename Container>
auto rend(const Container& container) -> decltype(Declval<const Container&>().ReverseEnd()) {
	return container.ReverseEnd();
}

// ---------------------------------------------------

template <typename Container>
auto cend(const Container& container) -> decltype(Declval<const Container&>().ConstEnd()) {
	return container.ConstEnd();
}

// ---------------------------------------------------

template <typename Container>
auto crend(Container&& container) -> decltype(Declval<Container&>().ConstReverseEnd()) {
	return container.ConstReverseEnd();
}

// ---------------------------------------------------

template <typename Container>
auto crend(const Container& container) -> decltype(Declval<const Container&>().ConstReverseEnd()) {
	return container.ConstReverseEnd();
}

} // namespace Eldritch2
