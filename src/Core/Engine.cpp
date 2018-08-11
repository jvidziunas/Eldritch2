/*==================================================================*\
  Engine.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scheduling/JobExecutor.hpp>
#include <Core/PropertyRegistrar.hpp>
#include <Scheduling/JobSystem.hpp>
#include <Core/IniParser.hpp>
#include <Core/Engine.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Core {

	using namespace ::Eldritch2::Scheduling;
	using namespace ::Eldritch2::Scripting;
	using namespace ::Eldritch2::Logging;
	using namespace ::Eldritch2::Assets;

	namespace {

		enum : size_t {
			HighParallelismSplit = 1u,
			LowParallelismSplit  = 2u
		};

		// ---

		ETInlineHint ETPureFunctionHint float32 AsMilliseconds(MicrosecondTime delta) {
			static constexpr float32 MicrosecondsPerMillisecond = 1000.0f;
			return AsFloat(delta) / MicrosecondsPerMillisecond;
		}

	} // anonymous namespace

	Engine::Engine(
		JobSystem& jobSystem) :
		_allocator("Game Engine Allocator"),
		_services(),
		_packageProvider(),
		_log(),
		_shouldRun(true),
		_worlds(MallocAllocator("Game Engine World List Allocator")),
		_components(MallocAllocator("Game Engine Component List Allocator")) {
		//	Create the bootstrap set of services.
		_services.PublishService<PackageDatabase>(_packageProvider.GetPackageDatabase());
		_services.PublishService<AssetDatabase>(_packageProvider.GetAssetDatabase());
		_services.PublishService<JobSystem>(jobSystem);
		_services.PublishService<Engine>(*this);
	}

	// ---------------------------------------------------

	ErrorCode Engine::CreateWorld(JobExecutor& executor) {
		UniquePointer<World> world(MakeUnique<World>(_allocator, GetServiceLocator()));
		ET_FAIL_UNLESS(world->BindResources(executor, _components.Begin(), _components.End()));

		{
			Lock _(_worldsMutex);
			_worlds.Append(eastl::move(world));
		} // End of lock scope.

		return Error::None;
	}

	// ---------------------------------------------------

	void Engine::TickWorlds(JobExecutor& executor) {
		ET_PROFILE_SCOPE("Engine/WorldManagement", "Tick worlds", 0xF32334);

		{
			ET_PROFILE_SCOPE("Engine/WorldManagement", "Garbage collect worlds", 0x3422F3);
			Lock _(_worldsMutex);

			executor.ForEach<HighParallelismSplit>(_worlds.Begin(), _worlds.End(), [this](JobExecutor& executor, UniquePointer<World>& world) {
				if (world->ShouldShutDown()) {
					/*	Note that there will (temporarily) be a null pointer in the world collection until these entries are removed at the end
					 *	of the sweep phase-- see below. */
					world->FreeResources(executor);
					world.Reset();
				}
			});

			//	Remove all null entries from the worlds collection.
			_worlds.EraseIf([](UniquePointer<World>& world) { return world == nullptr; });

			if (_worlds.IsEmpty()) {
				_log.Write(MessageType::Message, "No running worlds, shutting down engine." UTF8_NEWLINE);
				return SetShouldShutDown();
			}
		} // End of profile/lock scope.

		executor.ForEach<HighParallelismSplit>(_worlds.Begin(), _worlds.End(), [this](JobExecutor& executor, UniquePointer<World>& world) {
			world->Tick(executor);
		});
	}

	// ---------------------------------------------------

	ErrorCode Engine::ApplyConfiguration(StringView<PlatformChar> path) {
		MappedFile iniFile;
		Path<>     absolutePath;
		{
			const ErrorCode result(iniFile.Open(MappedFile::Read, absolutePath.Assign(KnownDirectory::UserDocuments, path)));
			if (Failed(result)) {
				_log.Write(MessageType::Warning, "Error opening {}, skipping configuration." UTF8_NEWLINE, path);
				return result;
			}
		}

		iniFile.Prefetch(0u, MappedFile::EndOfFile);

		//	Give the disk time to page in the configuration data by registering client properties.
		PropertyDatabase properties;
		{
			PropertyRegistrar registrar(properties);
			for (EngineComponent* component : _components) {
				component->PublishConfiguration(registrar);
			}
		}

		auto SetPropertyValue([&](StringView<Utf8Char> group, StringView<Utf8Char> name, StringView<Utf8Char> value) -> bool {
			return properties.SetValue(group, name, value);
		});

		auto LogUnknownProperty([&](StringView<Utf8Char> group, StringView<Utf8Char> name, StringView<Utf8Char> /*value*/) {
			_log.Write(MessageType::Warning, "Unknown configuration key '[{}] {}'." UTF8_NEWLINE, group, name);
		});

		ApplyIni(iniFile.GetRange<const Utf8Char>(0u, iniFile.GetSizeInBytes()), SetPropertyValue, LogUnknownProperty);

		return Error::None;
	}

	// ---------------------------------------------------

	void Engine::SweepPackages(size_t collectionLimit) {
		_packageProvider.GetPackageDatabase().DestroyGarbage(_packageProvider.GetAssetDatabase(), collectionLimit);
	}

	// ---------------------------------------------------

	void Engine::ScanPackages() {
		Stopwatch timer;

		_packageProvider.ScanPackages();

		_log.Write(MessageType::Message, "Rebuilt package listing in {:.2f}ms." UTF8_NEWLINE, AsMilliseconds(timer.GetDuration()));
	}

	// ---------------------------------------------------

	int Engine::BootOnCaller(JobExecutor& executor) {
		if (Failed(_packageProvider.BindResources())) {
			return -1;
		}
		ET_AT_SCOPE_EXIT(_packageProvider.FreeResources());

		Move(KnownDirectory::Logs, L"EngineLog.old.txt", L"EngineLog.txt");
		_log.BindResources(L"EngineLog.txt");
		ET_AT_SCOPE_EXIT(_log.FreeResources());

		_log.Write(MessageType::Message, "\t======================================================" UTF8_NEWLINE);
		_log.Write(MessageType::Message, "\t| INITIALIZING APPLICATION                           |" UTF8_NEWLINE);
		_log.Write(MessageType::Message, "\t======================================================" UTF8_NEWLINE);

		BindComponents(executor);
		CreateBootWorld(executor);

		while (ShouldRun()) {
			ET_PROFILE_FRAME_BEGIN_CPU();
			RunFrame(executor);
		}

		_log.Write(MessageType::Message, "\t======================================================" UTF8_NEWLINE);
		_log.Write(MessageType::Message, "\t| TERMINATING APPLICATION                            |" UTF8_NEWLINE);
		_log.Write(MessageType::Message, "\t======================================================" UTF8_NEWLINE);

		return 0;
	}

	// ---------------------------------------------------

	void Engine::BindComponents(JobExecutor& executor) {
		ET_PROFILE_SCOPE("Engine/Initialization", "Initialization", 0xAAAAAA);
		Stopwatch timer;

		//	Complete all initialization steps on each of the attached services.
		executor.ForEach<HighParallelismSplit>(_components.Begin(), _components.End(), [](JobExecutor& executor, EngineComponent* component) {
			component->BindResourcesEarly(executor);
		});
		executor.ForEach<HighParallelismSplit>(_components.Begin(), _components.End(), [](JobExecutor& executor, EngineComponent* component) {
			component->BindResources(executor);
		});

		_log.Write(MessageType::Message, "Engine initialization complete in {:.2f}ms." UTF8_NEWLINE, AsMilliseconds(timer.GetDuration()));
		_log.Write(MessageType::Message, "\t======================================================" UTF8_NEWLINE);
	}

	// ---------------------------------------------------

	void Engine::CreateBootWorld(JobExecutor& executor) {
		const ErrorCode result(CreateWorld(executor));
		if (Failed(result)) {
			_log.Write(MessageType::Error, "Failed to create boot world: {}!" UTF8_NEWLINE, AsCString(result));
		}
	}

	// ---------------------------------------------------

	void Engine::RunFrame(JobExecutor& executor) {
		ET_PROFILE_SCOPE("Engine/Frame", "Frame", 0xAAAAAA);

		//	Engine component tick is run in sequence before world tick; we want all shared state to be updated *before* world tick happens.
		{
			ET_PROFILE_SCOPE("Engine/Frame", "Engine Tick", 0xEEEEEE);
			executor.ForEach<LowParallelismSplit>(_components.Begin(), _components.End(), [](JobExecutor& executor, EngineComponent* component) {
				component->TickEarly(executor);
			});
		} // End of profile scope.

		{
			ET_PROFILE_SCOPE("Engine/Frame", "World Tick", 0xFFFFFF);
			executor.ForEach<LowParallelismSplit>(_components.Begin(), _components.End(), [](JobExecutor& executor, EngineComponent* component) {
				component->Tick(executor);
			});
		} // End of profile scope.
	}

}} // namespace Eldritch2::Core
