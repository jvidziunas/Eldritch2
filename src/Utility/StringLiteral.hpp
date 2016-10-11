/*==================================================================*\
  StringLiteral.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/CharTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename Character>
	class StringLiteral {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref StringLiteral instance.
		template <size_t stringLength>
		StringLiteral( const Character (&literal)[stringLength] );
	//!	Constructs this @ref StringLiteral instance.
		explicit StringLiteral( const Character* const literal );
	//!	Constructs this @ref StringLiteral instance.
		StringLiteral();

		~StringLiteral() = default;

	// ---------------------------------------------------

	public:
		operator	const Character*() const;

	// - DATA MEMBERS ------------------------------------

	private:
		const Character*	_string;
	};

// ---

	using Utf8Literal	= Eldritch2::StringLiteral<Eldritch2::Utf8Char>;
	using WcharLiteral	= Eldritch2::StringLiteral<wchar_t>;

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/StringLiteral.inl>
//------------------------------------------------------------------//