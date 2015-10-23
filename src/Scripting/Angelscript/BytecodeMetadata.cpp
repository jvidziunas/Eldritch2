/*==================================================================*\
  BytecodeMetadata.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Angelscript/BytecodeMetadata.hpp>
#include <Utility/Memory/NullAllocator.hpp>
//------------------------------------------------------------------//
#include <angelscript.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Utility;
using namespace ::Eldritch2;
using namespace ::std;

namespace Eldritch2 {
namespace Scripting {
namespace AngelScript {

	BytecodeMetadata::TypeMetadata::TypeMetadata( Allocator& allocator ) : _methodMetadata( { allocator, UTF8L("Angelscript Type Metadata Method Metadata Allocator") } ), _propertyMetadata( { allocator, UTF8L("Angelscript Type Metadata Property Metadata Allocator") } ) {}

// ---------------------------------------------------

	BytecodeMetadata::TypeMetadata::TypeMetadata() : TypeMetadata( NullAllocator::GetInstance() ) {}

// ---------------------------------------------------

	BytecodeMetadata::BytecodeMetadata( Allocator& allocator ) : _rootAllocator( { allocator, UTF8L("Angelscript Bytecode Package Metadata Root Allocator") } ),
																 _typeMetadata( { _rootAllocator, UTF8L("Angelscript Bytecode Package Type Metadata Allocator") } ),
																 _functionMetadata( { _rootAllocator, UTF8L("Angelscript Bytecode Package Function Metadata Allocator") } ) {}

// ---------------------------------------------------

	bool BytecodeMetadata::BindToModule( ::asIScriptModule& module ) {
		module.SetUserData( this );

		// Bind the deserialized metadata to the user data pointers in the native Angelscript type/function objects.
		for( auto& metadata : _typeMetadata ) {
			// Locate the object type in the module collection...
			if( ::asIObjectType* const type = module.GetObjectTypeByIndex( metadata.first ) ) {
				// ... and set the corresponding user data.
				type->SetUserData( &metadata.second );
			} else {
				return false;
			}
		}

		for( auto& metadata : _functionMetadata ) {
			// Locate the function in the module collection...
			if( ::asIScriptFunction* const function = module.GetFunctionByIndex( metadata.first ) ) {
				// ... and set the corresponding user data.
				function->SetUserData( &metadata.second );
			} else {
				return false;
			}
		}

		return true;
	}

}	// namespace AngelScript
}	// namespace Scripting
}	// namespace Eldritch2