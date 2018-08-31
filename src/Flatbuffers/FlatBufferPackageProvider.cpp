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
#include <Flatbuffers/FlatBufferPackageProvider.hpp>
#include <Flatbuffers/FlatbufferTools.hpp>
#include <Logging/FileLog.hpp>
#include <Assets/Asset.hpp>
//------------------------------------------------------------------//
#include <Flatbuffers/PackageIndex_generated.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace FlatBuffers {

	using namespace ::Eldritch2::Assets::FlatBuffers;
	using namespace ::Eldritch2::Logging;
	using namespace ::Eldritch2::Assets;
	using namespace ::flatbuffers;

	namespace {

		ETCpp14Constexpr ETInlineHint ETForceInlineHint ETPureFunctionHint PlatformStringView GetFileName(PlatformStringView fullPath) ETNoexceptHint {
			// Overflow will wrap this to 0.
			const auto filePos(fullPath.FindLast(ET_WDIR_SEPARATOR) + 1u);
			return fullPath.Substring(filePos, fullPath.Find(ET_WEXTENSION_SEPARATOR, filePos));
		}

		// ---------------------------------------------------

		ErrorCode DeserializeAssets(Package& package, AssetDatabase& assetDatabase, Log& log) {
			MappedFile diskIndex, diskBlob;
			Stopwatch  loadTimer;
			{
				const Path path(MallocAllocator(), KnownDirectory::Packages, L"{}.e2index", package.GetPath());
				ET_ABORT_UNLESS_MSG(diskIndex.Open(MappedFile::Read, path), log, "\tError opening {}: {}; aborting load." ET_NEWLINE, path, result);
			}
			{
				const Path path(MallocAllocator(), KnownDirectory::Packages, package.GetPath());
				ET_ABORT_UNLESS_MSG(diskBlob.Open(MappedFile::Read, path), log, "\tError opening {}: {}; aborting load." ET_NEWLINE, path, result);
			}

			/*	Ensure the data we're working with can plausibly represent a package index. Since verification of asset contents requires specific knowledge of asset
			 *	structure it is left as an exercise to listeners instead of being performed here. */
			Verifier verifier(diskIndex.Get<const uint8_t>(0u), diskIndex.GetSizeInBytes());
			ET_ABORT_UNLESS_MSG(VerifyPackageIndexBuffer(verifier) ? Error::None : Error::Unspecified, log, "\tPackage {} contains malformed index data; aborting load." ET_NEWLINE, package.GetPath());

			const auto         sourceAssets(GetPackageIndex(diskIndex.Get<const uint8_t>(0u))->Assets());
			Package::AssetList assets(MallocAllocator("Package Asset List Allocator"), sourceAssets->size());
			for (uoffset_t id(0u); id < sourceAssets->size(); ++id) {
				const AssetDescriptor* const source(sourceAssets->Get(id));
				if (diskBlob.GetSizeInBytes() - source->Length() < source->Offset()) {
					log.Write(Severity::Error, "\tAsset {} in package {} references out-of-bounds data!." ET_NEWLINE, AsString(source->Name()), package.GetPath());
					continue;
				}

				Asset::Builder       builder(log, diskBlob.GetRange<const char>(source->Offset(), source->Length()));
				UniquePointer<Asset> asset(assetDatabase.Insert(package, AsString(source->Name())));
				if (asset && Succeeded(asset->BindResources(builder))) {
					assets.Append(eastl::move(asset));
				}
			}

			log.Write(Severity::Message, "Deserialized package {} in {:.2f}ms ({} created/{} source(s))." ET_NEWLINE, package.GetPath(), AsMilliseconds(loadTimer.GetDuration()), assets.GetSize(), sourceAssets->size());
			package.BindAssets(eastl::move(assets));

			return Error::None;
		}

	} // anonymous namespace

	FlatBufferPackageProvider::FlatBufferPackageProvider() :
		_assetDatabase(),
		_packageDatabase(),
		_runBehavior(Continue) {}

	// ---------------------------------------------------

	ErrorCode FlatBufferPackageProvider::BindResources() {
		EnsureDirectoryExists(KnownDirectory::Logs);
		return this->Boot("Package Loader Thread");
	}

	// ---------------------------------------------------

	void FlatBufferPackageProvider::FreeResources() {
		AwaitCompletion();
	}

	// ---------------------------------------------------

	void FlatBufferPackageProvider::SetShouldShutDown() ETNoexceptHint {
		_runBehavior.store(Terminate, std::memory_order_release);
	}

	// ---------------------------------------------------

	ErrorCode FlatBufferPackageProvider::EnterOnCaller() {
		PackageDatabase::LoadableSet packages(MallocAllocator("Package Set Allocator"));
		const Path                   specifier(MallocAllocator("FlatBuffer Package Specifier"), KnownDirectory::Packages, L"*.e2index");
		FileLog                      log;

		Move(KnownDirectory::Logs, L"ContentLog.old.txt", L"ContentLog.txt");
		ET_ABORT_UNLESS(log.BindResources(L"ContentLog.txt"));
		ET_AT_SCOPE_EXIT(log.Write(Severity::Message, "\t======================================================" ET_NEWLINE // clang-format off
													  "\t| TERMINATING LOG                                    |" ET_NEWLINE
													  "\t======================================================" ET_NEWLINE); // clang-format on
						 log.FreeResources());

		log.Write(Severity::Message, "\t======================================================" ET_NEWLINE // clang-format off
									 "\t| INITIALIZING LOG                                   |" ET_NEWLINE
									 "\t======================================================" ET_NEWLINE); // clang-format on
		log.Write(Severity::Message, "Scanning packages." ET_NEWLINE);

		Stopwatch timer;
		ForEachFile(specifier, [&](PlatformStringView path) {
			const String name(MallocAllocator(), GetFileName(path));
			if (Package::MaxPathLength < name.GetLength()) {
				log.Write(Severity::Message, "\tPackage name '{}' exceeds maximum length {}; ignoring." ET_NEWLINE, name, Package::MaxPathLength);
				return;
			}

			log.Write(Severity::Message, "\tDiscovered package '{}'." ET_NEWLINE, name);
			packages.Emplace(name);
		});

		log.Write(Severity::Message, "Package scan complete in {:.2f}ms; found {} package(s)." ET_NEWLINE // clang-format off
									 "\t======================================================" ET_NEWLINE, AsMilliseconds(timer.GetDuration()), packages.GetSize()); // clang-format on

		ET_ABORT_UNLESS(_packageDatabase.BindResources(eastl::move(packages)));
		while (ShouldRun()) {
			PackageDatabase::LoadRequest request;

			if (_packageDatabase.PopRequest(request)) {
				request.callback(eastl::move(request.package), DeserializeAssets(*request.package, _assetDatabase, log));
			}
		}

		return Error::None;
	}

}} // namespace Eldritch2::FlatBuffers
