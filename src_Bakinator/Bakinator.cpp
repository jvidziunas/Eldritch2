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
#include <Common/Mpl/Tuple.hpp>
#include <Common/FileView.hpp>
#include <Common/Result.hpp>
#include <Common/Memory.hpp>
#include <Bakinator.hpp>
//------------------------------------------------------------------//
#include <FlatBuffers/PackageIndex_generated.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Tools {

	using namespace ::Eldritch2::Core::FlatBuffers;
	using namespace ::Eldritch2::Logging;
	using namespace ::flatbuffers;

	// ---------------------------------------------------

	Bakinator::Bakinator() ETNoexceptHint : _exports(MallocAllocator("Export File Name Collection Allocator")),
											_indexPath(MallocAllocator("Output Index File Name Allocator")),
											_blobPath(MallocAllocator("Output Blob File Name Allocator")) {}

	// ---------------------------------------------------

	void Bakinator::RegisterOptions(OptionRegistrar& options) {
		options.Register(SL("--packageName"), SL("-n"), [this](Log& log, PlatformStringSpan source) -> Result {
			ET_ABORT_IF(source.IsEmpty() ? Result::InvalidParameter : Result::Success, log.Write(Error, "Package name switch requires argument!" ET_NEWLINE));

			Path indexPath(_indexPath.GetAllocator(), KnownDirectory::Relative, source);
			Path blobPath(_blobPath.GetAllocator(), KnownDirectory::Relative, indexPath.Slice(0u, indexPath.FindLast(PlatformExtension)));

			Swap(_indexPath, indexPath);
			Swap(_blobPath, blobPath);

			return Result::Success;
		});
		options.RegisterInputFileHandler([this](Log& log, PlatformStringSpan source) -> Result {
			ExportList::Reference input(_exports.EmplaceBack(Path(MallocAllocator("Export Path Allocator"), KnownDirectory::Relative, source), FileReader()));

			// Canonicalize path elements.
			Get<Path&>(input).ReplaceAll(SL('\\'), SL('/'));
			ET_ABORT_UNLESS(Get<FileReader&>(input).Open(Get<Path&>(input)), log.Write(Error, "Error opening {} for read: {}" ET_NEWLINE, Get<Path&>(input), FUNC_RESULT));

			return Result::Success;
		});
	}

	// ---------------------------------------------------

	Result Bakinator::Process(Log& log) {
		ET_ABORT_IF(_exports.IsEmpty() ? Result::InvalidObjectState : Result::Success, log.Write(Error, "Package must contain at least one asset!" ET_NEWLINE "No source files were specified, or the supplied pattern(s) did not match any files." ET_NEWLINE));

		FlatBufferBuilder         flatbuffer;
		Offset<AssetDescriptor>*  exports;
		Offset<PackageDescriptor> root(CreatePackageDescriptor(flatbuffer, flatbuffer.CreateUninitializedVector(_exports.GetSize(), ETAddressOf(exports))));
		uint64                    blobPosition(0u);

		Transform(_exports.Begin(), _exports.End(), exports, [&](ExportList::Reference export) -> Offset<AssetDescriptor> {
			AssetDescriptorBuilder descriptor(flatbuffer);
			descriptor.add_ByteLength(Get<FileReader&>(export).GetByteSize());
			descriptor.add_ByteOffset(blobPosition);
			// Transcode name to UTF-8
			const String utf8Path(String::AllocatorType("UTF-8 Path Allocator"), Get<Path&>(export));
			descriptor.add_Name(flatbuffer.CreateSharedString(utf8Path.AsCString(), utf8Path.GetLength()));

			return descriptor.Finish();
		});

		flatbuffer.Finish(root, PackageDescriptorIdentifier());

		//	Open the index/table of contents file writer.
		FileWriter diskIndex;
		ET_ABORT_UNLESS(diskIndex.CreateOrTruncate(_indexPath), log.Write(Error, "Failed to create index file {}: {}!", _indexPath, FUNC_RESULT));
		ET_ABORT_UNLESS(diskIndex.Append(flatbuffer.GetBufferPointer(), flatbuffer.GetSize()), log.Write(Error, "Failed to write index {}: {}!", _indexPath, FUNC_RESULT));

		FileView diskBlob;
		ET_ABORT_UNLESS(diskBlob.ClearOrCreate(FileView::Write, _blobPath, blobPosition), log.Write(Error, "Failed to create blob file {}: {}!", _blobPath, FUNC_RESULT));
		ForEach(_exports.Begin(), _exports.End(), [&](ExportList::Reference export) {

		});

		return Result::Success;
	}

}} // namespace Eldritch2::Tools
