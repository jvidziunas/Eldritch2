/*==================================================================*\
  EngineService.MeshViewFactory.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/Direct3D11/MeshResourceView.hpp>
#include <Renderer/Direct3D11/EngineService.hpp>
#include <Utility/Memory/InstanceNew.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Configuration;
using namespace ::Eldritch2::FileSystem;
using namespace ::Eldritch2::Renderer;
using namespace ::Eldritch2::Utility;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Renderer {
namespace Direct3D11 {

	EngineService::MeshViewFactory::MeshViewFactory() : _device( nullptr ) {}

// ---------------------------------------------------

	void EngineService::MeshViewFactory::AcceptInitializationVisitor( ConfigurationPublishingInitializationVisitor& /*visitor*/ ) {
		// Nothing here for now.
	}

// ---------------------------------------------------

	Result<ResourceView> EngineService::MeshViewFactory::AllocateResourceView( Allocator& allocator, const UTF8Char* const name ) const {
		if( auto view = new(allocator, Allocator::AllocationDuration::Normal) MeshResourceView( name, allocator ) ) {
			return { ::std::move( view ) };
		}

		return{ Error::OutOfMemory };
	}

// ---------------------------------------------------

	void EngineService::MeshViewFactory::SetDevice( const COMPointer<::ID3D11Device>& device ) {
		_device = device;
	}

}	// namespace Direct3D11
}	// namespace Renderer
}	// namespace Eldritch2

