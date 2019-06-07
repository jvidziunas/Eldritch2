/*==================================================================*\
  WorldEventClient.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Networking/WorldEventClient.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Networking {

	Result WorldEventClient::AcceptPacket(Span<const byte*> /*bytes*/) {
		return Result::Success;
	}

	// ---------------------------------------------------

	Result WorldEventClient::AcceptPlayerJoinRequest() {
		return Result::Success;
	}

	// ---------------------------------------------------

	Result WorldEventClient::AcceptPlayerLeaveRequest() {
		return Result::Success;
	}

}} // namespace Eldritch2::Networking
