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
#include <Assets/ContentLocator.hpp>
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

	WrenWorldComponent::WrenWorldComponent(const ObjectLocator& services) :
		WorldComponent(services),
		_vm(nullptr),
		_wren(*FindService<AssetDatabase>(), FindService<World>()->GetLog()),
		_contentLocator(nullptr),
		_dispatcher(nullptr) {
	}

	// ---------------------------------------------------

	void WrenWorldComponent::BindResourcesEarly(JobExecutor& /*executor*/) {
		ET_PROFILE_SCOPE("World/EarlyInitialization", "Wren API registration", 0xCBABCB);
		WrenConfiguration configuration;

		wrenInitConfiguration(ETAddressOf(configuration));
		configuration.userData            = ETAddressOf(_wren);
		configuration.bindForeignClassFn  = nullptr;
		configuration.bindForeignMethodFn = nullptr;
		configuration.initialHeapSize     = 2u * 1024u * 1024u /* 2MB*/;
		configuration.minHeapSize         = 2u * 1024u * 1024u /* 2MB*/;
		configuration.reallocateFn        = [](void* memory, size_t size) {
            return _aligned_realloc(memory, size, 16u);
		};

		configuration.loadModuleFn = [](WrenVM* vm, const char* module) -> char* {
			String path;
			path.Append(module, FindTerminator(module)).Append(ScriptAsset::GetExtension());

			const ScriptAsset* script(GetContext(vm)->FindScriptModule(path));
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
			GetContext(vm)->WriteLog(text, StringLength(text));
		};

		configuration.errorFn = [](WrenVM* vm, WrenErrorType type, const char* module, int line, const char* message) {
			switch (type) {
			case WREN_ERROR_COMPILE: GetContext(vm)->WriteLog(Severity::Error, "{} ({}:{})" ET_NEWLINE, message, module, line); break;
			case WREN_ERROR_RUNTIME: GetContext(vm)->WriteLog(Severity::Error, "Wren runtime error: {}" ET_NEWLINE, message); break;
			case WREN_ERROR_STACK_TRACE: GetContext(vm)->WriteLog(Severity::Error, "\tin {} ({}:{})" ET_NEWLINE, message, module, line); break;
			} // switch (type)
		};

		WrenVM* const vm(wrenNewVM(ETAddressOf(configuration)));
		_wren.BindResources(vm);

		//	Build the list of application script types. This is single-threaded to avoid contention on the type and method tables.
		ApiBuilder api(vm, _wren);
		for (const UniquePointer<WorldComponent>& component : FindService<World>()->GetComponents()) {
			component->DefineScriptApi(api);
		}

		_vm = vm;
	}

	// ---------------------------------------------------

	void WrenWorldComponent::BindResources(JobExecutor& /*executor*/) {
		ET_PROFILE_SCOPE("World/Initialization", "Wren world boot", 0xCBABCB);
		if (Failed(_contentLocator->BindResources()) || Failed(_game.BindResources(_vm))) {
			FindService<World>()->SetShouldShutDown();
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

		if (ContentLocator* const locator = eastl::exchange(_contentLocator, nullptr)) {
			locator->FreeResources();
		}

		_game.FreeResources(vm);
		_wren.FreeResources(vm);
		wrenFreeVM(vm);
	}

	// ---------------------------------------------------

	void WrenWorldComponent::OnVariableRateTick(JobExecutor& /*executor*/, MicrosecondTime /*tickDuration*/, float32 /*residualFraction*/) {
		ET_PROFILE_SCOPE("World/Tick", "Wren script execution", 0xCBABCB);
		_contentLocator->RequirementsComplete();
	}

	// ---------------------------------------------------

	void WrenWorldComponent::OnFixedRateTick(JobExecutor& /*executor*/, MicrosecondTime delta) {
		ET_PROFILE_SCOPE("World/Tick", "Wren script execution", 0xCBABCB);
		_dispatcher->ExecuteScriptEvents(_vm, delta);
	}

}}} // namespace Eldritch2::Scripting::Wren
