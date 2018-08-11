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
#include <eastl/iterator.h>
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
		static const Path<> PlatformExtension(PackageIndexExtension());

		options.Register("--packageName", "-n", [this](StringView<Utf8Char> source) -> int {
			if (source.IsEmpty()) {
				std::cerr << "Package name switch requires argument!" << std::endl;
				return 1;
			}

			_indexPath.Assign(source).EnsureSuffix(PlatformExtension);
			_blobPath.Assign(_indexPath.Begin(), _indexPath.FindLastInstance(PlatformExtension));

			return 0;
		});

		options.RegisterInputFileHandler([this](StringView<Utf8Char> source) -> int {
			if (source.IsEmpty()) {
				return 1;
			}

			Path<> path(source, MallocAllocator("Export Path Allocator"));
			path.Replace('\\', '/');

			_exports.Append(eastl::move(path));

			return 0;
		});
	}

	// ---------------------------------------------------

	int Bakinator::Process() {
		if (_exports.IsEmpty()) {
			std::cerr << "Package must contain at least one asset!" << std::endl
					  << "No source files were specified, or the supplied pattern(s) did not match any files." << std::endl;
			return AsPosixInt(Error::InvalidParameter);
		}

		//	Open the index/table of contents file writer.
		FileWriter diskIndex, diskBlob;
		ET_ABORT_UNLESS2(diskIndex.CreateOrTruncate(_indexPath), "Failed to create index file '" << _indexPath << "': " << AsCString(result) << '!');
		ET_ABORT_UNLESS2(diskBlob.CreateOrTruncate(_blobPath), "Failed to create blob file '" << _blobPath << "': " << AsCString(result) << '!');

		FlatBufferBuilder        flatbuffer;
		Offset<AssetDescriptor>* assetDescriptors;
		Offset<PackageIndex>     root(CreatePackageIndex(flatbuffer, flatbuffer.CreateUninitializedVector<Offset<AssetDescriptor>>(_exports.GetSize(), &assetDescriptors)));

		//	Append the contents of each input file to the bulk data blob.
		uint64 blobOffsetInBytes(0u);
		for (const Path<>& path : _exports) {
			const String<> utf8Path(path, MallocAllocator("UTF-8 Path Allocator"));
			MappedFile     diskSource;

			ET_ABORT_UNLESS2(diskSource.Open(MappedFile::Read, path), "\t'" << path.AsCString() << "' failed: " << AsCString(result) << '!');
			ET_ABORT_UNLESS2(diskBlob.Append(diskSource.Get(0u), diskSource.GetSizeInBytes()), "\t'" << utf8Path.AsCString() << "' failed: " << AsCString(result) << '!');
			std::cout << "\t'" << utf8Path.AsCString() << "' done (" << diskSource.GetSizeInBytes() << " bytes)" << std::endl;

			*assetDescriptors++ = CreateAssetDescriptor(flatbuffer, flatbuffer.CreateSharedString(utf8Path.AsCString()), blobOffsetInBytes, diskSource.GetSizeInBytes());
			blobOffsetInBytes += diskSource.GetSizeInBytes();
		}

		flatbuffer.Finish(root, PackageIndexIdentifier());
		if (Succeeded(diskIndex.Append(flatbuffer.GetBufferPointer(), flatbuffer.GetSize()))) {
			std::cout << "Package created successfully." << std::endl;
		}

		return 0;
	}

}} // namespace Eldritch2::Tools
