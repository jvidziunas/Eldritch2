/*==================================================================*\
  ApiBuilder.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Wren/WrenInternal.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Scripting { namespace Wren {

	namespace {

		ETForceInlineHint ETPureFunctionHint Method AsMethod(const ForeignMethod& foreignMethod) ETNoexceptHint {
			Method method{ /*type =*/METHOD_FOREIGN };
			method.as.foreign = WrenForeignMethodFn(foreignMethod.body);

			return method;
		}

		// ---------------------------------------------------

		ETForceInlineHint ETPureFunctionHint Method AsMethod(void(ETWrenCall* finalizer)(void*)) ETNoexceptHint {
			Method method{ /*type =*/METHOD_FOREIGN };
			method.as.foreign = WrenForeignMethodFn(finalizer);

			return method;
		}

		// ---------------------------------------------------

		ETForceInlineHint ObjModule* GetModule(WrenVM* vm, StringSpan moduleName) ETNoexceptHint {
			const auto CreateModule([vm](Value name) ETNoexceptHint -> Value {
				const Value module(OBJ_VAL(wrenNewModule(vm, AS_STRING(name))));
				wrenPushRoot(vm, AS_OBJ(module));
				wrenMapSet(vm, vm->modules, name, module);
				wrenPopRoot(vm);

				return module;
			});

			const Value name(wrenNewStringLength(vm, moduleName.GetData(), moduleName.GetLength()));

			wrenPushRoot(vm, AS_OBJ(name));
			Value module(wrenMapGet(vm->modules, name));
			if (ET_UNLIKELY(module == UNDEFINED_VAL)) {
				module = CreateModule(name);
			}
			wrenPopRoot(vm);

			return AS_MODULE(module);
		}

	} // anonymous namespace

	ForeignMethod::ForeignMethod(StringSpan signature, Body body) ETNoexceptHint : body(body) {
		ETAssert(body != nullptr, "Wren foreign method must have a body!");
		signature.Copy(this->signature, ETCountOf(this->signature));
	}

	// ---------------------------------------------------

	ApiBuilder::ApiBuilder(ApiBuilder&& api) ETNoexceptHint : _vm(Exchange(api._vm, nullptr)), _classByCppType(Move(api._classByCppType)) {}

	// ---------------------------------------------------

	ApiBuilder::ApiBuilder(WrenVM* vm) ETNoexceptHint : _vm(vm), _classByCppType(MallocAllocator("Wren API Wren Class/C++ Type Map Allocator")) {
		ETAssert(vm != nullptr, "Wren VM must be initialized before API construction!");
	}

	// ---------------------------------------------------

	ApiBuilder::~ApiBuilder() {
		_classByCppType.ClearAndDispose([vm = _vm](ClassMap::ConstReference entry) ETNoexceptHint {
			wrenReleaseHandle(vm, entry.second);
		});

		if(_vm != nullptr) {
			wrenFreeVM(_vm);
		}
	}

	// ---------------------------------------------------

	void ApiBuilder::DefineClass(CppType type, StringSpan name, StringSpan moduleName, InitializerList<ForeignMethod> staticMethods, InitializerList<ForeignMethod> methods, void(ETWrenCall* finalizer)(void*) ETNoexceptHint) {
		const auto CreateNewClass([vm = _vm](StringSpan className, StringSpan moduleName) ETNoexceptHint -> Value {
			const Value name(wrenNewStringLength(vm, className.GetData(), className.GetLength()));
			wrenPushRoot(vm, AS_OBJ(name));
			
			const Value wrenClass(OBJ_VAL(wrenNewClass(vm, vm->objectClass, /*numFields =*/-1, AS_STRING(name))));
			const int   result(wrenDefineVariable(vm, GetModule(vm, moduleName), className.GetData(), className.GetLength(), wrenClass));
			ETAssert(result != -1, "Wren module {} already contains symbol {}!", moduleName, className);
			ETAssert(result != -2, "Exhausted symbol table in Wren module {}!", moduleName);

			wrenPopRoot(vm); // name

			return wrenClass;
		});

		const auto AsMethodSymbol([vm = _vm](StringSpan symbol) ETNoexceptHint {
			return wrenSymbolTableEnsure(vm, ETAddressOf(vm->methodNames), symbol.GetData(), symbol.GetLength());
		});

		// ---

		if (ET_UNLIKELY(_classByCppType.Contains(type))) {
			return; // Type has been previously registered, ignore.
		}

		const Value wrenClass(CreateNewClass(name, moduleName));
		_classByCppType.Emplace(type, wrenMakeHandle(_vm, OBJ_VAL(wrenClass)));

		wrenBindMethod(_vm, AS_CLASS(wrenClass), AsMethodSymbol("<finalizer>"), AsMethod(finalizer));
		for (const ForeignMethod& method : methods) {
			wrenBindMethod(_vm, AS_CLASS(wrenClass), AsMethodSymbol(method.signature), AsMethod(method));
		}

		ObjClass* const metaClass(AS_OBJ(wrenClass)->classObj);
		for (const ForeignMethod& method : staticMethods) {
			// Wren static methods are registered with the metaclass, not the class itself.
			wrenBindMethod(_vm, metaClass, AsMethodSymbol(method.signature), AsMethod(method));
		}
	}

	// ---------------------------------------------------

	double ApiBuilder::DefineVariable(StringSpan name, StringSpan wrenModule, double value) {
		const int result(wrenDefineVariable(_vm, GetModule(_vm, wrenModule), name.GetData(), name.GetLength(), NUM_VAL(value)));
		ETAssert(result != -1, "Wren module {} already contains symbol {}!", wrenModule, name);
		ETAssert(result != -2, "Exhausted symbol table in Wren module {}!", wrenModule);

		return value;
	}

	// ---------------------------------------------------

	void* ApiBuilder::DefineVariable(CppType type, size_t byteSize, StringSpan name, StringSpan wrenModule) {
		ETAssert(_classByCppType.Contains(type), "Class has not been registered with the API builder!");

		const Value variable(OBJ_VAL(wrenNewForeign(_vm, AS_CLASS(_classByCppType[type]->value), byteSize)));
		const int   result(wrenDefineVariable(_vm, GetModule(_vm, wrenModule), name.GetData(), name.GetLength(), variable));
		ETAssert(result != -1, "Wren module {} already contains symbol {}!", wrenModule, name);
		ETAssert(result != -2, "Exhausted symbol table in Wren module {}!", wrenModule);

		return AS_FOREIGN(variable)->data;
	}

}}} // namespace Eldritch2::Scripting::Wren
