/*==================================================================*\
  ScriptCompiler.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <FlatBufferMetadataBuilderVisitor.hpp>
#include <FileSystem/SynchronousFileWriter.hpp>
#include <Utility/Memory/InstanceDeleters.hpp>
#include <Utility/Containers/Range.hpp>
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

namespace Eldritch2 {
namespace Tools {

	template <class GlobalAllocator, class FileAccessorFactory>
	ScriptCompilerTool<GlobalAllocator, FileAccessorFactory>::ScriptCompilerTool() : AllocatorType( UTF8L("Root Allocator") ), _outputModuleName( { GetAllocator(), UTF8L("Output Module Name String Allocator") } ) {}

// ---------------------------------------------------

	template <class GlobalAllocator, class FileAccessorFactory>
	ETInlineHint typename ScriptCompilerTool<GlobalAllocator, FileAccessorFactory>::AllocatorType& ScriptCompilerTool<GlobalAllocator, FileAccessorFactory>::GetAllocator() {
		return static_cast<AllocatorType&>(*this);
	}

// ---------------------------------------------------

	template <class GlobalAllocator, class FileAccessorFactory>
	ScriptCompilerTool<GlobalAllocator, FileAccessorFactory>::~ScriptCompilerTool() {
		if( auto scriptModule = GetModule() ) {
			scriptModule->Discard();
		}

		if( engine ) {
			engine->ShutDownAndRelease();
		}
	}

// ---------------------------------------------------

	template <class GlobalAllocator, class FileAccessorFactory>
	template <class MetadataVisitor>
	int ScriptCompilerTool<GlobalAllocator, FileAccessorFactory>::AcceptMetadataVisitor( MetadataVisitor&& visitor ) {
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

	template <class GlobalAllocator, class FileAccessorFactory>
	int ScriptCompilerTool<GlobalAllocator, FileAccessorFactory>::ProcessInputFiles( const ::Eldritch2::Range<const ::Eldritch2::SystemChar**> inputFiles ) {
		class OutputStream : public ::asIBinaryStream {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref OutputStream instance.
			ETInlineHint OutputStream( ::Eldritch2::InstancePointer<FileSystem::SynchronousFileWriter>&& writer ) : writer( move( writer ) ) {}

			~OutputStream() = default;

		// ---------------------------------------------------

			void Read( void* /*destination*/, ::asUINT /*sizeInBytes*/ ) override {
				// This space intentionally left blank.
			}

			void Write( const void* source, ::asUINT sizeInBytes ) override {
				writer->Write( source, sizeInBytes );
			}

		// - DATA MEMBERS ------------------------------------

			::Eldritch2::InstancePointer<FileSystem::SynchronousFileWriter>	writer;
		};

	// ---

		OutputStream	stream( CreateWriter( GetAllocator(), SL("asdf") ) );

		if( (!stream.writer) || (::asSUCCESS != StartNewModule( ::asCreateScriptEngine(), _outputModuleName.GetCharacterArray() )) ) {
			return -1;
		}

		// Collate all the source script files together.
		for( const SystemChar* fileName : inputFiles ) {
			// Create a view of the source script file.
			auto	mappedFile( CreateReadableMemoryMappedFile( GetAllocator(), fileName ) );
			auto	sectionName( "asdf" );

			// Try to append all the data from source.
			if( !mappedFile || (0 > AddSectionFromMemory( sectionName, static_cast<const char*>(mappedFile->GetAddressForFileByteOffset( 0u )), mappedFile->GetAccessibleRegionSizeInBytes() )) ) {
				return -1;
			}
		}

		if( (::asSUCCESS != BuildModule()) || (::asSUCCESS != GetModule()->SaveByteCode( &stream )) ) {
			return -1;
		}

		return AcceptMetadataVisitor( FlatBufferMetadataBuilderVisitor( stream, *this ) );
	}

}	// namespace Tools
}	// namespace Eldritch2

