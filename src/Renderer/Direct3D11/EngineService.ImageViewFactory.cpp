/*==================================================================*\
  EngineService.ImageViewFactory.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Configuration/ConfigurationPublishingInitializationVisitor.hpp>
#include <Renderer/Direct3D11/ImageResourceView.hpp>
#include <Renderer/Direct3D11/EngineService.hpp>
#include <Utility/Memory/InstanceNew.hpp>
#include <Utility/Assert.hpp>
//------------------------------------------------------------------//
#include <microprofile/microprofile.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Configuration;
using namespace ::Eldritch2::FileSystem;
using namespace ::Eldritch2::Renderer;
using namespace ::Eldritch2::Utility;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Renderer {
namespace Direct3D11 {

	EngineService::ImageViewFactory::ImageViewFactory() : _device( nullptr ),
														  _MsaaCount( 1u ),
														  _MsaaQuality( 0u ),
														  _adaptiveResolutionMaxAreaFraction( 1.0f ),
														  _adaptiveResolutionMinAreaFraction( 0.25f ) {}

// ---------------------------------------------------

	void EngineService::ImageViewFactory::AcceptInitializationVisitor( ConfigurationPublishingInitializationVisitor& visitor ) {
		visitor.Register( UTF8L("MsaaSamplesPerPixel"), _MsaaCount ).Register( UTF8L("MsaaQualityLevel"), _MsaaQuality )
			   .Register( UTF8L("MaximumAdaptiveResolutionScreenFraction"), _adaptiveResolutionMaxAreaFraction ).Register( UTF8L("MinimumAdaptiveResolutionScreenFraction"), _adaptiveResolutionMinAreaFraction );
	}

// ---------------------------------------------------

	ErrorCode EngineService::ImageViewFactory::AllocateResourceView( Allocator& allocator, ContentLibrary& contentLibrary, ContentPackage& package, const UTF8Char* const name, const Range<const char*> /*sourceAsset*/ ) {
		ETRuntimeAssert( _device );

	// ---

		MICROPROFILE_SCOPEI( "Direct3D11 Renderer", "Create shader resource view", 0xAAAAAA );

		return new(allocator, Allocator::AllocationDuration::Normal) ImageResourceView( contentLibrary, package, name, allocator ) ? Error::None : Error::OutOfMemory;
	}

// ---------------------------------------------------

	void EngineService::ImageViewFactory::SetDevice( const COMPointer<::ID3D11Device>& device ) {
		_device = device;
	}

}	// namespace Direct3D11
}	// namespace Renderer
}	// namespace Eldritch2
