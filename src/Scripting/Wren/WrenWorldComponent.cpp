/*==================================================================*\
  WrenWorldComponent.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Wren/AssetViews/ScriptAsset.hpp>
#include <Scripting/Wren/WrenWorldComponent.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
#include <Scripting/Wren/Dispatcher.hpp>
#include <Assets/PackageLocator.hpp>
#include <Assets/AssetDatabase.hpp>
#include <Scripting/Wren/Game.hpp>
#include <Core/World.hpp>
//------------------------------------------------------------------//
#include <microprofile/microprofile.h>
//------------------------------------------------------------------//
#include <wren.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Scripting { namespace Wren {

	using namespace ::Eldritch2::Scripting::Wren::AssetViews;
	using namespace ::Eldritch2::Scheduling;
	using namespace ::Eldritch2::Logging;
	using namespace ::Eldritch2::Assets;
	using namespace ::Eldritch2::Core;

	namespace {

		enum : size_t { WrenHeapSizeInBytes = 2u * 1024u * 1024u /* 2MB*/ };

	} // anonymous namespace

	WrenWorldComponent::WrenWorldComponent(const ObjectLocator& services) :
		WorldComponent(services),
		_vm(nullptr),
		_wren(FindService<AssetDatabase>(), FindService<World>().GetLog()),
		_packageLocator(nullptr),
		_assetLocator(nullptr),
		_dispatcher(nullptr) {
	}

	// ---------------------------------------------------

	void WrenWorldComponent::BindResourcesEarly(JobExecutor& /*executor*/) {
		ET_PROFILE_SCOPE("World/EarlyInitialization", "Wren API registration", 0xCBABCB);
		WrenConfiguration configuration;

		wrenInitConfiguration(&configuration);
		configuration.userData            = eastl::addressof(_wren);
		configuration.bindForeignClassFn  = nullptr;
		configuration.bindForeignMethodFn = nullptr;
		configuration.initialHeapSize     = WrenHeapSizeInBytes;
		configuration.minHeapSize         = WrenHeapSizeInBytes;
		configuration.reallocateFn        = [](void* memory, size_t newSize) {
            return _aligned_realloc(memory, newSize, 16u);
		};

		configuration.loadModuleFn = [](WrenVM* vm, const char* module) -> char* {
			char path[Asset::MaxPathLength];

			AppendString(CopyString(path, module), ScriptAsset::GetExtension());

			const ScriptAsset* script(AsContext(vm).FindScriptModule(path));
			if (!script) {
				return nullptr;
			}

			//	The initial script source is null-terminated, though we need to make sure to allocate room for the terminator here.
			char* const source(static_cast<char*>(_aligned_malloc(script->GetLength() + 1u, 16u)));
			if (source) {
				CopyAndTerminate(script->Begin(), script->End(), source, '\0');
			}

			return source;
		};

		configuration.writeFn = [](WrenVM* vm, const char* text) {
			AsContext(vm).WriteLog(text, StringLength(text));
		};

		configuration.errorFn = [](WrenVM* vm, WrenErrorType type, const char* module, int line, const char* message) {
			switch (type) {
			case WREN_ERROR_COMPILE: AsContext(vm).WriteLog(MessageType::Error, "{} ({}:{})" UTF8_NEWLINE, message, module, line); break;
			case WREN_ERROR_RUNTIME: AsContext(vm).WriteLog(MessageType::Error, "Wren runtime error: {}" UTF8_NEWLINE, message); break;
			case WREN_ERROR_STACK_TRACE: AsContext(vm).WriteLog(MessageType::Error, "\tin {} ({}:{})" UTF8_NEWLINE, message, module, line); break;
			} // switch (type)
		};

		WrenVM* const vm = wrenNewVM(&configuration);

		_wren.BindResources(vm);

		//	Build the list of application script types. This is single-threaded to avoid contention on the type and method tables.
		ApiBuilder api(vm, _wren);
		for (const UniquePointer<WorldComponent>& component : FindService<World>().GetComponents()) {
			component->DefineScriptApi(api);
		}

		_vm = vm;
	}

	// ---------------------------------------------------

	void WrenWorldComponent::BindResources(JobExecutor& /*executor*/) {
		ET_PROFILE_SCOPE("World/Initialization", "Wren world boot", 0xCBABCB);
		if (Failed(_packageLocator->BindResources())) {
			FindService<World>().SetShouldShutDown();
		}

		if (Failed(_game.BindResources(_vm))) {
			return FindService<World>().SetShouldShutDown();
		}
	}

	// ---------------------------------------------------

	void WrenWorldComponent::FreeResources(JobExecutor& /*executor*/) {
		WrenVM* const vm(eastl::exchange(_vm, nullptr));
		if (vm == nullptr) {
			return;
		}

		if (Dispatcher* const dispatcher = eastl::exchange(_dispatcher, nullptr)) {
			dispatcher->FreeResources(vm);
		}

		if (PackageLocator* const locator = eastl::exchange(_packageLocator, nullptr)) {
			locator->FreeResources();
		}

		_game.FreeResources(vm);
		_wren.FreeResources(vm);
		wrenFreeVM(vm);
	}

	// ---------------------------------------------------

	void WrenWorldComponent::OnVariableRateTick(Scheduling::JobExecutor& executor, MicrosecondTime tickDuration, float32 residualFraction) {
		ET_PROFILE_SCOPE("World/Tick", "Wren script execution", 0xCBABCB);
		_packageLocator->PollLoadStatus();
	}

	// ---------------------------------------------------

	void WrenWorldComponent::OnFixedRateTick(JobExecutor& /*executor*/, MicrosecondTime delta) {
		ET_PROFILE_SCOPE("World/Tick", "Wren script execution", 0xCBABCB);
		_dispatcher->ExecuteScriptEvents(_vm, delta);
	}

}}} // namespace Eldritch2::Scripting::Wren
