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
#include <Scripting/Wren/Context.hpp>
#include <Assets/AssetDatabase.hpp>
//------------------------------------------------------------------//
#include <wren.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Scripting { namespace Wren {

	using namespace ::Eldritch2::Scripting::Wren::AssetViews;
	using namespace ::Eldritch2::Logging;
	using namespace ::Eldritch2::Assets;

	namespace {

		ETInlineHint ObjModule* CreateModule(WrenVM* vm, ObjString* name) {
			ObjModule* const module(wrenNewModule(vm, name));

			wrenPushRoot(vm, reinterpret_cast<Obj*>(module));
			wrenMapSet(vm, vm->modules, OBJ_VAL(name), OBJ_VAL(module));
			wrenPopRoot(vm);

			return module;
		}

		// ---------------------------------------------------

		ETInlineHint ObjModule* GetModule(WrenVM* vm, StringView moduleName) {
			ObjString* const name(AS_STRING(wrenNewStringLength(vm, moduleName.GetData(), moduleName.GetLength())));

			wrenPushRoot(vm, reinterpret_cast<Obj*>(name));
			const Value candidate(wrenMapGet(vm->modules, OBJ_VAL(name)));
			ObjModule*  module(candidate != UNDEFINED_VAL ? AS_MODULE(candidate) : CreateModule(vm, name));
			wrenPopRoot(vm);

			return module;
		}

	} // anonymous namespace

	Context::Context(const AssetDatabase& assets, Log& log) :
		_log(log),
		_assets(&assets),
		_classesByType(MallocAllocator("Wren Class By Type Allocator")),
		_callStubByArity { nullptr } {
	}

	// ---------------------------------------------------

	Context::~Context() {
#if ET_DEBUG_BUILD
		for (WrenHandle* stub : _callStubByArity) {
			ET_ASSERT(stub == nullptr, "Leaking Wren call handle!");
		}
#endif
	}

	// ---------------------------------------------------

	WrenHandle* Context::CreateForeignClass(WrenVM* vm, StringView module, StringView name, CppType type) {
		ObjString* const wrenName(AS_STRING(wrenNewStringLength(vm, name.GetData(), name.GetLength())));

		wrenPushRoot(vm, reinterpret_cast<Obj*>(wrenName));
		const Value wrenClass(OBJ_VAL(wrenNewClass(vm, vm->objectClass, -1, wrenName)));
		const int   result(wrenDefineVariable(vm, GetModule(vm, module), name.GetData(), name.GetLength(), wrenClass));

		ET_ASSERT(result != -1, "Duplicate Wren class registration!");
		ET_ASSERT(result != -2, "Error registering Wren class with module!");

		WrenHandle* klass(wrenMakeHandle(vm, OBJ_VAL(wrenClass)));
		//	Pop Wren class name.
		wrenPopRoot(vm);

		_classesByType.Emplace(type, klass);

		return klass;
	}

	// ---------------------------------------------------

	void* Context::CreateVariable(WrenVM* vm, StringView module, StringView name, WrenHandle* wrenClass, size_t size) ETNoexceptHint {
		ObjForeign* const variable(wrenNewForeign(vm, AS_CLASS(wrenClass->value), size));
		const int         result(wrenDefineVariable(vm, GetModule(vm, module), name.GetData(), name.GetLength(), OBJ_VAL(variable)));

		ET_ASSERT(result != -1, "Duplicate Wren variable registration!");
		ET_ASSERT(result != -2, "Error registering Wren variable with module!");

		return variable->data;
	}

	// ---------------------------------------------------

	double Context::CreateVariable(WrenVM* vm, StringView module, StringView name, double value) ETNoexceptHint {
		const int result(wrenDefineVariable(vm, GetModule(vm, module), name.GetData(), name.GetLength(), NUM_VAL(value)));

		ET_ASSERT(result != -1, "Duplicate Wren variable registration!");
		ET_ASSERT(result != -2, "Error registering Wren variable with module!");

		return value;
	}

	// ---------------------------------------------------

	const ScriptAsset* Context::FindScriptModule(StringView path) const ETNoexceptHint {
		return static_cast<const ScriptAsset*>(_assets->Find(path));
	}

	// ---------------------------------------------------

	void Context::BindResources(WrenVM* vm) {
		static ETConstexpr const char* CallSignatures[ETCountOf(_callStubByArity)] = {
			"call()",
			"call(_)",
			"call(_,_)",
			"call(_,_,_)",
			"call(_,_,_,_)",
			"call(_,_,_,_,_)",
			"call(_,_,_,_,_,_)",
			"call(_,_,_,_,_,_,_)",
			"call(_,_,_,_,_,_,_,_)",
			"call(_,_,_,_,_,_,_,_,_)",
			"call(_,_,_,_,_,_,_,_,_,_)",
			"call(_,_,_,_,_,_,_,_,_,_,_)",
			"call(_,_,_,_,_,_,_,_,_,_,_,_)",
			"call(_,_,_,_,_,_,_,_,_,_,_,_,_)",
			"call(_,_,_,_,_,_,_,_,_,_,_,_,_,_)",
			"call(_,_,_,_,_,_,_,_,_,_,_,_,_,_,_)"
		};

		Transform(CallSignatures, _callStubByArity, [vm](const char* const signature) {
			return wrenMakeCallHandle(vm, signature);
		});
	}

	// ---------------------------------------------------

	void Context::FreeResources(WrenVM* vm) {
		_classesByType.ClearAndDispose([vm](const Pair<CppType, WrenHandle*>& entry) {
			wrenReleaseHandle(vm, entry.second);
		});

		for (WrenHandle*& stub : _callStubByArity) {
			wrenReleaseHandle(vm, eastl::exchange(stub, nullptr));
		}
	}

}}} // namespace Eldritch2::Scripting::Wren
