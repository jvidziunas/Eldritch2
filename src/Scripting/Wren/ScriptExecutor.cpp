/*==================================================================*\
  ScriptExecutor.cpp
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
#include <Scripting/Wren/AssetViews/ScriptAsset.hpp>
#include <Scripting/Wren/ScriptExecutor.hpp>
#include <Scripting/Wren/WrenInternal.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
//------------------------------------------------------------------//
#include <wren.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Scripting { namespace Wren {

	namespace {

		static ETConstexpr WrenHandle* InvalidHandle(nullptr);

	} // anonymous namespace

	using namespace ::Eldritch2::Scripting::Wren::AssetViews;
	using namespace ::Eldritch2::Logging;
	using namespace ::Eldritch2::Core;

	// ---------------------------------------------------

	ScriptExecutor::ScriptExecutor() ETNoexceptHint : _assets(nullptr),
													  _classByCppType(MallocAllocator("Wren Class By Type Allocator")),
													  _callStubByArity{ InvalidHandle },
													  _newStubByArity{ InvalidHandle } {}

	// ---------------------------------------------------

	ScriptExecutor::~ScriptExecutor() {
		for (WrenHandle* stub : _callStubByArity) {
			ETAssert(stub == InvalidHandle, "Leaking Wren call stub {}!", fmt::ptr(stub));
		}

		for (WrenHandle* stub : _newStubByArity) {
			ETAssert(stub == InvalidHandle, "Leaking Wren new stub {}!", fmt::ptr(stub));
		}
	}

	// ---------------------------------------------------

	bool ScriptExecutor::Interpret(const char* source) ETNoexceptHint {
		return wrenInterpret(_vm, "_main", source) == WREN_RESULT_SUCCESS;
	}

	// ---------------------------------------------------

	ApiBuilder ScriptExecutor::CreateApiBuilder(uint32 heapByteSize) {
		WrenConfiguration configuration;
		wrenInitConfiguration(ETAddressOf(configuration));
		configuration.userData            = this;
		configuration.bindForeignClassFn  = nullptr;
		configuration.bindForeignMethodFn = nullptr;
		configuration.initialHeapSize     = heapByteSize;
		configuration.minHeapSize         = heapByteSize;
		configuration.reallocateFn        = [](void* allocation, size_t byteSize) ETNoexceptHint {
            return _aligned_realloc(allocation, byteSize, /*alignment =*/16u);
		};

		configuration.loadModuleFn = [](WrenVM* vm, const char* module) ETNoexceptHint -> char* {
			String     path(MallocAllocator(), module, FindTerminator(module));
			const auto asset(Get<ScriptAsset>(GetExecutor(vm)->_assets->Find(path.EnsureSuffix(ScriptAsset::GetExtension()))));
			if (!asset) {
				return nullptr;
			}

			//	The initial script source is null-terminated, though we need to make sure to allocate room for the terminator here.
			char* const script(static_cast<char*>(_aligned_malloc(asset->GetLength() + 1u, 16u)));
			if (script) {
				CopyAndTerminate(asset->Begin(), asset->End(), script, '\0');
			}

			return script;
		};

		configuration.writeFn = [](WrenVM* vm, const char* text) ETNoexceptHint {
			GetExecutor(vm)->_log.Write(text, StringLength(text));
		};

		configuration.errorFn = [](WrenVM* vm, WrenErrorType type, const char* module, int line, const char* message) ETNoexceptHint {
			switch (type) {
			case WREN_ERROR_COMPILE: GetExecutor(vm)->_log.Write(Severity::Error, "{} ({}:{})" ET_NEWLINE, message, module, line); break;
			case WREN_ERROR_RUNTIME: GetExecutor(vm)->_log.Write(Severity::Error, "Wren runtime error: {}" ET_NEWLINE, message); break;
			case WREN_ERROR_STACK_TRACE: GetExecutor(vm)->_log.Write(Severity::Error, "\tin {} ({}:{})" ET_NEWLINE, message, module, line); break;
			} // switch (type)
		};

		return ApiBuilder(wrenNewVM(ETAddressOf(configuration)));
	}

	// ---------------------------------------------------

	Result ScriptExecutor::BindResources(Log& log, const AssetDatabase& assetDatabase, ApiBuilder api) {
		using ::Eldritch2::Swap;

		static ETConstexpr const char* CallSignatures[ETCountOf(_callStubByArity)] = {
			"call()",
			"call(_)",
			"call(_,_)",
			"call(_,_,_)",
			"call(_,_,_,_)",
			"call(_,_,_,_,_)",
			"call(_,_,_,_,_,_)",
			"call(_,_,_,_,_,_,_)"
		};

		// ---

		static ETConstexpr const char* NewSignatures[ETCountOf(_newStubByArity)] = {
			"new()",
			"new(_)",
			"new(_,_)",
			"new(_,_,_)",
			"new(_,_,_,_)",
			"new(_,_,_,_,_)",
			"new(_,_,_,_,_,_)",
			"new(_,_,_,_,_,_,_)"
		};

		ET_ABORT_UNLESS(_log.BindResources(log));

		const AssetDatabase* assets(ETAddressOf(assetDatabase));
		WrenHandle*          callStubByArity[ETCountOf(CallSignatures)] = { nullptr };
		ET_FOREACH_AT_SCOPE_EXIT(stub, callStubByArity, wrenReleaseHandle(api.GetVm(), stub));
		Transform(Begin(CallSignatures), End(CallSignatures), callStubByArity, [vm = api.GetVm()](const char* const signature) ETNoexceptHint {
			return wrenMakeCallHandle(vm, signature);
		});

		WrenHandle* newStubByArity[ETCountOf(NewSignatures)] = { nullptr };
		ET_FOREACH_AT_SCOPE_EXIT(stub, newStubByArity, wrenReleaseHandle(api.GetVm(), stub));
		Transform(Begin(NewSignatures), End(NewSignatures), newStubByArity, [vm = api.GetVm()](const char* const signature) ETNoexceptHint {
			return wrenMakeCallHandle(vm, signature);
		});

		Swap(_assets, assets);
		Swap(Tie(_vm, _classByCppType), Tie(api.GetVm(), api.GetClasses()));
		Swap(_callStubByArity, callStubByArity);
		Swap(_newStubByArity, newStubByArity);

		return Result::Success;
	}

	// ---------------------------------------------------

	void ScriptExecutor::FreeResources() ETNoexceptHint {
		ForEach(_newStubByArity, [vm = _vm](WrenHandle* stub) ETNoexceptHint { wrenReleaseHandle(vm, stub); });
		ForEach(_callStubByArity, [vm = _vm](WrenHandle* stub) ETNoexceptHint { wrenReleaseHandle(vm, stub); });
		_classByCppType.ClearAndDispose([vm = _vm](WrenHandle* wrenClass) ETNoexceptHint { wrenReleaseHandle(vm, wrenClass); });

		if (WrenVM* vm = Exchange(_vm, nullptr)) {
			wrenFreeVM(vm);
		}
	}

	// ---------------------------------------------------

	void Swap(ScriptExecutor& lhs, ScriptExecutor& rhs) ETNoexceptHint {
		using ::Eldritch2::Swap;

		Swap(lhs._log, rhs._log);
		Swap(lhs._assets, rhs._assets);
		Swap(lhs._vm, rhs._vm);
		Swap(lhs._classByCppType, rhs._classByCppType);
		Swap(lhs._callStubByArity, rhs._callStubByArity);

		// For sanity, update userdata *after* VM has been installed.
		if (lhs._vm) {
			wrenSetUserData(lhs._vm, ETAddressOf(lhs));
		}

		if (rhs._vm) {
			wrenSetUserData(rhs._vm, ETAddressOf(rhs));
		}
	}

}}} // namespace Eldritch2::Scripting::Wren
