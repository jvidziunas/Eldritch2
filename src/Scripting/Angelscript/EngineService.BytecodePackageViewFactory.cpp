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
#include <microprofile/microprofile.h>
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

	ErrorCode EngineService::BytecodePackageViewFactory::AllocateResourceView( Allocator& allocator, ContentLibrary& contentLibrary, ContentPackage& package, const UTF8Char* const name, const Range<const char*> /*sourceAsset*/ ) {
		MICROPROFILE_SCOPEI( "Angelscript Virtual Machine", "Create bytecode package resource view", 0xAAAAAB );

		if( ModuleHandle newModule { _scriptEngine->GetModule( name, ::asGM_CREATE_IF_NOT_EXISTS ) } ) {
			return new(allocator, Allocator::AllocationDuration::Normal) BytecodePackageResourceView( ::std::move( newModule ), contentLibrary, package, name, allocator ) ? Error::None : Error::OutOfMemory;
		}

		return Error::InvalidParameter;
	}

// ---------------------------------------------------

	void EngineService::BytecodePackageViewFactory::AcceptInitializationVisitor( ConfigurationPublishingInitializationVisitor& /*visitor*/ ) {}

}	// namespace AngelScript
}	// namespace Scripting
}	// namespace Eldritch2