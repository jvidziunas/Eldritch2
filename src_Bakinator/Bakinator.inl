/*==================================================================*\
  Bakinator.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <FileSystem/ReadableMemoryMappedFile.hpp>
#include <Packages/PackageHeader_generated.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Tools {

	template <class GlobalAllocator, class FileAccessorFactory>
	Bakinator<GlobalAllocator, FileAccessorFactory>::Bakinator() : _allocator( UTF8L("Root Allocator") ),
																   _outputFileName( { GetAllocator(), UTF8L("Output File Name Allocator") } ),
																   _outputDataBlobName( { GetAllocator(), UTF8L("Output Data Blob File Name Allocator") } ),
																   _importNames( { GetAllocator(), UTF8L("Imported Package Name Collection Allocator") } ),
																   _exportNames( { GetAllocator(), UTF8L("Export File Name Collection Allocator") } ) {}

// ---------------------------------------------------

	template <class GlobalAllocator, class FileAccessorFactory>
	ETInlineHint FileAccessorFactory& Bakinator<GlobalAllocator, FileAccessorFactory>::GetFileAccessorFactory() {
		return _fileAccessorFactory;
	}

// ---------------------------------------------------

	template <class GlobalAllocator, class FileAccessorFactory>
	ETInlineHint GlobalAllocator& Bakinator<GlobalAllocator, FileAccessorFactory>::GetAllocator() {
		return _allocator;
	}

// ---------------------------------------------------

	template <class GlobalAllocator, class FileAccessorFactory>
	void Bakinator<GlobalAllocator, FileAccessorFactory>::RegisterOptions( OptionRegistrationVisitor& visitor ) {
		using namespace ::std::placeholders;

	// ---

		visitor.AddArgument( UTF8L("-import"), UTF8L("-i"), ::std::bind( &Bakinator::AddImport, this, _1, _2 ) );
		visitor.AddArgument( UTF8L("-packageName"), UTF8L("-n"), ::std::bind( &Bakinator::SetOutputFileName, this, _1, _2 ) );
		visitor.AddInputFileHandler( ::std::bind( &Bakinator::AddExport, this, _1, _2 ) );
	}

// ---------------------------------------------------

	template <class GlobalAllocator, class FileAccessorFactory>
	int	Bakinator<GlobalAllocator, FileAccessorFactory>::Process() {
		using namespace ::Eldritch2::FileSystem::FlatBuffers;
		using namespace ::flatbuffers;

	// ---

		ResizableArray<Offset<flatbuffers::String>>	pendingImports( { GetAllocator(), UTF8L("Pending Import Collection Allocator") } );
		ResizableArray<Offset<Export>>				pendingExports( { GetAllocator(), UTF8L("Pending Export Collection Allocator") } );
		FlatBufferBuilder							builder;
		HeaderBuilder								headerBuilder( builder );
		uint64										dataBlobSize( 0u );
		auto										outputWriter( GetFileAccessorFactory().CreateWriter( GetAllocator(), _outputFileName.GetCharacterArray() ) );
		auto										dataFileWriter( GetFileAccessorFactory().CreateWriter( GetAllocator(), _outputDataBlobName.GetCharacterArray() ) );

		if( !outputWriter || !dataFileWriter ) {
			return -1;
		}
		
		for( const auto& importName : _importNames ) {
			pendingImports.PushBack( builder.CreateString( importName.GetCharacterArray() ) );
		}

		for( const auto& exportName : _exportNames ) {
			const auto			extensionPosition( exportName.IteratorToLastInstance( UTF8L('.') ) );
			const auto			fileNamePosition( exportName.IteratorToLastInstance( ET_UTF8_DIR_SEPARATOR ) );
			const UTF8String<>	inPackageName( (fileNamePosition != exportName.End() ? fileNamePosition + 1 : exportName.Begin()), extensionPosition, { GetAllocator(), UTF8L("Export In-Package Name Allocator") } );
			const UTF8String<>	typeName( (extensionPosition != exportName.End() ? extensionPosition + 1 : extensionPosition), exportName.End(), { GetAllocator(), UTF8L("Export Type Name Allocator") } );
			auto				reader( GetFileAccessorFactory().CreateReadableMemoryMappedFile( GetAllocator(), exportName.GetCharacterArray() ) );

			// Ensure the file was opened.
			if( !reader ) {
				return -1;
			}

			pendingExports.PushBack( CreateExport( builder,
												   builder.CreateString( inPackageName.GetCharacterArray() ),
												   builder.CreateString( typeName.GetCharacterArray() ),
												   dataBlobSize,
												   reader->GetAccessibleRegionSizeInBytes() ) );

			dataFileWriter->Write( reader->GetAddressForFileByteOffset( 0u ), reader->GetAccessibleRegionSizeInBytes() );

			dataBlobSize += reader->GetAccessibleRegionSizeInBytes();
		}

		FinishHeaderBuffer( builder,
							CreateHeader( builder,
										  pendingImports ? builder.CreateVector( pendingImports.Data(), pendingImports.Size() ) : 0,
										  pendingExports ? builder.CreateVector( pendingExports.Data(), pendingExports.Size() ) : 0 ) );

		outputWriter->Write( builder.GetBufferPointer(), builder.GetSize() );

		return 0;
	}

// ---------------------------------------------------

	template <class GlobalAllocator, class FileAccessorFactory>
	int	Bakinator<GlobalAllocator, FileAccessorFactory>::SetOutputFileName( const ::Eldritch2::UTF8Char* const name, const ::Eldritch2::UTF8Char* const nameEnd ) {
		_outputFileName.Assign( name, nameEnd ).EnsureEndsWith( UTF8L(".e2toc") );
		_outputDataBlobName.Assign( _outputFileName.Begin(), _outputFileName.IteratorToLastInstance( UTF8L('.') ) );

		return 0;
	}

// ---------------------------------------------------

	template <class GlobalAllocator, class FileAccessorFactory>
	int	Bakinator<GlobalAllocator, FileAccessorFactory>::AddImport( const ::Eldritch2::UTF8Char* const name, const ::Eldritch2::UTF8Char* const nameEnd ) {
		_importNames.PushBack( { name, nameEnd, { GetAllocator(), UTF8L("Import Name Allocator") } } );

		return 0;
	}

// ---------------------------------------------------

	template <class GlobalAllocator, class FileAccessorFactory>
	int	Bakinator<GlobalAllocator, FileAccessorFactory>::AddExport( const ::Eldritch2::UTF8Char* const name, const ::Eldritch2::UTF8Char* const nameEnd ) {
		_exportNames.PushBack( { name, nameEnd, { GetAllocator(), UTF8L("Export Name Allocator") } } );

		return 0;
	}

}	// namespace Tools
}	// namespace Eldritch2