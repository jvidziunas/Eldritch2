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
#include <Utility/MessagePackReader.hpp>
#include <Utility/MessagePackWriter.hpp>
#include <Utility/Memory/MemStdLib.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Utility;
using namespace ::Eldritch2;
using namespace ::std;

namespace Eldritch2 {
namespace Scripting {

	AngelscriptBytecodePackage::TypeMetadata::TypeMetadata( Allocator& allocator ) : _methodMetadata( 0u, ::rde::less<::asUINT>(), allocator, UTF8L("Angelscript Type Metadata Method Metadata Allocator") ),
																					 _propertyMetadata( 0u, ::rde::less<::asUINT>(), allocator, UTF8L("Angelscript Type Metadata Property Metadata Allocator") ) {}

// ---------------------------------------------------

	AngelscriptBytecodePackage::AngelscriptBytecodePackage( unique_ptr<::asIScriptModule>&& ownedModule, Allocator& allocator ) : _module( ::std::move( ownedModule ) ),
																																  _typeMetadata( 0u, ::rde::less<::asUINT>(), allocator, UTF8L("Angelscript Bytecode Package Type Metadata Allocator") ),
																																  _functionMetadata( 0u, ::rde::less<::asUINT>(), allocator, UTF8L("Angelscript Bytecode Package Function Metadata Allocator") ) {
		_module->SetUserData( this );
	}

// ---------------------------------------------------

	ETNoAliasHint const UTF8Char* const AngelscriptBytecodePackage::GetSerializedDataTag() {
		return UTF8L("AngelscriptBytecodePackage");
	}

// ---------------------------------------------------

	bool AngelscriptBytecodePackage::Serialize( MessagePackReader& reader ) {
		class BytecodeStream : public MessagePackReader::InPlaceBulkData, public ::asIBinaryStream {
		public:
			ETInlineHint BytecodeStream( ::asIScriptModule& module ) : _module( module ) {}

		// ---------------------------------------------------

			void Read( void* destination, ::asUINT sizeInBytes ) override sealed {
				const ::asUINT	copyLength( Min<::asUINT>( sizeInBytes, Size() ) );

				// Simple copy of mapped memory into the destination buffer, zeroing out any bytes we lack source data for.
				CopyMemory( destination, first, copyLength );
				ZeroMemory( static_cast<char*>(destination)+copyLength, (sizeInBytes - copyLength) );

				// Update the read pointer.
				first += copyLength;
			}

			void Write( const void* /*source*/, ::asUINT /*sizeInBytes*/ ) override sealed {
				// Purposely unimplemented.
				ETRuntimeAssert( false );
			}

		// ---------------------------------------------------

			ETForceInlineHint bool Serialize( MessagePackReader& reader ) {
				return reader( static_cast<MessagePackReader::InPlaceBulkData&>(*this) ) && (0 <= _module.LoadByteCode( this ));
			}

		// - DATA MEMBERS ------------------------------------

		private:
			::asIScriptModule&	_module;
		};

	// ---

		auto&	typeMetadataAllocator( _typeMetadata.GetAllocator().GetParent() );
		auto	CreateTypeMetadata( [&typeMetadataAllocator] () -> TypeMetadata&& { return { typeMetadataAllocator }; } );
		auto	CreateFunctionMetadata( [] () -> FunctionMetadata&& { return {}; } );

		if( reader( BytecodeStream( *_module ), MessagePackReader::WrapMapContainer( _typeMetadata, CreateTypeMetadata ), MessagePackReader::WrapMapContainer( _functionMetadata, CreateFunctionMetadata ) ) ) {
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
		} else {
			return false;
		}

		return true;
	}

// ---------------------------------------------------

	bool AngelscriptBytecodePackage::Serialize( MessagePackWriter& writer ) {
		class BytecodeStream : public ::asIBinaryStream {
		public:
			ETInlineHint BytecodeStream( ::asIScriptModule& module ) : _module( module ) {}

		// ---------------------------------------------------

			void Read( void* /*destination*/, ::asUINT /*sizeInBytes*/ ) override sealed {
				// Purposely unimplemented.
				ETRuntimeAssert( false );
			}

			void Write( const void* source, ::asUINT sizeInBytes ) override sealed {
			}

		// ---------------------------------------------------

			ETForceInlineHint bool Serialize( MessagePackWriter& reader ) {
				
			}

		// - DATA MEMBERS ------------------------------------

		private:
			::asIScriptModule&	_module;
		};

	// ---

		return writer( BytecodeStream( *_module ), MessagePackWriter::WrapMapContainer( _typeMetadata, [] () {} ), MessagePackWriter::WrapMapContainer( _functionMetadata, [] () {} ) );
	}

}	// namespace Scripting
}	// namespace Eldritch2