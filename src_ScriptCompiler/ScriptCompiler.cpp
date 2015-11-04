/*==================================================================*\
  ScriptCompiler.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Memory/Win32HeapAllocator.hpp>
#include <FileSystem/SynchronousFileWriter.hpp>
#include <FlatBufferMetadataBuilderVisitor.hpp>
#include <Utility/Memory/InstanceDeleters.hpp>
#include <Tools/Win32/FileAccessorFactory.hpp>
#include <Utility/Containers/UTF8String.hpp>
#include <Utility/Containers/Range.hpp>
#include <Tools/Tool.hpp>
//------------------------------------------------------------------//
#include <angelscript/sdk/add_on/scriptbuilder/scriptbuilder.h>
//------------------------------------------------------------------//

//==================================================================//
// LIBRARIES
//==================================================================//
#if( ET_DEBUG_MODE_ENABLED && ET_PLATFORM_X86 && ET_PLATFORM_64BIT )
	ET_LINK_LIBRARY( "angelscript64d.lib" )
#elif( ET_PLATFORM_X86 && ET_PLATFORM_64BIT )
	ET_LINK_LIBRARY( "angelscript64.lib" )
#endif
//------------------------------------------------------------------//

using namespace ::Eldritch2;

namespace {

	class ScriptCompilerTool : public Win32GlobalHeapAllocator,
							   public Tools::ToolCRTPBase<ScriptCompilerTool>,
							   public Tools::Win32::FileAccessorFactory,
							   public ::CScriptBuilder {

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref ScriptCompilerTool instance.
		ScriptCompilerTool() : Win32GlobalHeapAllocator( UTF8L("Root Allocator") ), ToolCRTPBase<ScriptCompilerTool>( static_cast<Win32GlobalHeapAllocator&>(*this) ) {}

		//!	Destroys this @ref ScriptCompilerTool instance.
		~ScriptCompilerTool() {
			if( auto scriptModule = GetModule() ) {
				scriptModule->Discard();
			}

			if( engine ) {
				engine->ShutDownAndRelease();
			}
		}

	// ---------------------------------------------------

		template <class MetadataVisitor>
		int AcceptMetadataVisitor( MetadataVisitor&& visitor ) {
			visitor.BeginMetadataProcessing();

			// The built-in metadata parser uses really inconsistent indexing that additionally isn't useful across execution boundaries, so we need to get a little creative here.
			// Since type/function/property indices will remain the same within the module, we instead will loop through each exposed element in the module, check if any metadata has
			// been associated with the element, and then dispatch out to the visitor if appropriate.

			if( !typeMetadataMap.empty() ) {
				visitor.BeginTypeMetadataProcessing();

				for( uint32 index( 0u ), typeCount( GetModule()->GetObjectTypeCount() ); index != typeCount; ++index ) {
					auto	type( GetModule()->GetObjectTypeByIndex( index ) );
					int		typeID( type->GetTypeId() );

					for( uint32 methodIndex( 0u ), methodCount( type->GetMethodCount() ); index != methodCount; ++methodIndex ) {
						if( auto metadata = GetMetadataStringForTypeMethod( typeID, type->GetMethodByIndex( methodIndex ) ) ) {
							visitor.ProcessTypePropertyMetadata( methodIndex, metadata );
						}
					}

					for( uint32 propertyIndex( 0u ), propertyCount( type->GetPropertyCount() ); index != propertyCount; ++propertyIndex ) {
						if( auto metadata = GetMetadataStringForTypeProperty( typeID, static_cast<int>(propertyIndex) ) ) {
							visitor.ProcessTypePropertyMetadata( propertyIndex, metadata );
						}
					}

					if( auto metadata = GetMetadataStringForType( type->GetTypeId() ) ) {
						visitor.ProcessTypeMetadata( index, metadata );
					}
				}

				visitor.FinishTypeMetadataProcessing();
			}

			if( !funcMetadataMap.empty() ) {
				visitor.BeginFunctionMetadataProcessing();

				for( uint32 index( 0u ); index != GetModule()->GetFunctionCount(); ++index ) {
					auto	function( GetModule()->GetFunctionByIndex( index ) );

					if( auto metadata = GetMetadataStringForFunc( function ) ) {
						visitor.ProcessFunctionMetadata( index, metadata );
					}
				}

				visitor.FinishFunctionMetadataProcessing();
			}

			if( !varMetadataMap.empty() ) {
				visitor.BeginPropertyMetadataProcessing();

				for( uint32 index( 0u ); index != GetModule()->GetGlobalVarCount(); ++index ) {
					if( auto metadata = GetMetadataStringForVar( static_cast<int>(index) ) ) {
						visitor.ProcessFunctionMetadata( index, metadata );
					}
				}

				visitor.FinishPropertyMetadataProcessing();
			}

			return visitor.FinishMetadataProcessing();
		}

	// ---------------------------------------------------

		int ProcessInputFiles( Range<const SystemChar**> inputFiles ) {
			class Stream : public ::asIBinaryStream {
			// - CONSTRUCTOR/DESTRUCTOR --------------------------

			public:
				ETInlineHint Stream( InstancePointer<FileSystem::SynchronousFileWriter>&& writer ) : _writer( move( writer ) ) {}

			// ---------------------------------------------------

				void Read( void* /*destination*/, ::asUINT /*sizeInBytes*/ ) override {}

				void Write( const void* source, ::asUINT sizeInBytes ) override {
					_writer->Write( source, sizeInBytes );
				}

			// - DATA MEMBERS ------------------------------------

			private:
				InstancePointer<FileSystem::SynchronousFileWriter>	_writer;
			};

		// ---

			 if( auto createOutputFileResult = CreateWriter( *this, SL("asdf") ) ) {
				if( ::asSUCCESS != StartNewModule( ::asCreateScriptEngine(), "asdf" ) ) {
					return -1;
				}

				for( const SystemChar* fileName : inputFiles ) {
					// AddSectionFromMemory();
				}

				if( ::asSUCCESS == BuildModule() ) {
					Stream	stream( move( createOutputFileResult ) );

					return (::asSUCCESS == GetModule()->SaveByteCode( &stream )) ? AcceptMetadataVisitor( Tools::FlatBufferMetadataBuilderVisitor( stream, *this ) ) : -1;
				}
			}

			return -1;
		}

	// - DATA MEMBERS ------------------------------------

	private:
		::Eldritch2::UTF8String<>	_outputModuleName;
	};

}	// anonymous namespace

int main( int argc, SystemChar** argv ) {
	return ScriptCompilerTool().Run( { argv + 1, argv + argc } );
}