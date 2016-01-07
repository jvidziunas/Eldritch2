/*==================================================================*\
  EngineService.ShaderResourceViewFactory.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Configuration/ConfigurationPublishingInitializationVisitor.hpp>
#include <Renderer/Direct3D11/ShaderResourceResourceView.hpp>
#include <Renderer/Direct3D11/EngineService.hpp>
#include <Utility/Memory/InstanceNew.hpp>
#include <Utility/Assert.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Configuration;
using namespace ::Eldritch2::FileSystem;
using namespace ::Eldritch2::Renderer;
using namespace ::Eldritch2::Utility;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Renderer {
namespace Direct3D11 {

	EngineService::ShaderResourceViewFactory::ShaderResourceViewFactory() : _device( nullptr ),
																			_MSAACount( 1u ),
																			_MSAAQuality( 0u ),
																			_adaptiveResolutionMaxAreaFraction( 1.0f ),
																			_adaptiveResolutionMinAreaFraction( 0.25f ) {}

// ---------------------------------------------------

	void EngineService::ShaderResourceViewFactory::AcceptInitializationVisitor( ConfigurationPublishingInitializationVisitor& visitor ) {
		visitor.Register( UTF8L("MSAASamplesPerPixel"), _MSAACount ).Register( UTF8L("MSAAQualityLevel"), _MSAAQuality )
			   .Register( UTF8L("MaximumAdaptiveResolutionScreenFraction"), _adaptiveResolutionMaxAreaFraction ).Register( UTF8L("MinimumAdaptiveResolutionScreenFraction"), _adaptiveResolutionMinAreaFraction );
	}

// ---------------------------------------------------

	ErrorCode EngineService::ShaderResourceViewFactory::AllocateResourceView( Allocator& allocator, ContentLibrary& contentLibrary, ContentPackage& package, const UTF8Char* const name, const Range<const char*> /*sourceAsset*/ ) {
		ETRuntimeAssert( _device );

	// ---

		return new(allocator, Allocator::AllocationOption::PERMANENT_ALLOCATION) ShaderResourceResourceView( contentLibrary, package, name, allocator ) ? Error::NONE : Error::OUT_OF_MEMORY;
	}

// ---------------------------------------------------

	void EngineService::ShaderResourceViewFactory::SetDevice( const COMPointer<::ID3D11Device>& device ) {
		_device = device;
	}

}	// namespace Direct3D11
}	// namespace Renderer
}	// namespace Eldritch2
