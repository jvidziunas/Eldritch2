/*==================================================================*\
  FlatBufferPackageProvider.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Assets/FlatBufferPackageProvider.hpp>
#include <Assets/Asset.hpp>
//------------------------------------------------------------------//
#include <Flatbuffers/PackageIndex_generated.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Assets {

	using namespace ::Eldritch2::Assets::FlatBuffers;
	using namespace ::Eldritch2::Logging;
	using namespace ::flatbuffers;

	namespace {

		ETInlineHint ETPureFunctionHint float32 AsMilliseconds(MicrosecondTime microseconds) {
			return AsFloat(microseconds) / /*microseconds per millisecond*/ 1000.0f;
		}

	} // anonymous namespace

	FlatBufferPackageProvider::FlatBufferPackageProvider() :
		_log(),
		_assetDatabase(),
		_packageDatabase() {}

	// ---------------------------------------------------

	void FlatBufferPackageProvider::ScanPackages() {
		PackageDatabase::LoadableSet packages(MallocAllocator("Package Set Allocator"));
		Path<>                       specifier;

		_log.Write(MessageType::Message, "Scanning packages." UTF8_NEWLINE);
		ForEachFile(specifier.Assign(KnownDirectory::Packages, L"*.e2index"), [this, &packages](StringView<PlatformChar> path) {
			String<> name(path);

			if (Package::MaxPathLength < name.GetLength()) {
				_log.Write(MessageType::Message, "\tPackage name '{}' exceeds maximum length {}; ignoring." UTF8_NEWLINE, name, Package::MaxPathLength);
				return;
			}

			_log.Write(MessageType::Message, "\tDiscovered package '{}'." UTF8_NEWLINE, name);
			packages.Emplace(name);
		});

		_log.Write(MessageType::Message, "Package scan complete; found {} package(s)." UTF8_NEWLINE, packages.GetSize());
		_packageDatabase.BindResources(eastl::move(packages));
	}

	// ---------------------------------------------------

	ErrorCode FlatBufferPackageProvider::BindResources() {
		Move(KnownDirectory::Logs, L"ContentLog.old.txt", L"ContentLog.txt");
		ET_FAIL_UNLESS(_log.BindResources(L"ContentLog.txt"));

		_log.Write(MessageType::Message, "\t======================================================" UTF8_NEWLINE);
		_log.Write(MessageType::Message, "\t| INITIALIZING LOG                                   |" UTF8_NEWLINE);
		_log.Write(MessageType::Message, "\t======================================================" UTF8_NEWLINE);

		return Error::None;
	}

	// ---------------------------------------------------

	void FlatBufferPackageProvider::FreeResources() {
		_log.Write(MessageType::Message, "\t======================================================" UTF8_NEWLINE);
		_log.Write(MessageType::Message, "\t| TERMINATING LOG                                    |" UTF8_NEWLINE);
		_log.Write(MessageType::Message, "\t======================================================" UTF8_NEWLINE);

		_log.FreeResources();
	}

	// ---------------------------------------------------

	void FlatBufferPackageProvider::SatisfyLoad() {
		PackageDatabase::LoadRequest request;

		if (_packageDatabase.PopRequest(request)) {
			request.callback(eastl::move(request.package), DeserializeAssets(*request.package));
		}
	}

	// ---------------------------------------------------

	ErrorCode FlatBufferPackageProvider::DeserializeAssets(Package& package) {
		if (package.IsLoaded()) {
			return Error::None;
		}

		MappedFile diskIndex, diskBlob;
		Stopwatch  loadTimer;
		Path<>     path;

		{
			const ErrorCode result(diskIndex.Open(MappedFile::Read, path.Assign(KnownDirectory::Packages, L"{}.e2index", package.GetPath())));
			if (Failed(result)) {
				_log.Write(MessageType::Error, "\tError opening {}: {}; aborting load." UTF8_NEWLINE, path, result);
				return result;
			}
		}
		{
			const ErrorCode result(diskBlob.Open(MappedFile::Read, path.Assign(KnownDirectory::Packages, L"{}", package.GetPath())));
			if (Failed(result)) {
				_log.Write(MessageType::Error, "\tError opening {}: {}; aborting load." UTF8_NEWLINE, path, result);
				return result;
			}
		}

		/*	Ensure the data we're working with can plausibly represent a package index. Since verification of asset contents requires specific knowledge of asset
		 *	structure it is left as an exercise to listeners instead of being performed here. */
		Verifier verifier(diskIndex.Get<const uint8_t>(0u), diskIndex.GetSizeInBytes());
		if (!VerifyPackageIndexBuffer(verifier)) {
			_log.Write(MessageType::Error, "\tPackage {} contains malformed index data; aborting load." UTF8_NEWLINE, package.GetPath());
			return Error::Unspecified;
		}

		const PackageIndex* index(GetPackageIndex(diskIndex.Get<const uint8_t>(0u)));
		Package::AssetList  assets(MallocAllocator("Package Asset List Allocator"));
		for (const AssetDescriptor* sourceAsset : *index->Assets()) {
			if (diskBlob.GetSizeInBytes() < sourceAsset->Offset() + sourceAsset->Length()) {
				_log.Write(MessageType::Error, "\tAsset {} in package {} references out-of-bounds data!." UTF8_NEWLINE, sourceAsset->Name()->c_str(), package.GetPath());
				continue;
			}

			Asset::Builder       builder(_log, diskBlob.GetRange<const char>(sourceAsset->Offset(), sourceAsset->Length()));
			UniquePointer<Asset> asset(_assetDatabase.Insert(package, StringView<Utf8Char>(sourceAsset->Name()->c_str(), sourceAsset->Name()->size())));
			if (asset && Succeeded(asset->BindResources(builder))) {
				assets.Append(eastl::move(asset));
			}
		}

		_log.Write(MessageType::Message, "Deserialized package {} in {:.2f}ms ({} created/{} source(s))." UTF8_NEWLINE, package.GetPath(), AsMilliseconds(loadTimer.GetDuration()), assets.GetSize(), index->Assets()->size());

		assets.ShrinkToFit();
		package.BindAssets(eastl::move(assets));
	}

}} // namespace Eldritch2::Assets
