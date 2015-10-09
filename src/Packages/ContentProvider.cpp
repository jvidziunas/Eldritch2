/*==================================================================*\
  ContentProvider.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Packages/ContentProvider.hpp>
#include <Utility/ErrorCode.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Configuration;
using namespace ::Eldritch2::FileSystem;
using namespace ::Eldritch2::Scheduler;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace FileSystem {

	ContentProvider::ContentProvider() {}

// ---------------------------------------------------

	ContentProvider::~ContentProvider() {}

// ---------------------------------------------------

	ErrorCode ContentProvider::RegisterConfigurableVariables( ConfigurationDatabase& ) {
		return Errors::NONE;
	}

}	// namespace Foundation
}	// namespace Eldritch2