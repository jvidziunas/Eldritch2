/*==================================================================*\
  EngineService.PipelineFactory.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/Vulkan/SPIRVPipelineDefinitionResourceView.hpp>
#include <Utility/Memory/StandardLibrary.hpp>
#include <Renderer/Vulkan/EngineService.hpp>
#include <Utility/Memory/InstanceNew.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Configuration;
using namespace ::Eldritch2::FileSystem;
using namespace ::Eldritch2::Renderer;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {

	EngineService::PipelineFactory::PipelineFactory() {}

// ---------------------------------------------------

	void EngineService::PipelineFactory::AcceptInitializationVisitor( ConfigurationPublishingInitializationVisitor& /*visitor*/ ) {
		// Empty for now.
	}

// ---------------------------------------------------

	ETNoAliasHint const UTF8Char* const EngineService::PipelineFactory::GetSerializedDataTag() {
		return SPIRVPipelineDefinitionResourceView::GetSerializedDataTag();
	}

// ---------------------------------------------------

	ErrorCode EngineService::PipelineFactory::AllocateResourceView( Allocator& allocator, ContentLibrary& contentLibrary, ContentPackage& package, const UTF8Char* const name, const Range<const char*>	sourceAsset ) {
		if( const auto view = new(allocator, Allocator::AllocationDuration::Normal) SPIRVPipelineDefinitionResourceView( contentLibrary, package, name, allocator ) ) {
			return view->UpdateFromByteStream( sourceAsset );
		}

		return Error::OutOfMemory;
	}

}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2