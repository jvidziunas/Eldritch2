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
#include <Assets/PackageHeader_generated.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Tools {

	template <class GlobalAllocator, class FileAccessorFactory>
	Bakinator<GlobalAllocator, FileAccessorFactory>::Bakinator() : _allocator( "Root Allocator" ),
																   _outputFileName( { GetAllocator(), "Output File Name Allocator" } ),
																   _outputDataBlobName( { GetAllocator(), "Output Data Blob File Name Allocator" } ),
																   _importNames( { GetAllocator(), "Imported Package Name Collection Allocator" } ),
																   _exportNames( { GetAllocator(), "Export File Name Collection Allocator" } ) {}

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
		using namespace std::placeholders;

	// ---

		visitor.AddArgument( "--import",		"-i",	std::bind( &Bakinator::AddImport, this, _1, _2 ) );
		visitor.AddArgument( "--packageName",	"-n",	std::bind( &Bakinator::SetOutputFileName, this, _1, _2 ) );

		visitor.AddInputFileHandler( std::bind( &Bakinator::AddExport, this, _1, _2 ) );
	}

// ---------------------------------------------------

	template <class GlobalAllocator, class FileAccessorFactory>
	int	Bakinator<GlobalAllocator, FileAccessorFactory>::Process() {
		using namespace ::Eldritch2::FileSystem::FlatBuffers;
		using namespace ::flatbuffers;

	// ---

		ResizableArray<Offset<flatbuffers::String>>	pendingImports( { GetAllocator(), "Pending Import Collection Allocator" } );
		ResizableArray<Offset<Export>>				pendingExports( { GetAllocator(), "Pending Export Collection Allocator" } );
		FlatBufferBuilder							builder;
		uint64										dataBlobSize( 0u );
		auto										outputWriter( GetFileAccessorFactory().CreateWriter( GetAllocator(), _outputFileName.AsCString() ) );
		auto										dataFileWriter( GetFileAccessorFactory().CreateWriter( GetAllocator(), _outputDataBlobName.AsCString() ) );

		if( !outputWriter || !dataFileWriter ) {
			return -1;
		}

		for( const auto& exportName : _exportNames ) {
			const auto			extensionPosition( exportName.FindLastInstance( UTF8L('.') ) );
			const auto			fileNamePosition( exportName.FindLastInstance( ET_DIR_SEPARATOR ) );
			const Utf8String<>	inPackageName( (fileNamePosition != exportName.End() ? fileNamePosition + 1 : exportName.Begin()), extensionPosition, { GetAllocator(), "Export In-Package Name Allocator" } );
			const Utf8String<>	typeName( extensionPosition, exportName.End(), { GetAllocator(), "Export Type Name Allocator" } );
			auto				reader( GetFileAccessorFactory().CreateReadableMemoryMappedFile( GetAllocator(), exportName.AsCString() ) );

			// Ensure the file was opened.
			if( !reader ) {
				return -1;
			}

			pendingExports.PushBack( CreateExport( builder,
												   builder.CreateSharedString( inPackageName.AsCString() ),
												   builder.CreateSharedString( typeName.AsCString() ),
												   dataBlobSize,
												   reader->GetAccessibleRegionSizeInBytes() ) );

			dataFileWriter->Write( reader->GetAddressForFileByteOffset( 0u ), reader->GetAccessibleRegionSizeInBytes() );

			dataBlobSize += reader->GetAccessibleRegionSizeInBytes();
		}

		for( const auto& importName : _importNames ) {
			pendingImports.PushBack( builder.CreateString( importName.AsCString() ) );
		}

		FinishPackageHeaderBuffer( builder,
								   CreatePackageHeader( builder,
								   						pendingImports ? builder.CreateVector( pendingImports.Data(), pendingImports.GetSize() ) : 0,
								   						pendingExports ? builder.CreateVector( pendingExports.Data(), pendingExports.GetSize() ) : 0 ) );

		outputWriter->Write( builder.GetBufferPointer(), builder.GetSize() );

		return 0;
	}

// ---------------------------------------------------

	template <class GlobalAllocator, class FileAccessorFactory>
	int	Bakinator<GlobalAllocator, FileAccessorFactory>::SetOutputFileName( const Eldritch2::Utf8Char* const name, const Eldritch2::Utf8Char* const nameEnd ) {
		_outputFileName.Assign( name, nameEnd ).EnsureEndsWith( FileSystem::FlatBuffers::PackageHeaderExtension() );
		_outputDataBlobName.Assign( _outputFileName.Begin(), _outputFileName.FindLastInstance( '.' ) );

		return 0;
	}

// ---------------------------------------------------

	template <class GlobalAllocator, class FileAccessorFactory>
	int	Bakinator<GlobalAllocator, FileAccessorFactory>::AddImport( const Eldritch2::Utf8Char* const name, const Eldritch2::Utf8Char* const nameEnd ) {
		_importNames.PushBack( { name, nameEnd, { GetAllocator(), "Import Name Allocator" } } );

		return 0;
	}

// ---------------------------------------------------

	template <class GlobalAllocator, class FileAccessorFactory>
	int	Bakinator<GlobalAllocator, FileAccessorFactory>::AddExport( const Eldritch2::Utf8Char* const name, const Eldritch2::Utf8Char* const nameEnd ) {
		_exportNames.PushBack( { name, nameEnd, { GetAllocator(), "Export Name Allocator" } } );

		return 0;
	}

}	// namespace Tools
}	// namespace Eldritch2