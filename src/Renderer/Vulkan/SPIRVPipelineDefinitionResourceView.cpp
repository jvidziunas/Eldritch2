/*==================================================================*\
  SPIRVPipelineDefinitionResourceView.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/Vulkan/SPIRVPipelineDefinitionResourceView.hpp>
#include <Utility/Containers/Range.hpp>
#include <Utility/ErrorCode.hpp>
//------------------------------------------------------------------//
#include <Renderer/PipelineTypes_generated.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2::FileSystem;
using namespace ::Eldritch2::Renderer;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {

	SPIRVPipelineDefinitionResourceView::SPIRVPipelineDefinitionResourceView( ContentLibrary& owningLibrary, ContentPackage& package, const UTF8Char* const name, Allocator& allocator ) : ResourceView( owningLibrary, package, name, allocator ), _pipeline( nullptr ) {}

// ---------------------------------------------------

	ErrorCode SPIRVPipelineDefinitionResourceView::UpdateFromByteStream( const Range<const char*> /*bytes*/ ) {
		return Error::None;
	}

// ---------------------------------------------------

	ETNoAliasHint const UTF8Char* const SPIRVPipelineDefinitionResourceView::GetSerializedDataTag() {
		return UTF8L("SPIRVPipelineDefinition");
	}

}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2
