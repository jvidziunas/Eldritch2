/*==================================================================*\
  ResourceViewFactory.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Packages/ResourceViewFactory.hpp>
#include <Utility/ErrorCode.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2;

namespace Eldritch2 {
namespace FileSystem {

	ErrorCode ResourceViewFactory::AllocateEngineResources( Allocator& /*allocator*/, ContentLibrary& /*contentLibrary*/, ContentPackage& /*package*/ ) {
		// Default implementation does nothing.
		return Error::NONE;
	}

}	// namespace FileSystem
}	// namespace Eldritch2