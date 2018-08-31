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
#include <Core/PropertyRegistrar.hpp>
#include <Scheduling/JobExecutor.hpp>
#include <Core/IniParser.hpp>
#include <Core/Engine.hpp>
//------------------------------------------------------------------//

#if defined(CreateDirectory)
#	undef CreateDirectory
#endif

namespace Eldritch2 { namespace Core {

	using namespace ::Eldritch2::Scheduling;
	using namespace ::Eldritch2::Scripting;
	using namespace ::Eldritch2::Logging;
	using namespace ::Eldritch2::Assets;

	namespace {

		enum : size_t {
			HighParallelismSplit = 2u,
			LowParallelismSplit  = 4u
		};

		// ---

		ETInlineHint ETForceInlineHint ETPureFunctionHint float32 AsMilliseconds(MicrosecondTime delta) {
			return AsFloat(delta) / /*microseconds per millisecond*/ 1000.0f;
		}

	} // anonymous namespace

	Engine::Engine() :
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
		_services.PublishService<Engine>(*this);
	}

	// ---------------------------------------------------

	ErrorCode Engine::CreateWorld(JobExecutor& executor) {
		UniquePointer<World> world(MakeUnique<World>(_allocator, GetServiceLocator()));
		ET_ABORT_UNLESS(world->BindResources(executor, _components.Begin(), _components.End()));
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
				_log.Write(Severity::Message, "No running worlds, shutting down engine." ET_NEWLINE);
				return SetShouldShutDown();
			}
		} // End of profile/lock scope.
		executor.ForEach<HighParallelismSplit>(_worlds.Begin(), _worlds.End(), [this](JobExecutor& executor, UniquePointer<World>& world) {
			world->Tick(executor);
		});
	}

	// ---------------------------------------------------

	ErrorCode Engine::ApplyConfiguration(PlatformStringView filePath) {
		const Path path(MallocAllocator(), KnownDirectory::UserDocuments, filePath);
		MappedFile iniFile;
		if (Failed(iniFile.Open(MappedFile::Read, path))) {
			_log.Write(Severity::Warning, "Error opening {}, skipping configuration." ET_NEWLINE, path);
			return Error::None;
		}

		iniFile.Prefetch(0u, MappedFile::LengthOfFile);
		//	Give the disk time to page in the configuration data by registering client properties.
		PropertyDatabase properties;
		{
			PropertyRegistrar registrar(properties);
			for (EngineComponent* component : _components) {
				component->PublishConfiguration(registrar);
			}
		}

		auto SetPropertyValue([&](StringView group, StringView name, StringView value) -> bool {
			return properties.SetValue(group, name, value);
		});

		auto LogUnknownProperty([&](StringView group, StringView name, StringView /*value*/) {
			_log.Write(Severity::Warning, "Unknown configuration key '[{}] {}'." ET_NEWLINE, group, name);
		});

		ParseIni({ iniFile.Get<const Utf8Char>(0u), iniFile.GetSizeInBytes() }, SetPropertyValue, LogUnknownProperty);

		return Error::None;
	}

	// ---------------------------------------------------

	ErrorCode Engine::BootOnCaller(JobExecutor& executor) {
		EnsureDirectoryExists(KnownDirectory::Logs, L"");
		ET_ABORT_UNLESS(_packageProvider.BindResources());
		ET_AT_SCOPE_EXIT(_packageProvider.FreeResources());
		Move(KnownDirectory::Logs, L"EngineLog.old.txt", L"EngineLog.txt");
		ET_ABORT_UNLESS(_log.BindResources(L"EngineLog.txt"));
		ET_AT_SCOPE_EXIT(
			_log.Write(Severity::Message, "\t======================================================" ET_NEWLINE);
			_log.Write(Severity::Message, "\t| TERMINATING APPLICATION                            |" ET_NEWLINE);
			_log.Write(Severity::Message, "\t======================================================" ET_NEWLINE);
			_log.FreeResources());

		_log.Write(Severity::Message, "\t======================================================" ET_NEWLINE);
		_log.Write(Severity::Message, "\t| INITIALIZING APPLICATION                           |" ET_NEWLINE);
		_log.Write(Severity::Message, "\t======================================================" ET_NEWLINE);
		Stopwatch timer;
		BindComponents(executor);
		CreateBootWorld(executor);
		_log.Write(Severity::Message, "Engine initialization complete in {:.2f}ms." ET_NEWLINE, AsMilliseconds(timer.GetDuration()));
		_log.Write(Severity::Message, "\t======================================================" ET_NEWLINE);

		while (ShouldRun()) {
			ET_PROFILE_FRAME_BEGIN_CPU();
			RunFrame(executor);
		}

		return Error::None;
	}

	// ---------------------------------------------------

	void Engine::BindComponents(JobExecutor& executor) {
		ET_PROFILE_SCOPE("Engine/Initialization", "Initialization", 0xAAAAAA);

		executor.ForEach<HighParallelismSplit>(_components.Begin(), _components.End(), [](JobExecutor& executor, EngineComponent* component) {
			component->BindResourcesEarly(executor);
		});
		executor.ForEach<HighParallelismSplit>(_components.Begin(), _components.End(), [](JobExecutor& executor, EngineComponent* component) {
			component->BindResources(executor);
		});
	}

	// ---------------------------------------------------

	void Engine::CreateBootWorld(JobExecutor& executor) {
		const ErrorCode result(CreateWorld(executor));

		if (Failed(result)) {
			_log.Write(Severity::Error, "Failed to create boot world: {}!" ET_NEWLINE, result);
		}
	}

	// ---------------------------------------------------

	void Engine::RunFrame(JobExecutor& executor) {
		ET_PROFILE_SCOPE("Engine/Frame", "Frame", 0xAAAAAA);
		{
			//	Engine component tick is run in sequence before world tick; we want all shared state to be updated *before* world tick happens.
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
