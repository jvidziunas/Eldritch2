/*==================================================================*\
  WorldService.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Core {

	ETInlineHint WorldService::WorldService( const Core::ServiceBlackboard& blackboard ) : _blackboard( blackboard ) {}

// ---------------------------------------------------

	template <typename ServiceType>
	ETInlineHint ServiceType& WorldService::LocateService() const {
		return _blackboard.Locate<ServiceType>();
	}

}	// namespace Core
}	// namespace Eldritch2


