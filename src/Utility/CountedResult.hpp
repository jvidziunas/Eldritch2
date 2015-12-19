/*==================================================================*\
  CountedResult.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/ObjectHandle.hpp>
#include <Utility/ResultBase.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename ResultObject>
	using CountedResult	= Detail::ResultBase<Scripting::ObjectHandle<ResultObject>>;

}	// namespace Eldritch2