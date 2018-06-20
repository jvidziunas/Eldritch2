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

namespace Eldritch2 {
	namespace Assets {
		namespace {

			enum { PathTempBufferSize = Package::MaxPathLength + 32u };

		// ---

			ETInlineHint ETPureFunctionHint float32 AsMilliseconds(uint64 microseconds) {
				static constexpr float32	MicrosecondsPerMillisecond = 1000.0f;

				return microseconds / MicrosecondsPerMillisecond;
			}

		}	// anonymous namespace

		using namespace ::Eldritch2::Assets::FlatBuffers;
		using namespace ::Eldritch2::Logging;
		using namespace ::flatbuffers;

		FlatBufferPackageProvider::FlatBufferPackageProvider() : _log(), _assetDatabase(), _packageDatabase() {}

	// ---------------------------------------------------

		PackageDatabase& FlatBufferPackageProvider::GetPackageDatabase() {
			return _packageDatabase;
		}

	// ---------------------------------------------------

		AssetDatabase& FlatBufferPackageProvider::GetAssetDatabase() {
			return _assetDatabase;
		}

	// ---------------------------------------------------

		FileSystem& FlatBufferPackageProvider::GetFileSystem() {
			return _fileSystem;
		}

	// ---------------------------------------------------

		void FlatBufferPackageProvider::ScanPackages() {
			PackageDatabase::LoadableSet	packages(MallocAllocator("Loadable Package Collection Allocator"));
			String<>						indexFilter(MallocAllocator(""));

			_log.Write(MessageType::Message, "Scanning packages." UTF8_NEWLINE);

			indexFilter.Format("*{}", PackageIndexExtension());

			_fileSystem.EnumerateMatchingFiles(KnownDirectory::Packages, indexFilter.AsCString(), [this, &packages](const Utf8Char* path) {
				_log.Write(MessageType::Message, "\tDiscovered package '{}'." UTF8_NEWLINE, path);

				Utf8Char	name[Package::MaxPathLength];

			//	Strip the extension from the path and terminate.
				CopyAndTerminate(path, FindLastInstance(path, PackageIndexExtension()), name, '\0');

				packages.Emplace(name);
			});

			_log.Write(MessageType::Message, "Package scan complete; found {} package(s)." UTF8_NEWLINE, packages.GetSize());

			_packageDatabase.SetPackages(eastl::move(packages), [this](Package& package) {
				return this->Load(package);
			});
		}

	// ---------------------------------------------------

		ErrorCode FlatBufferPackageProvider::BindResources() {
			const PlatformString<>	logPath(_fileSystem.GetAbsolutePath(KnownDirectory::Logs, "ContentLog.txt"));

			_fileSystem.Move(KnownDirectory::Logs, "ContentLog.old.txt", "ContentLog.txt");
			ET_FAIL_UNLESS(_log.BindResources(logPath.AsCString()));

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

		void FlatBufferPackageProvider::Load(Package& package) {
			String<>	path(MallocAllocator("Path Allocator"));
			Stopwatch	loadTimer;

			_log.Write(MessageType::Message, "Loading package {}." UTF8_NEWLINE, package.GetPath());

		//	Open the index file.
			MappedFile indexFile;
			path.Format("{}{}", package.GetPath(), PackageIndexExtension());
			{	const ErrorCode result(indexFile.Open(MappedFile::Read, _fileSystem.GetAbsolutePath(KnownDirectory::Packages, path.AsCString()).AsCString()));
			if (Failed(result)) {
				_log.Write(MessageType::Error, "\tError opening {}: {}; aborting load." UTF8_NEWLINE, path.AsCString(), AsCString(result));
				return;
			}
			}

		//	Open the data blob.
			MappedFile blobFile;
			path.Assign(package.GetPath());
			{	const ErrorCode result(blobFile.Open(MappedFile::Read, _fileSystem.GetAbsolutePath(KnownDirectory::Packages, path.AsCString()).AsCString()));
			if (Failed(result)) {
				_log.Write(MessageType::Error, "\tError opening {}: {}; aborting load." UTF8_NEWLINE, path.AsCString(), AsCString(result));
				return;
			}
			}

		/*	Ensure the data we're working with can plausibly represent a package index. Since verification of asset contents requires specific knowledge of asset
		 *	structure it is left as an exercise to listeners instead of being performed here. */
			Verifier verifier(indexFile.GetAtOffset<const uint8_t>(MappedFile::StartOfFile), indexFile.GetSizeInBytes());
			if (!VerifyPackageIndexBuffer(verifier)) {
				_log.Write(MessageType::Error, "\tPackage {} contains malformed index data; aborting load." UTF8_NEWLINE, package.GetPath());
				return;
			}

			const PackageIndex&	index(*GetPackageIndex(indexFile.GetAtOffset<const uint8_t>(MappedFile::StartOfFile)));
			Package::AssetList	assets(MallocAllocator("Package Asset List Allocator"));

			_log.Write(MessageType::Message, "\tPackage {} contains {} asset(s)." UTF8_NEWLINE, package.GetPath(), index.Assets()->size());

			for (const AssetDescriptor* source : *index.Assets()) {
				if (blobFile.GetSizeInBytes() < (source->Offset() + source->Length())) {
					_log.Write(MessageType::Error, "\tAsset {} in package {} references out-of-bounds data!." UTF8_NEWLINE, source->Name()->c_str(), package.GetPath());
					continue;
				}

				if (UniquePointer<Asset> asset = _assetDatabase.CreateAsset(source->Name()->c_str())) {
					const auto begin(blobFile.GetAtOffset<const char>(source->Offset()));

					if (Succeeded(asset->BindResources(Asset::Builder(_log, begin, begin + source->Length())))) {
						assets.Append(eastl::move(asset));
					}
				}
			}

			_log.Write(MessageType::Message, "Load complete for package {} in {:.2f}ms ({} asset(s)/{} source(s))." UTF8_NEWLINE,
					   package.GetPath(), AsMilliseconds(loadTimer.GetDuration()), assets.GetSize(), index.Assets()->size()
			);

			package.BindAssets(eastl::move(assets));
		}

	}	// namespace Assets
}	// namespace Eldritch2