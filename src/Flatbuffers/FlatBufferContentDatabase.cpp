/*==================================================================*\
  FlatBufferPackageProvider.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Flatbuffers/FlatBufferContentDatabase.hpp>
#include <Flatbuffers/FlatbufferTools.hpp>
//------------------------------------------------------------------//
#include <Flatbuffers/PackageIndex_generated.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace FlatBuffers {

	using namespace ::Eldritch2::Logging;
	using namespace ::Eldritch2::Core;

	// ---------------------------------------------------

	namespace {

		ETCpp14Constexpr ETForceInlineHint ETPureFunctionHint PlatformStringSpan SliceFileName(PlatformStringSpan path) ETNoexceptHint {
			// Overflow will wrap this to 0.
			const auto filePos(path.FindLast(SL(ET_PATH_DELIMITER)) + 1u);
			return path.Slice(filePos, path.Find(SL(ET_EXTENSION_DELIMITER), filePos));
		}

	} // anonymous namespace

	FlatBufferContentDatabase::DeserializationThread::DeserializationThread(ContentDatabase& content) ETNoexceptHint : _runBehavior(RunBehavior::Continue), _content(ETAddressOf(content)) {}

	// ---------------------------------------------------

	FlatBufferContentDatabase::DeserializationThread::~DeserializationThread() {
		AwaitCompletion();
	}

	// ---------------------------------------------------

	FlatBufferContentDatabase::FlatBufferContentDatabase() ETNoexceptHint : _deserializer(*this) {}

	// ---------------------------------------------------

	Result FlatBufferContentDatabase::BindResources(AssetApiBuilder api) {
		const Path packageSpecifier(Path::AllocatorType("Package Specifier"), KnownDirectory::Packages, SL("*.e2index"));
		PackageSet packages(PackageSet::AllocatorType("Package Set Allocator"));

		ForEachFile(packageSpecifier, [&](PlatformStringSpan path) {
			const String name(String::AllocatorType("Package Name Allocator"), SliceFileName(path));
			if (name.GetLength() > Package::MaxPathLength) {
				packages.Emplace(name);
			}
		});

		ET_ABORT_UNLESS(_deserializer.Boot("Package Loader Thread"));
		return ContentDatabase::BindResources(Move(api), packages);
	}

	// ---------------------------------------------------

	void FlatBufferContentDatabase::FreeResources() ETNoexceptHint {
		_deserializer.AwaitCompletion();
	}

	// ---------------------------------------------------

	Result FlatBufferContentDatabase::DeserializationThread::EnterOnCaller() ETNoexceptHint {
		using namespace ::Eldritch2::Core::FlatBuffers;
		using namespace ::flatbuffers;

		ETConstexpr StringSpan LogHeader("\t======================================================" ET_NEWLINE // clang-format off
										 "\t| INITIALIZING LOG                                   |" ET_NEWLINE
										 "\t======================================================" ET_NEWLINE); // clang-format on
		ETConstexpr StringSpan LogFooter("\t======================================================" ET_NEWLINE // clang-format off
										 "\t| TERMINATING LOG                                    |" ET_NEWLINE
										 "\t======================================================" ET_NEWLINE); // clang-format on

		const auto Deserialize([&](Package& package) -> Result {
			const PackageDescriptor* descriptor(nullptr);
			Stopwatch                loadTimer;
			FileView                 index, blob;
			{
				const Path path(Path::AllocatorType("Package Path Allocator"), KnownDirectory::Packages, SL("{}.e2index"), package.GetPath());
				ET_ABORT_UNLESS(index.Open(FileView::Read, path), _log.Write(Error, "\tError opening {}: {}; aborting load." ET_NEWLINE, path, FUNC_RESULT));
				/*	Ensure the data we're working with can plausibly represent a package index. Verification of asset contents requires knowledge of asset
				 *	structure and is delegated to listeners instead of being performed here. */
				descriptor = GetVerifiedRoot<PackageDescriptor>(index.Slice(0u, index.GetByteSize()), PackageDescriptorIdentifier());
				ET_ABORT_UNLESS(descriptor ? Result::Success : Result::Unspecified, _log.Write(Error, "\tValidation failed for {}; aborting load." ET_NEWLINE, path));
			}
			{
				const Path path(Path::AllocatorType("Package Path Allocator"), KnownDirectory::Packages, SL("{}"), package.GetPath());
				ET_ABORT_UNLESS(blob.Open(FileView::Read, path), _log.Write(Error, "\tError opening {}: {}; aborting load." ET_NEWLINE, path, FUNC_RESULT));
			}

			const FileTime modifiedTime(blob.GetLastWriteTime());
			const FileTime packageTime(package.GetReadTime());
			if (packageTime == modifiedTime && packageTime != FileTime::Unavailable) {
				return Result::Success;
			}

			const auto         descriptors(GetSizePrefixedPackageDescriptor(index.Get<const byte>(0u))->Assets());
			Package::AssetList assets(Package::AssetList::AllocatorType("Package Asset List Allocator"), descriptors->size());
			for (uoffset_t id(0u); id < descriptors->size(); ++id) {
				const AssetDescriptor* const descriptor(descriptors->Get(id));
				if ((blob.GetByteSize() - descriptor->ByteLength()) < descriptor->ByteOffset()) {
					_log.Write(Error, "\tAsset {} in package {} references out-of-bounds data!." ET_NEWLINE, AsString(descriptor->Name()), package.GetPath());
					continue;
				}

				if (const Asset* const asset = _content->Insert(_log, AsString(descriptor->Name()), AssetBuilder(package.GetPath(), blob.Slice(descriptor->ByteOffset(), descriptor->ByteLength())))) {
					assets.EmplaceBack(asset);
				}
			}

			_log.Write(Message, "Deserialized package {} in {:.2f}ms ({} assets/{} source(s))." ET_NEWLINE, package.GetPath(), loadTimer.GetDurationMilliseconds(), assets.GetSize(), descriptors->size());
			package.BindAssets(modifiedTime, assets);

			return Result::Success;
		});

		// Back up the previous log, if present.
		Move(KnownDirectory::Logs, SL("ContentLog.old.txt"), SL("ContentLog.txt"));
		ET_ABORT_UNLESS(_log.BindResources(SL("ContentLog.txt"), LogHeader));

		LoadRequest request;
		while (ShouldRun()) {
			if (_content->PopRequest(request)) {
				request.source->CompleteLoad(Deserialize(*request.package), Move(request.package));
				continue;
			}

			YieldProcessor();
		}

		return Result::Success;
	}

}} // namespace Eldritch2::FlatBuffers
