/*==================================================================*\
  Bakinator.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/FileWriter.hpp>
#include <Common/MappedFile.hpp>
#include <Common/ErrorCode.hpp>
#include <Bakinator.hpp>
//------------------------------------------------------------------//
#include <FlatBuffers/PackageIndex_generated.h>
#include <iostream>
//------------------------------------------------------------------//

#define ET_ABORT_UNLESS2(condition, message, ...)          \
	{                                                      \
		const auto result(condition);                      \
		if (Failed(result)) {                              \
			std::cerr << message __VA_ARGS__ << std::endl; \
			return AsPosixInt(result);                     \
		}                                                  \
	}

namespace Eldritch2 { namespace Tools {

	using namespace ::Eldritch2::Assets::FlatBuffers;
	using namespace ::flatbuffers;

	Bakinator::Bakinator() :
		_exports(MallocAllocator("Export File Name Collection Allocator")),
		_indexPath(MallocAllocator("Output Index File Name Allocator")),
		_blobPath(MallocAllocator("Output Blob File Name Allocator")) {}

	// ---------------------------------------------------

	void Bakinator::RegisterOptions(OptionRegistrar& options) {
		options.Register(L"--packageName", L"-n", [this](PlatformStringView source) -> int {
			static const Path PlatformExtension(MallocAllocator(), { PackageIndexExtension(), StringLength(PackageIndexExtension()) });
			if (source.IsEmpty()) {
				std::cerr << "Package name switch requires argument!" << std::endl;
				return 1;
			}

			_indexPath.Assign(source).EnsureSuffix(PlatformExtension);
			_blobPath.Assign(_indexPath.Begin(), _indexPath.FindLast(PlatformExtension));

			return 0;
		});
		options.RegisterInputFileHandler([this](PlatformStringView source) -> int {
			if (source.IsEmpty()) {
				return 1;
			}

			Path path(MallocAllocator("Export Path Allocator"), source);
			path.ReplaceAll(L'\\', L'/');

			_exports.Append(eastl::move(path));
			return 0;
		});
	}

	// ---------------------------------------------------

	ErrorCode Bakinator::Process() {
		if (_exports.IsEmpty()) {
			std::cerr << "Package must contain at least one asset!" << std::endl
					  << "No source files were specified, or the supplied pattern(s) did not match any files." << std::endl;
			return Error::InvalidParameter;
		}

		//	Open the index/table of contents file writer.
		FileWriter diskIndex, diskBlob;
		ET_ABORT_UNLESS2(diskIndex.CreateOrTruncate(_indexPath), "Failed to create index file {}: {}!", _indexPath, result);
		ET_ABORT_UNLESS2(diskBlob.CreateOrTruncate(_blobPath), "Failed to create blob file {}: {}!", _blobPath, result);

		FlatBufferBuilder        flatbuffer;
		Offset<AssetDescriptor>* assetDescriptors;
		Offset<PackageIndex>     root(CreatePackageIndex(flatbuffer, flatbuffer.CreateUninitializedVector<Offset<AssetDescriptor>>(_exports.GetSize(), &assetDescriptors)));

		//	Append the contents of each input file to the bulk data blob.
		uint64 blobOffset(0u);
		for (const Path& path : _exports) {
			const String utf8Path(MallocAllocator("UTF-8 Path Allocator"), path);
			MappedFile   diskSource;

			ET_ABORT_UNLESS2(diskSource.Open(MappedFile::Read, path), "\tFailed to open {}: {}!", path, result);
			ET_ABORT_UNLESS2(diskBlob.Append(diskSource.Get(0u), diskSource.GetSizeInBytes()), "\tFailed to write {} to blob: {}!" << utf8Path, result);
			std::cout << "\t'" << utf8Path << "' done (" << diskSource.GetSizeInBytes() << " bytes)" << std::endl;

			*assetDescriptors++ = CreateAssetDescriptor(flatbuffer, flatbuffer.CreateSharedString(utf8Path.AsCString()), blobOffset, diskSource.GetSizeInBytes());
			blobOffset += diskSource.GetSizeInBytes();
		}

		flatbuffer.Finish(root, PackageIndexIdentifier());
		ET_ABORT_UNLESS2(diskIndex.Append(flatbuffer.GetBufferPointer(), flatbuffer.GetSize()), "Failed to write index {}: {}!", _indexPath, result);

		return Error::None;
	}

}} // namespace Eldritch2::Tools
