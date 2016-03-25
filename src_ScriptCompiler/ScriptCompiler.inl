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
#include <Scripting/AngelScript/ApiExport.hpp>
#include <Utility/Containers/Range.hpp>
#include <iostream>
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
	ScriptCompiler<GlobalAllocator, FileAccessorFactory>::ScriptCompiler() : _globalAllocator( UTF8L("Root Allocator") ),
																			 _engine( ::asCreateScriptEngine() ),
																			 _fileAccessorFactory(),
																			 _scriptBuilder(),
																			 _outputModuleName( { GetAllocator(), UTF8L("Output Module Name String Allocator") } ),
																			 _engineApiDescriptorPath( { GetAllocator(), UTF8L("Engine API Descriptor Path Allocator") } ),
																			 _inputFiles( { GetAllocator(), UTF8L("Input File Name Collection Allocator") } ) {
		auto callback( [] ( const asSMessageInfo* msg, void* /*param*/ ) {
			::std::cerr << msg->section << '(' << msg->row << ", " << msg->col << "): " << msg->message << ::std::endl;
		} );

		_engine->SetMessageCallback( asFunctionPtr( static_cast<void (ETStdCall*)(const asSMessageInfo*, void*)>(callback) ), nullptr, ::asECallConvTypes::asCALL_STDCALL );
	}

// ---------------------------------------------------

	template <class GlobalAllocator, class FileAccessorFactory>
	ScriptCompiler<GlobalAllocator, FileAccessorFactory>::~ScriptCompiler() {
		if( auto scriptModule = GetScriptBuilder().GetModule() ) {
			scriptModule->Discard();
		}
	}

// ---------------------------------------------------

	template <class GlobalAllocator, class FileAccessorFactory>
	ETInlineHint typename ScriptCompiler<GlobalAllocator, FileAccessorFactory>::AllocatorType& ScriptCompiler<GlobalAllocator, FileAccessorFactory>::GetAllocator() {
		return _globalAllocator;
	}

// ---------------------------------------------------

	template <class GlobalAllocator, class FileAccessorFactory>
	ETInlineHint typename ScriptCompiler<GlobalAllocator, FileAccessorFactory>::FileAccessorFactoryType& ScriptCompiler<GlobalAllocator, FileAccessorFactory>::GetFileAccessorFactory() {
		return _fileAccessorFactory;
	}

// ---------------------------------------------------

	template <class GlobalAllocator, class FileAccessorFactory>
	ETInlineHint ::CScriptBuilder& ScriptCompiler<GlobalAllocator, FileAccessorFactory>::GetScriptBuilder() {
		return _scriptBuilder;
	}

// ---------------------------------------------------

	template <class GlobalAllocator, class FileAccessorFactory>
	template <class MetadataVisitor>
	int ScriptCompiler<GlobalAllocator, FileAccessorFactory>::AcceptMetadataVisitor( MetadataVisitor&& visitor ) {
		const auto	scriptModule( GetScriptBuilder().GetModule() );

		visitor.BeginMetadataProcessing();

		// The built-in metadata parser uses really inconsistent indexing that additionally isn't useful across execution boundaries, so we need to get a little creative here.
		// Since type/function/property indices will remain the same within the module, we instead will loop through each exposed element in the module, check if any metadata has
		// been associated with the element, and then dispatch out to the visitor if appropriate.
		{	visitor.BeginTypeMetadataProcessing();

			for( uint32 index( 0u ), typeCount( scriptModule->GetObjectTypeCount() ); index < typeCount; ++index ) {
				const auto	type( scriptModule->GetObjectTypeByIndex( index ) );
				int			typeID( type->GetTypeId() );

				for( uint32 methodIndex( 0u ), methodCount( type->GetMethodCount() ); methodIndex < methodCount; ++methodIndex ) {
					if( auto metadata = GetScriptBuilder().GetMetadataStringForTypeMethod( typeID, type->GetMethodByIndex( methodIndex ) ) ) {
						visitor.ProcessTypeMethodMetadata( methodIndex, metadata );
					}
				}

				for( uint32 propertyIndex( 0u ), propertyCount( type->GetPropertyCount() ); propertyIndex < propertyCount; ++propertyIndex ) {
					if( auto metadata = GetScriptBuilder().GetMetadataStringForTypeProperty( typeID, static_cast<int>(propertyIndex) ) ) {
						visitor.ProcessTypePropertyMetadata( propertyIndex, metadata );
					}
				}

				if( auto metadata = GetScriptBuilder().GetMetadataStringForType( type->GetTypeId() ) ) {
					visitor.ProcessTypeMetadata( index, metadata );
				}
			}

			visitor.FinishTypeMetadataProcessing();
		}

		{	visitor.BeginFunctionMetadataProcessing();

			for( uint32 index( 0u ); index != scriptModule->GetFunctionCount(); ++index ) {
				auto	function( scriptModule->GetFunctionByIndex( index ) );

				if( auto metadata = GetScriptBuilder().GetMetadataStringForFunc( function ) ) {
					visitor.ProcessFunctionMetadata( index, metadata );
				}
			}

			visitor.FinishFunctionMetadataProcessing();
		}

		{	visitor.BeginPropertyMetadataProcessing();

			for( uint32 index( 0u ); index != scriptModule->GetGlobalVarCount(); ++index ) {
				if( auto metadata = GetScriptBuilder().GetMetadataStringForVar( static_cast<int>(index) ) ) {
					visitor.ProcessFunctionMetadata( index, metadata );
				}
			}

			visitor.FinishPropertyMetadataProcessing();
		}

		return visitor.FinishMetadataProcessing();
	}

// ---------------------------------------------------

	template <class GlobalAllocator, class FileAccessorFactory>
	int ScriptCompiler<GlobalAllocator, FileAccessorFactory>::Process() {
		class OutputStream : public ::asIBinaryStream {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref OutputStream instance.
			ETInlineHint OutputStream( ::Eldritch2::InstancePointer<FileSystem::SynchronousFileWriter>&& writer ) : writer( ::std::move( writer ) ) {}

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

		OutputStream	stream( GetFileAccessorFactory().CreateWriter( GetAllocator(), _outputModuleName.AsCString() ) );

		if( !stream.writer || (!GetScriptBuilder().GetModule()) ) {
			return -1;
		}

		if( _engineApiDescriptorPath ) {
			const auto	apiDescriptorFile( GetFileAccessorFactory().CreateReadableMemoryMappedFile( GetAllocator(), _engineApiDescriptorPath.AsCString() ) );

			if( !apiDescriptorFile || !Scripting::AngelScript::ImportScriptApi( GetAllocator(), *_engine.get(), *apiDescriptorFile ) ) {
				return -1;
			}
		}

		// Collate all the source script files together.
		for( const auto& fileName : _inputFiles ) {
			// Create a view of the source script file.
			auto	mappedFile( GetFileAccessorFactory().CreateReadableMemoryMappedFile( GetAllocator(), fileName.AsCString() ) );

			// Try to append all the data from source.
			if( !mappedFile || (0 > GetScriptBuilder().AddSectionFromMemory( fileName.AsCString(), static_cast<const char*>(mappedFile->GetAddressForFileByteOffset( 0u )), mappedFile->GetAccessibleRegionSizeInBytes() )) ) {
				return -1;
			}
		}

		if( (::asSUCCESS != GetScriptBuilder().BuildModule()) || (::asSUCCESS != GetScriptBuilder().GetModule()->SaveByteCode( &stream )) ) {
			return -1;
		}

		return AcceptMetadataVisitor( FlatBufferMetadataBuilderVisitor( stream, GetAllocator() ) );
	}

// ---------------------------------------------------

	template <class GlobalAllocator, class FileAccessorFactory>
	int ScriptCompiler<GlobalAllocator, FileAccessorFactory>::SetOutputModuleName( const ::Eldritch2::UTF8Char* const argument, const ::Eldritch2::UTF8Char* const argumentEnd ) {
		_outputModuleName.Assign( argument, argumentEnd ).EnsureEndsWith( Scripting::AngelScript::FlatBuffers::ModuleMetadataExtension() );

		// Ensure we don't leak resources.
		if( auto existingModule = GetScriptBuilder().GetModule() ) {
			existingModule->Discard();
		}

		// Internally, the module name won't have the extension.
		const UTF8String<>	moduleName( _outputModuleName.Begin(),
										_outputModuleName.FindFirstInstance( Scripting::AngelScript::FlatBuffers::ModuleMetadataExtension() ),
										{ _outputModuleName.GetAllocator().GetParent(), UTF8L("Script Module Name Temp Allocator") } );
		
		GetScriptBuilder().StartNewModule( _engine.get(), moduleName.AsCString() );

		return 0;
	}

// ---------------------------------------------------

	template <class GlobalAllocator, class FileAccessorFactory>
	int ScriptCompiler<GlobalAllocator, FileAccessorFactory>::SetApiDescriptorPath( const ::Eldritch2::UTF8Char* const argument, const ::Eldritch2::UTF8Char* const argumentEnd ) {
		_engineApiDescriptorPath.Assign( argument, argumentEnd );

		if( _engineApiDescriptorPath ) {
			_engineApiDescriptorPath.EnsureEndsWith( UTF8L(".E2AngelscriptApi") );
		}

		return 0;
	}

// ---------------------------------------------------

	template <class GlobalAllocator, class FileAccessorFactory>
	int ScriptCompiler<GlobalAllocator, FileAccessorFactory>::SetOptimizationLevel( const int level ) {
		_engine->SetEngineProperty( ::asEEngineProp::asEP_OPTIMIZE_BYTECODE, static_cast<::asPWORD>(level > 0) );

		return 0;
	}

// ---------------------------------------------------

	template <class GlobalAllocator, class FileAccessorFactory>
	int ScriptCompiler<GlobalAllocator, FileAccessorFactory>::AddInputFile( const ::Eldritch2::UTF8Char* const argument, const ::Eldritch2::UTF8Char* const argumentEnd ) {
		_inputFiles.PushBack( { argument, argumentEnd, { GetAllocator(), UTF8L("Input File Name Allocator") } } );

		return 0;
	}

// ---------------------------------------------------

	template <class GlobalAllocator, class FileAccessorFactory>
	void ScriptCompiler<GlobalAllocator, FileAccessorFactory>::RegisterOptions( OptionRegistrationVisitor& visitor ) {
		using namespace ::std::placeholders;

	// ---

		visitor.AddTypedArgument<int>(	UTF8L("--optimizationLevel"),	UTF8L("-o"),	::std::bind( &ScriptCompiler::SetOptimizationLevel, this, _1 ) );
		visitor.AddArgument(			UTF8L("--moduleName"),			UTF8L("-m"),	::std::bind( &ScriptCompiler::SetOutputModuleName, this, _1, _2 ) );
		visitor.AddArgument(			UTF8L("--apiDescriptorPath"),	UTF8L("-a"),	::std::bind( &ScriptCompiler::SetApiDescriptorPath, this, _1, _2 ) );

		visitor.AddInputFileHandler( ::std::bind( &ScriptCompiler::AddInputFile, this, _1, _2 ) );
		
	}

}	// namespace Tools
}	// namespace Eldritch2

