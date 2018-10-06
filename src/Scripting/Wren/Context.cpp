/*==================================================================*\
  Context.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Wren/AssetViews/ScriptAsset.hpp>
#include <Scripting/Wren/WrenInternal.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
#include <Scripting/Wren/Context.hpp>
#include <Assets/AssetDatabase.hpp>
//------------------------------------------------------------------//
#include <wren.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Scripting { namespace Wren {

	using namespace ::Eldritch2::Scripting::Wren::AssetViews;
	using namespace ::Eldritch2::Logging;
	using namespace ::Eldritch2::Assets;

	Context::Context() :
		_assets(nullptr),
		_classByCppType(MallocAllocator("Wren Class By Type Allocator")),
		_callStubByArity { nullptr } {}

	// ---------------------------------------------------

	Context::~Context() {
#if ET_DEBUG_BUILD
		for (WrenHandle* stub : _callStubByArity) {
			ET_ASSERT(stub == nullptr, "Leaking Wren call stub {}!", fmt::ptr(stub));
		}
#endif
	}

	// ---------------------------------------------------

	bool Context::Interpret(const char* source) {
		return wrenInterpret(_vm, source) == WREN_RESULT_SUCCESS;
	}

	// ---------------------------------------------------

	ErrorCode Context::BindResources(const AssetDatabase& assets, Log& parentLog, Function<void(ApiBuilder&)> apiBuilder) {
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

		WrenConfiguration configuration;
		wrenInitConfiguration(ETAddressOf(configuration));
		configuration.userData            = this;
		configuration.bindForeignClassFn  = nullptr;
		configuration.bindForeignMethodFn = nullptr;
		configuration.initialHeapSize     = 4u * 1024u * 1024u /* 2MB*/;
		configuration.minHeapSize         = 2u * 1024u * 1024u /* 2MB*/;
		configuration.reallocateFn        = [](void* previousAllocation, size_t byteSize) ETNoexceptHint {
            return _aligned_realloc(previousAllocation, byteSize, /*alignment =*/16u);
		};

		configuration.loadModuleFn = [](WrenVM* vm, const char* module) ETNoexceptHint -> char* {
			String path;
			path.Append(module, FindTerminator(module)).Append(ScriptAsset::GetExtension());

			const ScriptAsset* asset(static_cast<const ScriptAsset*>(GetContext(vm)->_assets->Find(path)));
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
			GetContext(vm)->_log.Write(text, StringLength(text));
		};

		configuration.errorFn = [](WrenVM* vm, WrenErrorType type, const char* module, int line, const char* message) ETNoexceptHint {
			switch (type) {
			case WREN_ERROR_COMPILE: GetContext(vm)->_log.Write(Severity::Error, "{} ({}:{})" ET_NEWLINE, message, module, line); break;
			case WREN_ERROR_RUNTIME: GetContext(vm)->_log.Write(Severity::Error, "Wren runtime error: {}" ET_NEWLINE, message); break;
			case WREN_ERROR_STACK_TRACE: GetContext(vm)->_log.Write(Severity::Error, "\tin {} ({}:{})" ET_NEWLINE, message, module, line); break;
			} // switch (type)
		};

		ChildLog log;
		ET_ABORT_UNLESS(log.BindResources(parentLog));

		WrenVM* vm(wrenNewVM(ETAddressOf(configuration)));
		ET_ABORT_UNLESS(vm ? Error::None : Error::Unspecified);
		ET_AT_SCOPE_EXIT(if (vm) wrenFreeVM(vm));

		ApiBuilder api(vm);
		apiBuilder(api);

		ClassMap classByCppType(api.ReleaseClasses());
		ET_AT_SCOPE_EXIT(for (ClassMap::ValueType& wrenClass
							  : classByCppType) {
			wrenReleaseHandle(vm, wrenClass.second);
		});

		WrenHandle* callStubByArity[ETCountOf(CallSignatures)] = { nullptr };
		ET_AT_SCOPE_EXIT(for (WrenHandle* stub
							  : callStubByArity) {
			if (stub) {
				wrenReleaseHandle(vm, stub);
			}
		});
		Transform(Begin(CallSignatures), End(CallSignatures), callStubByArity, [vm](const char* const signature) {
			return wrenMakeCallHandle(vm, signature);
		});

		Swap(_log, log);
		_assets = ETAddressOf(assets);
		Swap(_vm, vm);
		Swap(_classByCppType, classByCppType);
		Swap(_callStubByArity, callStubByArity);

		return Error::None;
	}

	// ---------------------------------------------------

	void Context::FreeResources() {
		for (WrenHandle*& stub : _callStubByArity) {
			wrenReleaseHandle(_vm, eastl::exchange(stub, nullptr));
		}

		_classByCppType.ClearAndDispose([this](const Pair<CppType, WrenHandle*>& entry) {
			wrenReleaseHandle(_vm, entry.second);
		});

		wrenFreeVM(eastl::exchange(_vm, nullptr));
	}

	// ---------------------------------------------------

	void Swap(Context& lhs, Context& rhs) {
		using ::Eldritch2::Swap;

		Swap(lhs._log, rhs._log);
		Swap(lhs._assets, rhs._assets);
		Swap(lhs._vm, rhs._vm);
		Swap(lhs._classByCppType, rhs._classByCppType);
		Swap(lhs._callStubByArity, rhs._callStubByArity);

		if (lhs._vm) {
			wrenSetUserData(lhs._vm, ETAddressOf(lhs));
		}

		if (rhs._vm) {
			wrenSetUserData(rhs._vm, ETAddressOf(rhs));
		}
	}

}}} // namespace Eldritch2::Scripting::Wren
