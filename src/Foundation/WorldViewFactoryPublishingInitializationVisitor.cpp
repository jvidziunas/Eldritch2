/*==================================================================*\
  WorldViewFactoryPublishingInitializationVisitor.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Foundation/WorldViewFactoryPublishingInitializationVisitor.hpp>
#include <Foundation/GameEngine.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Foundation;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Foundation {

	WorldViewFactoryPublishingInitializationVisitor::WorldViewFactoryPublishingInitializationVisitor( GameEngine& engine ) : _engine( engine ) {}

// ---------------------------------------------------

	WorldViewFactoryPublishingInitializationVisitor& WorldViewFactoryPublishingInitializationVisitor::PublishFactory( void* const parameter, const size_t allocationHintInBytes, ErrorCode (*factoryFunction)( Allocator&, World&, void* ) ) {
		_engine._worldViewAllocationHintInBytes += allocationHintInBytes;
		_engine._worldViewFactories.PushBack( { factoryFunction, parameter } );

		return *this;
	}

}	// namespace Foundation
}	// namespace Eldritch2


