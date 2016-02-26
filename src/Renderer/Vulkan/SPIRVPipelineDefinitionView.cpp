/*==================================================================*\
  SPIRVPipelineDefinitionView.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/Vulkan/SPIRVPipelineDefinitionView.hpp>
#include <Utility/Containers/Range.hpp>
#include <Utility/ErrorCode.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::FileSystem;
using namespace ::Eldritch2::Renderer;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {

	SPIRVPipelineDefinitionView::SPIRVPipelineDefinitionView( ContentLibrary& owningLibrary, ContentPackage& package, const UTF8Char* const name, Allocator& allocator ) : ResourceView( owningLibrary, package, name, allocator ),
																																										   _pipelines( { allocator, UTF8L("SPIR-V Pipeline Definition Allocator") } ) {}

// ---------------------------------------------------

	ErrorCode SPIRVPipelineDefinitionView::UpdateFromByteStream( const Range<const char*> /*bytes*/ ) {
		return Error::None;
	}

}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2
