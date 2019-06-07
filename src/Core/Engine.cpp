/*==================================================================*\
  Engine.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Core/PropertyApiBuilder.hpp>
#include <Scheduling/JobExecutor.hpp>
#include <Core/IniParser.hpp>
#include <Core/Profiler.hpp>
#include <Core/Engine.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Core {

	using namespace ::Eldritch2::Scheduling;
	using namespace ::Eldritch2::Scripting;
	using namespace ::Eldritch2::Logging;

	// ---------------------------------------------------

	AbstractEngine::AbstractEngine() ETNoexceptHint : _allocator("Engine Allocator"), _shouldRun(true), _worlds(WorldList::AllocatorType("Engine World List Allocator")) {}

	// ---------------------------------------------------

	void AbstractEngine::TickWorlds(JobExecutor& executor) ETNoexceptHint {
		enum : size_t { HighParallelismSplit = 1u };

		ET_PROFILE_SCOPE("Engine/WorldManagement", "Tick worlds", 0xF32334);
		{
			ET_PROFILE_SCOPE("Engine/WorldManagement", "Garbage collect worlds", 0x3422F3);

			ET_LOCK_SCOPE(_worldsMutex);
			// Remove any worlds that have marked themselves for deletion.
			executor.ForEach<HighParallelismSplit>(_worlds.Begin(), _worlds.End(), [this](JobExecutor& executor, UniquePointer<AbstractWorld>& world) ETNoexceptHint {
				if (world->ShouldShutDown()) {
					/*	Note that there will (temporarily) be a null pointer in the world collection until these entries are removed at the end
					 *	of the sweep phase-- see below. */
					world->FreeResources(executor);
					world.Reset();
				}
			});

			//	Remove all deleted worlds from the list.
			_worlds.EraseIf([](const UniquePointer<AbstractWorld>& world) ETNoexceptHint { return world == nullptr; });

			if (_worlds.IsEmpty()) {
				_log.Write(Severity::Message, "No running worlds, shutting down engine." ET_NEWLINE);
				SetShouldShutDown();
			}
			// End of lock scope.
		}

		// Tick all remaining worlds.
		executor.ForEach<HighParallelismSplit>(_worlds.Begin(), _worlds.End(), [this](JobExecutor& executor, UniquePointer<AbstractWorld>& world) ETNoexceptHint {
			ET_PROFILE_SCOPE("Engine/WorldManagement", "Tick single world", 0xF32334);

			world->Tick(executor);
		});
	}

	// ---------------------------------------------------

	void AbstractEngine::ApplyProperties(JobExecutor& executor, PlatformStringSpan filePath) ETNoexceptHint {
		const Path path(Path::AllocatorType(), KnownDirectory::UserDocuments, filePath);
		FileView   values;

		if (Succeeded(values.Open(FileView::Read, path))) {
			_log.Write(Severity::Message, "Applying user configuration from file {}." ET_NEWLINE, path);

			values.Prefetch(0u, FileView::EndOfFile);
			//	Give the disk time to page in the configuration data by registering client properties.
			PropertyApiBuilder propertyApi(BuildApi(PropertyApiBuilder()));
			ParseIni(values.StringSlice<Utf8Char>(0u, values.GetByteSize()), [this, &properties = propertyApi.GetProperties()](StringSpan group, StringSpan name, StringSpan value) ETNoexceptHint {
				if (!properties.SetValue(group, name, value)) {
					_log.Write(Severity::Warning, "Unknown configuration key '[{}] {} (= {})'." ET_NEWLINE, group, name, value);
				}
			});
		}

		BindConfigurableResources(executor);
	}

	// ---------------------------------------------------

	Result AbstractEngine::BindResources(JobExecutor& executor) ETNoexceptHint {
		ET_PROFILE_SCOPE("Engine/Initialization", "Initialization", 0xAAAAAA);

		Stopwatch bindTimer;
		// Back up the previous log, if present.
		Move(KnownDirectory::Logs, SL("EngineLog.old.txt"), SL("EngineLog.txt"));
		ET_ABORT_UNLESS(_log.BindResources(SL("EngineLog.txt"), "\t======================================================" ET_NEWLINE // clang-format off
																"\t| INITIALIZING APPLICATION                           |" ET_NEWLINE
																"\t======================================================" ET_NEWLINE)); // clang-format on
		_services = BuildApi(ObjectInjector(*this, _localization, static_cast<Log&>(_log)));
		BindComponents(executor);
		CreateWorld(executor);

		_log.Write(
			Severity::Message,
			"Engine initialization complete in {:.2f}ms." ET_NEWLINE
			"\t======================================================" ET_NEWLINE,
			bindTimer.GetDurationMilliseconds());

		return Result::Success;
	}

	// ---------------------------------------------------

	void AbstractEngine::FreeResources(JobExecutor& executor) ETNoexceptHint {
		FreeComponents(executor);
		_log.FreeResources("\t======================================================" ET_NEWLINE // clang-format off
						   "\t| TERMINATING APPLICATION                            |" ET_NEWLINE
						   "\t======================================================" ET_NEWLINE); // clang-format on
	}

}} // namespace Eldritch2::Core
