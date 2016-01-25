/*==================================================================*\
  ContentProvider.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <FileSystem/ContentProvider.hpp>
#include <Utility/ErrorCode.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Configuration;
using namespace ::Eldritch2::FileSystem;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace FileSystem {

	ErrorCode ContentProvider::RegisterConfigurableVariables( ConfigurationDatabase& /*database*/ ) {
		return Error::None;
	}

}	// namespace Foundation
}	// namespace Eldritch2