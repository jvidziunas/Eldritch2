/*==================================================================*\
  EngineService.ObjectGraphViewFactory.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/AngelScript/ObjectGraphResourceView.hpp>
#include <Scripting/Angelscript/EngineService.hpp>
#include <Utility/Memory/InstanceNew.hpp>
#include <Utility/Containers/Range.hpp>
//------------------------------------------------------------------//
#include <microprofile/microprofile.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Configuration;
using namespace ::Eldritch2::FileSystem;
using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Scripting {
namespace AngelScript {

	ErrorCode EngineService::ObjectGraphViewFactory::AllocateResourceView( Allocator& allocator, ContentLibrary& contentLibrary, ContentPackage& package, const UTF8Char* const name, const Range<const char*> /*sourceAsset*/ ) {
		MICROPROFILE_SCOPEI( "Angelscript Virtual Machine", "Create object graph resource view", 0xAABBBB );

		return new(allocator, Allocator::AllocationDuration::Normal) ObjectGraphResourceView( contentLibrary, package, name, allocator ) ? Error::None : Error::OutOfMemory;
	}

// ---------------------------------------------------
		
	void EngineService::ObjectGraphViewFactory::AcceptInitializationVisitor( ConfigurationPublishingInitializationVisitor& /*visitor*/ ) {}

}	// namespace AngelScript
}	// namespace Scripting
}	// namespace Eldritch2