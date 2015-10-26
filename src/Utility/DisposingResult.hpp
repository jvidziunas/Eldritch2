/*==================================================================*\
  DisposingResult.hpp
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
	using DisposingResult	= Detail::ResultBase<Scripting::ObjectHandle<ResultObject>>;

}	// namespace Eldritch2