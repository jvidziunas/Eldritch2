/*==================================================================*\
  EngineService.BytecodePackageViewFactory.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/AngelScript/BytecodePackageResourceView.hpp>
#include <Scripting/Angelscript/EngineService.hpp>
#include <Utility/Memory/InstanceNew.hpp>
//------------------------------------------------------------------//
#include <angelscript.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Configuration;
using namespace ::Eldritch2::FileSystem;
using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Scripting {
namespace AngelScript {

	EngineService::BytecodePackageViewFactory::BytecodePackageViewFactory() : _scriptEngine( nullptr ) {}

// ---------------------------------------------------

	Result<ResourceView> EngineService::BytecodePackageViewFactory::AllocateResourceView( Allocator& allocator, const UTF8Char* const name ) const {
		ModuleHandle	module( _scriptEngine->GetModule( name, asEGMFlags::asGM_CREATE_IF_NOT_EXISTS ) );

		if( !module ) {
			return{ Error::Unspecified };
		}

		if( auto view = new(allocator, Allocator::AllocationDuration::Normal) BytecodePackageResourceView( name, ::std::move( module ), allocator ) ) {
			return { ::std::move( view ) };
		}

		return { Error::OutOfMemory };
	}

// ---------------------------------------------------

	void EngineService::BytecodePackageViewFactory::AcceptInitializationVisitor( ConfigurationPublishingInitializationVisitor& /*visitor*/ ) {}

}	// namespace AngelScript
}	// namespace Scripting
}	// namespace Eldritch2