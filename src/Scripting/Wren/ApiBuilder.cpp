/*==================================================================*\
  ApiBuilder.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Wren/WrenInternal.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Scripting { namespace Wren {

	namespace {

		ETCpp14Constexpr ETPureFunctionHint size_t HashForeignMethod(const char* const module, const char* const className, bool isStatic, const char* const signature, size_t seed) ETNoexceptHint {
			return HashMemory(signature, StringLength(signature), HashForeignClass(module, className, seed + (isStatic ? 0u : 31u)));
		}

		// ---------------------------------------------------

		ETCpp14Constexpr ETPureFunctionHint size_t HashForeignClass(const char* const module, const char* const className, size_t seed) ETNoexceptHint {
			return HashMemory(module, StringLength(module), HashMemory(className, StringLength(className), seed));
		}

		// ---------------------------------------------------

		ETInlineHint ETForceInlineHint int GetMethodSymbol(WrenVM* vm, StringView symbol) ETNoexceptHint {
			return wrenSymbolTableEnsure(vm, ETAddressOf(vm->methodNames), symbol.GetData(), symbol.GetLength());
		}

		// ---------------------------------------------------

		ETInlineHint ETForceInlineHint ETPureFunctionHint Method AsWrenMethod(const ForeignMethod& foreignMethod) ETNoexceptHint {
			Method method { /*type =*/METHOD_FOREIGN };
			method.fn.foreign = WrenForeignMethodFn(foreignMethod.body);

			return method;
		}

		// ---------------------------------------------------

		ETInlineHint ETForceInlineHint ETPureFunctionHint Method AsWrenMethod(void (*finalizer)(void*)) ETNoexceptHint {
			Method method { /*type =*/METHOD_FOREIGN };
			method.fn.foreign = WrenForeignMethodFn(finalizer);

			return method;
		}

		// ---------------------------------------------------

		ETInlineHint ETForceInlineHint Value CreateWrenModule(WrenVM* vm, Value name) ETNoexceptHint {
			const Value module(OBJ_VAL(wrenNewModule(vm, AS_STRING(name))));
			wrenPushRoot(vm, AS_OBJ(module));
			ET_AT_SCOPE_EXIT(wrenPopRoot(vm));

			wrenMapSet(vm, vm->modules, name, module);
			return module;
		}

		// ---------------------------------------------------

		ETInlineHint ETForceInlineHint ObjModule* GetWrenModule(WrenVM* vm, StringView wrenName) ETNoexceptHint {
			const Value name(wrenNewStringLength(vm, wrenName.GetData(), wrenName.GetLength()));
			wrenPushRoot(vm, AS_OBJ(name));
			ET_AT_SCOPE_EXIT(wrenPopRoot(vm));

			Value module(wrenMapGet(vm->modules, name));
			if (ET_UNLIKELY(module == UNDEFINED_VAL)) {
				module = CreateWrenModule(vm, name);
			}

			return AS_MODULE(module);
		}

		// ---------------------------------------------------

		ETInlineHint ETForceInlineHint ObjClass* CreateForeignClass(WrenVM* vm, StringView wrenModule, StringView wrenName) ETNoexceptHint {
			const Value name(wrenNewStringLength(vm, wrenName.GetData(), wrenName.GetLength()));
			wrenPushRoot(vm, AS_OBJ(name));
			ET_AT_SCOPE_EXIT(wrenPopRoot(vm));

			const Value klass(OBJ_VAL(wrenNewClass(vm, vm->objectClass, /*numFields =*/-1, AS_STRING(name))));
			const int   result(wrenDefineVariable(vm, GetWrenModule(vm, wrenModule), wrenName.GetData(), wrenName.GetLength(), klass));
			ET_ASSERT(result != -1, "Wren module {} already contains symbol {}!", wrenModule, wrenName);
			ET_ASSERT(result != -2, "Exhausted symbol table in Wren module {}!", wrenModule);

			return AS_CLASS(klass);
		}

	} // anonymous namespace

	ForeignMethod::ForeignMethod(StringView signature, Body body) ETNoexceptHint : body(body) {
		ET_ASSERT(body != nullptr, "Wren foreign method must have a body!");
		signature.Copy(this->signature, ETCountOf(this->signature));
	}

	// ---------------------------------------------------

	ApiBuilder::ApiBuilder(WrenVM* vm) ETNoexceptHint : _vm(vm), _classByCppType(MallocAllocator("Wren API Wren Class/C++ Type Map Allocator")) {
		ET_ASSERT(vm != nullptr, "Wren VM must be initialized before API construction!");
	}

	// ---------------------------------------------------

	ApiBuilder::~ApiBuilder() {
		_classByCppType.ClearAndDispose([this](const Pair<CppType, WrenHandle*>& entry) {
			wrenReleaseHandle(_vm, entry.second);
		});
	}

	// ---------------------------------------------------

	void ApiBuilder::DefineClass(CppType type, StringView wrenModule, StringView wrenName, std::initializer_list<ForeignMethod> staticMethods, std::initializer_list<ForeignMethod> methods, void (*finalizer)(void*) ETNoexceptHint) {
		if (ET_UNLIKELY(_classByCppType.Contains(type))) {
			return;
		}

		ObjClass* const klass(CreateForeignClass(_vm, wrenModule, wrenName));
		_classByCppType.Emplace(type, wrenMakeHandle(_vm, OBJ_VAL(klass)));
		wrenBindMethod(_vm, klass, GetMethodSymbol(_vm, "<finalizer>"), AsWrenMethod(finalizer));
		for (const ForeignMethod& method : methods) {
			wrenBindMethod(_vm, klass, GetMethodSymbol(_vm, method.signature), AsWrenMethod(method));
		}

		ObjClass* const metaClass(klass->obj.classObj);
		for (const ForeignMethod& method : staticMethods) {
			// Wren static methods are registered with the metaclass, not the class itself.
			wrenBindMethod(_vm, metaClass, GetMethodSymbol(_vm, method.signature), AsWrenMethod(method));
		}
	}

	// ---------------------------------------------------

	double ApiBuilder::DefineVariable(StringView wrenModule, StringView wrenName, double value) {
		const int result(wrenDefineVariable(_vm, GetWrenModule(_vm, wrenModule), wrenName.GetData(), wrenName.GetLength(), NUM_VAL(value)));
		ET_ASSERT(result != -1, "Wren module {} already contains symbol {}!", wrenModule, wrenName);
		ET_ASSERT(result != -2, "Exhausted symbol table in Wren module {}!", wrenModule);

		return value;
	}

	// ---------------------------------------------------

	void* ApiBuilder::DefineVariable(CppType type, size_t byteSize, StringView wrenModule, StringView wrenName) {
		ET_ASSERT(_classByCppType.Contains(type), "Class has not been registered with the API builder!");

		const Value variable(OBJ_VAL(wrenNewForeign(_vm, AS_CLASS(_classByCppType[type]->value), byteSize)));
		const int   result(wrenDefineVariable(_vm, GetWrenModule(_vm, wrenModule), wrenName.GetData(), wrenName.GetLength(), variable));
		ET_ASSERT(result != -1, "Wren module {} already contains symbol {}!", wrenModule, wrenName);
		ET_ASSERT(result != -2, "Exhausted symbol table in Wren module {}!", wrenModule);

		return AS_FOREIGN(variable)->data;
	}

}}} // namespace Eldritch2::Scripting::Wren
