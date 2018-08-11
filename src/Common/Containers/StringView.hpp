/*==================================================================*\
  StringView.hpp
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
#include <eastl/string_view.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

template <typename Character>
class StringView {
	// - TYPE PUBLISHING ---------------------------------

private:
	using UnderlyingView = eastl::basic_string_view<Character>;

public:
	using ConstIterator = const Character*;
	using CharacterType = Character;
	using SizeType      = size_t;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref StringView instance.
	ETConstexpr StringView(const CharacterType* begin, SizeType length);
	//!	Constructs this @ref StringView instance.
	template <size_t size>
	ETConstexpr StringView(const CharacterType (&string)[size]);
	//!	Constructs this @ref StringView instance.
	ETConstexpr StringView(const StringView&) = default;
	ETConstexpr StringView();

	ETConstexpr ~StringView() = default;

	// - ELEMENT ITERATION -------------------------------

public:
	ETConstexpr ConstIterator Begin() const;

	ETConstexpr ConstIterator End() const;

	ETConstexpr ConstIterator ConstBegin() const;

	ETConstexpr ConstIterator ConstEnd() const;

	// - SIZE QUERY --------------------------------------

public:
	ETConstexpr SizeType GetLength() const;

	// - DATA MEMBERS ------------------------------------

private:
	UnderlyingView _view;
};

} // namespace Eldritch2
