/*==================================================================*\
  AngelscriptBytecodePackage.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Angelscript/AngelscriptBytecodePackage.hpp>
#include <Utility/Memory/NullAllocator.hpp>
#include <Utility/MessagePackReader.hpp>
#include <Utility/MessagePackWriter.hpp>
#include <Utility/Memory/MemStdLib.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Utility;
using namespace ::Eldritch2;
using namespace ::std;

namespace {

	static const UTF8Char	methodMetadataBucketAllocator[]		= UTF8L("Angelscript Type Metadata Method Metadata Allocator");
	static const UTF8Char	propertyMetadataBucketAllocator[]	= UTF8L("Angelscript Type Metadata Property Metadata Allocator");

}	// anonymous namespace

namespace Eldritch2 {
namespace Scripting {

	AngelscriptBytecodePackage::TypeMetadata::TypeMetadata( Allocator& allocator ) : _methodMetadata( 0u, allocator, methodMetadataBucketAllocator ), _propertyMetadata( 0u, allocator, propertyMetadataBucketAllocator ) {}

// ---------------------------------------------------

	AngelscriptBytecodePackage::TypeMetadata::TypeMetadata() : _methodMetadata( 0u, NullAllocator::GetInstance(), methodMetadataBucketAllocator ), _propertyMetadata( 0u, NullAllocator::GetInstance(), propertyMetadataBucketAllocator ) {}

// ---------------------------------------------------

	AngelscriptBytecodePackage::AngelscriptBytecodePackage( unique_ptr<::asIScriptModule>&& ownedModule, Allocator& allocator ) : _module( move( ownedModule ) ),
																																  _rootAllocator( allocator, UTF8L("Angelscript Bytecode Package Metadata Root Allocator") ),
																																  _typeMetadata( 0u, _rootAllocator, UTF8L( "Angelscript Bytecode Package Type Metadata Allocator" ) ),
																																  _functionMetadata( 0u, _rootAllocator, UTF8L( "Angelscript Bytecode Package Function Metadata Allocator" ) ) {
		_module->SetUserData( this );
	}

// ---------------------------------------------------

	ETNoAliasHint const UTF8Char* const AngelscriptBytecodePackage::GetSerializedDataTag() {
		return UTF8L("AngelscriptBytecodePackage");
	}

// ---------------------------------------------------

	bool AngelscriptBytecodePackage::SerializeAndBindToModule( MessagePackReader& reader ) {
		if( !Serialize( reader ) ) {
			return false;
		}

		// Bind the deserialized metadata to the user data pointers in the native Angelscript type/function objects.
		for( auto& metadata : _typeMetadata ) {
			// Locate the object type in the module collection...
			if( ::asIObjectType* const type = _module->GetObjectTypeByIndex( metadata.first ) ) {
				// ... and set the corresponding user data.
				type->SetUserData( &metadata.second );
			} else {
				return false;
			}
		}

		for( auto& metadata : _functionMetadata ) {
			// Locate the function in the module collection...
			if( ::asIScriptFunction* const function = _module->GetFunctionByIndex( metadata.first ) ) {
				// ... and set the corresponding user data.
				function->SetUserData( &metadata.second );
			} else {
				return false;
			}
		}

		return true;
	}

}	// namespace Scripting
}	// namespace Eldritch2