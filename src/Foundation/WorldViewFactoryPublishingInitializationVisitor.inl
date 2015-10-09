/*==================================================================*\
  WorldViewFactoryPublishingInitializationVisitor.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Foundation {

	ETInlineHint Foundation::WorldViewFactoryPublishingInitializationVisitor& WorldViewFactoryPublishingInitializationVisitor::PublishFactory( const size_t allocationHintInBytes, ErrorCode (*factoryFunction)( ::Eldritch2::Allocator&, Foundation::World&, void* ) ) {
		return PublishFactory( nullptr, allocationHintInBytes, factoryFunction );
	}

}	// namespace Foundation
}	// namespace Eldritch2



