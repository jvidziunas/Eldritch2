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
	namespace Detail {

		namespace {

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

		} // anonymous namespace

		ETCpp14Constexpr ETPureFunctionHint size_t HashForeignMethod(const char* const module, const char* const className, bool isStatic, const char* const signature, size_t seed) ETNoexceptHint {
			return HashMemory(signature, StringLength(signature), HashForeignClass(module, className, seed + (isStatic ? 0u : 31u)));
		}

		// ---------------------------------------------------

		ETCpp14Constexpr ETPureFunctionHint size_t HashForeignClass(const char* const module, const char* const className, size_t seed) ETNoexceptHint {
			return HashMemory(module, StringLength(module), HashMemory(className, StringLength(className), seed));
		}

		// ---------------------------------------------------

		void BindFinalizer(WrenVM* vm, WrenHandle* target, void (*finalizer)(void*)) ETNoexceptHint {
			ETConstexpr StringView name("<finalizer>");
			wrenBindMethod(vm, AS_CLASS(target->value), wrenSymbolTableEnsure(vm, &vm->methodNames, name.GetData(), name.GetLength()), AsWrenMethod(finalizer));
		}

		// ---------------------------------------------------

		void Bind(WrenVM* vm, WrenHandle* target, const ForeignMethod& method, bool isStatic) ETNoexceptHint {
			wrenBindMethod(
				vm,
				isStatic ? AS_CLASS(target->value)->obj.classObj : AS_CLASS(target->value),
				wrenSymbolTableEnsure(vm, &vm->methodNames, method.signature, StringLength(method.signature)),
				AsWrenMethod(method));
		}

	} // namespace Detail

	ForeignMethod::ForeignMethod(StringView signature, Body body) ETNoexceptHint : body(body) {
		ET_ASSERT(body != nullptr, "Wren foreign method must have a body!");
		signature.Copy(this->signature, ETCountOf(this->signature));
	}

	// ---------------------------------------------------

	ApiBuilder::ApiBuilder(WrenVM* vm, Context& wren) ETNoexceptHint : _vm(vm),
																	   _context(ETAddressOf(wren)) {
		ET_ASSERT(vm != nullptr, "Wren VM must be initialized before API construction!");
	}

}}} // namespace Eldritch2::Scripting::Wren
