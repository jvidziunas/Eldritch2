/*==================================================================*\
  SteamTools.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2019 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {

	ETConstexpr ETPureFunctionHint Eldritch2::StringSpan AsCString(EP2PSessionError error) ETNoexceptHint {
		switch (error) {
		case k_EP2PSessionErrorNotRunningApp: return "user not running the same application";
		case k_EP2PSessionErrorNoRightsToApp: return "user does not have rights to use application";
		case k_EP2PSessionErrorDestinationNotLoggedIn: return "user not connected to Steam";
		case k_EP2PSessionErrorTimeout: return "session timeout/blocked connection";
		default: return "unspecified error";
		};
	}

}	// namespace Eldritch2
