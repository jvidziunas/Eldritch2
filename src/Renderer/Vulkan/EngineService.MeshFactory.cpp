/*==================================================================*\
  EngineService.MeshFactory.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/Vulkan/MeshResourceView.hpp>
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

	EngineService::MeshFactory::MeshFactory() : _devicePool( nullptr ), _devicePoolSize( 128u * 1024u * 1024u ) {}

// ---------------------------------------------------

	void EngineService::MeshFactory::AcceptInitializationVisitor( ConfigurationPublishingInitializationVisitor& /*visitor*/ ) {
		// Empty for now.
	}

// ---------------------------------------------------

	ETNoAliasHint const UTF8Char* const EngineService::MeshFactory::GetSerializedDataTag() {
		return MeshResourceView::GetSerializedDataTag();
	}

// ---------------------------------------------------

	ErrorCode EngineService::MeshFactory::AllocateResourceView( Allocator& allocator, ContentLibrary& contentLibrary, ContentPackage& package, const UTF8Char* const name, const Range<const char*> sourceAsset ) {
		if( const auto view = new(allocator, Allocator::AllocationDuration::Normal) MeshResourceView( contentLibrary, package, name, allocator ) ) {
			return view->UpdateFromByteStream( sourceAsset );
		}

		return Error::None;
	}

// ---------------------------------------------------

	void EngineService::MeshFactory::CreateDeviceResources( DeviceContext& deviceContext ) {
		if( auto allocatePoolResult = deviceContext.AllocateMemory( _devicePoolSize, 0u ) ) {
			_devicePool = ::std::move( allocatePoolResult.object );
		}
	}

}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2



