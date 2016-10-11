/*==================================================================*\
  IniParserMixin.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
	template <typename>
	class	Range;
}

namespace Eldritch2 {
namespace Configuration {

	template <typename Database>
	class IniParserMixin : public Database {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref IniParserMixin instance.
		template <typename... Arguments>
		IniParserMixin( Arguments&&... arguments );

		~IniParserMixin() = default;

	// ---------------------------------------------------

		void	ApplyIni( Eldritch2::Range<const Eldritch2::Utf8Char*> sourceData );
	};

}	// namespace Configuration
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Configuration/IniParserMixin.inl>
//------------------------------------------------------------------//