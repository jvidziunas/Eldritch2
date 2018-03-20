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
#include <Common/Containers/PlatformString.hpp>
#include <Common/FileWriter.hpp>
#include <Common/MappedFile.hpp>
#include <Common/ErrorCode.hpp>
#include <Bakinator.hpp>
//------------------------------------------------------------------//
#include <FlatBuffers/PackageIndex_generated.h>
#include <eastl/iterator.h>
#include <iostream>
//------------------------------------------------------------------//

#define ET_ABORT_UNLESS2(condition, message, ...)			\
	{	const auto result( condition );						\
		if (Failed( result ) ) {							\
			std::cerr << message __VA_ARGS__ << std::endl;	\
			return AsPosixInt( result );					\
		}													\
	}

namespace Eldritch2 {
namespace Tools {
namespace {

	template <typename Allocator>
	ETInlineHint PlatformString<Allocator> AsPlatformString( const String<Allocator>& source ) {
		return PlatformString<Allocator>( source.Begin(), source.End(), source.GetAllocator() );
	}

}	// anonymous namespace

	using namespace ::Eldritch2::Assets::FlatBuffers;
	using namespace ::flatbuffers;

	Bakinator::Bakinator(
	) : _exports( MallocAllocator( "Export File Name Collection Allocator" ) ),
		_outPath( MallocAllocator( "Output File Name Allocator" ) ) {}

// ---------------------------------------------------

	void Bakinator::RegisterOptions( OptionRegistrar& options ) {
		options.Register( "--packageName", "-n", [this] ( Range<const Utf8Char*> source ) -> int {
			if (source.IsEmpty()) {
				std::cerr << "Package name switch requires argument!" << std::endl;
				return 1;
			}

			_outPath.Assign( source.Begin(), source.End() ).EnsureSuffix( PackageIndexExtension() );

			return 0;
		} );

		options.RegisterInputFileHandler( [this] ( Range<const Utf8Char*> source ) -> int {
			if (source.IsEmpty()) {
				return 1;
			}

			String<> path( source.Begin(), source.End(), MallocAllocator( "Export Path Allocator" ) );

			path.Replace( '\\', '/' );

			_exports.Insert( eastl::move( path ) );

			return 0;
		} );
	}

// ---------------------------------------------------

	int Bakinator::Process() {
		FlatBufferBuilder	builder;
		uint64				blobSizeInBytes( 0u );

		if (_exports.IsEmpty()) {
			std::cerr << "Package must contain at least one asset!" << std::endl;
			std::cerr << "No source files were specified, or the supplied pattern(s) did not match any files." << std::endl;

			return AsPosixInt( Error::InvalidParameter );
		}

	//	Open the index/table of contents file writer.
		FileWriter indexFile;
		ET_ABORT_UNLESS2( indexFile.CreateOrTruncate( AsPlatformString( _outPath ) ), "Failed to create index file '"<<_outPath<<"': "<<AsCString( result )<<'!' );
		
	//	Strip the index file suffix.
		_outPath.Erase( _outPath.FindLastInstance( PackageIndexExtension() ), _outPath.End() );

	//	Open the bulk data blob file writer.
		FileWriter blobFile;
		ET_ABORT_UNLESS2( blobFile.CreateOrTruncate( AsPlatformString( _outPath ) ), "Failed to create blob file '"<<_outPath<<"': "<<AsCString( result )<<'!' );

	//	Append the contents of each input file to the bulk data blob.
		ArrayList<Offset<AssetDescriptor>>	exports( MallocAllocator( "Pending Export Collection Allocator" ) );
		for (const String<>& exportPath : _exports) {
			MappedFile sourceFile;
			ET_ABORT_UNLESS2( sourceFile.Open( MappedFile::Read, AsPlatformString( exportPath ) ), "\t'"<<exportPath.AsCString()<<"' failed: "<<AsCString( result )<<'!' );

			const size_t assetSize( sourceFile.GetSizeInBytes() );
			exports.Append( CreateAssetDescriptor( builder, builder.CreateSharedString( exportPath ), blobSizeInBytes, assetSize ) );
			blobFile.Append( sourceFile.GetAddressForByteOffset( 0u ), assetSize );

			blobSizeInBytes += assetSize;

			std::cout <<"\t'"<<exportPath.AsCString()<<"' done ("<<assetSize<<" bytes)"<< std::endl;
		}

		FinishPackageIndexBuffer( builder, CreatePackageIndex( builder, builder.CreateVector( exports.GetData(), exports.GetSize() ) ) );

		if (Succeeded( indexFile.Append( builder.GetBufferPointer(), builder.GetSize() ) )) {
			std::cout << "Package created successfully." << std::endl;
		}
		
		return 0;
	}

}	// namespace Tools
}	// namespace Eldritch2