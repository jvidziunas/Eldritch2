/*==================================================================*\
  EngineService.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Core/ServiceBlackboard.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Core {

	ETInlineHint EngineService::EngineService( const Core::ServiceBlackboard& serviceBlackboard ) : _serviceLocator( serviceBlackboard ) {}

// ---------------------------------------------------

	template <typename ServiceType>
	ETInlineHint ServiceType& EngineService::LocateService() const {
		return _serviceLocator.Locate<ServiceType>();
	}

}	// namespace Core
}	// namespace Eldritch2