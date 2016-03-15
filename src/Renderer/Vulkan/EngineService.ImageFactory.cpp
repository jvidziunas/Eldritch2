/*==================================================================*\
  EngineService.ImageFactory.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/Vulkan/EngineService.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Configuration;
using namespace ::Eldritch2::FileSystem;
using namespace ::Eldritch2::Renderer;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {

	EngineService::ImageFactory::ImageFactory() : _devicePoolSize( 128u * 1024u * 1024u ), _devicePool( nullptr ), _stagingPool( nullptr ) {}

// ---------------------------------------------------

	void EngineService::ImageFactory::AcceptInitializationVisitor( ConfigurationPublishingInitializationVisitor& /*visitor*/ ) {
		// Empty for now.
	}

// ---------------------------------------------------

	ETNoAliasHint const UTF8Char* const EngineService::ImageFactory::GetSerializedDataTag() {
		return UTF8L("E2Image");
	}

// ---------------------------------------------------

	ErrorCode EngineService::ImageFactory::AllocateResourceView( Allocator& /*allocator*/, ContentLibrary& /*contentLibrary*/, ContentPackage& /*package*/, const UTF8Char* const /*name*/, const Range<const char*> /*sourceAsset*/ ) {
		return Error::UnsupportedOperation;
	}

// ---------------------------------------------------

	void EngineService::ImageFactory::CreateDeviceResources( DeviceContext& deviceContext ) {
		if( auto allocatePoolResult = deviceContext.AllocateMemory( _devicePoolSize, 0u ) ) {
			_devicePool = ::std::move( allocatePoolResult.object );
		}
	}

}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2



