/*==================================================================*\
  PlayerCommon.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/CharTypes.hpp>
#include <Utility/MPL/IntTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Foundation {

	namespace Foundation	= ::Eldritch2::Foundation;

// ---------------------------------------------------

	struct MachineAddress {
		union {
			::Eldritch2::uint64			version6Address[2];

			struct {
				union {
					::Eldritch2::uint32	address;
					::Eldritch2::uint8	addressBytes[4u];
				};

				::Eldritch2::uint16		port;
			}							version4Address;
		};

		bool							isVersion6Address;
	};

	struct PlayerGUID {
		::Eldritch2::uint8	bytes[16u];
	};

}	// namespace Foundation
}	// namespace Eldritch2