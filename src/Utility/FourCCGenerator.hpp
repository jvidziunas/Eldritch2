/*==================================================================*\
  FourCCGenerator.hpp
  ------------------------------------------------------------------
  Purpose:
  Utility object that can be used to generate FourCC codes used to
  identify data at compile time.

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/IntTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Utility {

	template <char fourCC0, char fourCC1, char fourCC2, char fourCC3>
	struct FourCCGenerator {
		enum : ::Eldritch2::uint32 {
			FourCC = ( static_cast<::Eldritch2::uint32>(fourCC0) | (static_cast<::Eldritch2::uint32>(fourCC1) << 8) | (static_cast<::Eldritch2::uint32>(fourCC2) << 16) | (static_cast<::Eldritch2::uint32>(fourCC3) << 24) )
		};
	};

// ---------------------------------------------------

	::Eldritch2::uint32	CStringToFourCC( const char* src );
	::Eldritch2::uint32	CStringToFourCC( const wchar_t* src );

}	// namespace Utility
}	// namespace Eldritch2