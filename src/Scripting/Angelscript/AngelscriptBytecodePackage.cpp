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
	static const UTF8Char	propertyMetadataBucketAllocator[]	= UTF8L( "Angelscript Type Metadata Property Metadata Allocator" );

}	// anonymous namespace

namespace Eldritch2 {
namespace Scripting {

	AngelscriptBytecodePackage::TypeMetadata::TypeMetadata( Allocator& allocator ) : _methodMetadata( 0u, allocator, methodMetadataBucketAllocator ), _propertyMetadata( 0u, allocator, propertyMetadataBucketAllocator ) {}

// ---------------------------------------------------

	AngelscriptBytecodePackage::TypeMetadata::TypeMetadata() : _methodMetadata( 0u, NullAllocator::GetInstance(), methodMetadataBucketAllocator ), _propertyMetadata( 0u, NullAllocator::GetInstance(), propertyMetadataBucketAllocator ) {}

// ---------------------------------------------------

	AngelscriptBytecodePackage::AngelscriptBytecodePackage( unique_ptr<::asIScriptModule>&& ownedModule, Allocator& allocator ) : _module( move( ownedModule ) ),
																																  _typeMetadata( 0u, allocator, UTF8L("Angelscript Bytecode Package Type Metadata Allocator") ),
																																  _functionMetadata( 0u, allocator, UTF8L("Angelscript Bytecode Package Function Metadata Allocator") ) {
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

		using FunctionMetadataCollection	= decltype(_functionMetadata);
		using TypeMetadataCollection		= decltype(_typeMetadata);

	// ---

		auto&	typeMetadataAllocator( _typeMetadata.GetAllocator().GetParent() );
		auto	CreateTypeMetadata( [&typeMetadataAllocator]()->TypeMetadataCollection::ValueType&& { return { 0u, { typeMetadataAllocator } }; } );
		auto	CreateFunctionMetadata( []()->FunctionMetadataCollection::ValueType&& { return { 0u, {} }; } );

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
		class BytecodeSerializer {
		public:
			ETInlineHint BytecodeSerializer( ::asIScriptModule& module ) : _module( module ) {}

		// ---------------------------------------------------

			ETForceInlineHint bool Serialize( MessagePackWriter& writer ) {
				class BytecodeStream : public ::asIBinaryStream {
				public:
					ETInlineHint BytecodeStream( MessagePackWriter& writer ) : _writer( writer ), _writtenBytes( 0u ) {}

					void Read( void* /*destination*/, ::asUINT /*sizeInBytes*/ ) override sealed {
						// Purposely unimplemented.
						ETRuntimeAssert( false );
					}

					void Write( const void* source, ::asUINT sizeInBytes ) override sealed {
						_writer.WriteRaw( source, sizeInBytes );
						_writtenBytes += sizeInBytes;
					}

					bool Finalize() {
						return true;
					}

				private:
					MessagePackWriter&	_writer;
					::asUINT			_writtenBytes;
				};

			// ---

				BytecodeStream	stream( writer );

				return (0 <= _module.SaveByteCode( &stream )) ? stream.Finalize() : false;
			}

		// - DATA MEMBERS ------------------------------------

		private:
			::asIScriptModule&	_module;
		};

	// ---

		return writer( BytecodeSerializer( *_module ), MessagePackWriter::WrapMapContainer( _typeMetadata, [] () {} ), MessagePackWriter::WrapMapContainer( _functionMetadata, [] () {} ) );
	}

}	// namespace Scripting
}	// namespace Eldritch2