/*==================================================================*\
  INIParser.hpp
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
	template <typename Iterator>
	class	Range;
}

namespace Eldritch2 {
namespace Configuration {

	template <typename SettingHandler>
	void ParseINI( const ::Eldritch2::Range<const ::Eldritch2::UTF8Char*>& sourceData, SettingHandler settingHandler );

}	// namespace Configuration
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Configuration/INIParser.inl>
//------------------------------------------------------------------//