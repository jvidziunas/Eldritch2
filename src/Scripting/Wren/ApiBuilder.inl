/*==================================================================*\
  ApiBuilder.inl
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Wren/Context.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scripting {
namespace Wren {
	namespace Detail {

		ETCpp14Constexpr ETPureFunctionHint size_t HashForeignMethod(const char* const module, const char* const className, bool isStatic, const char* const signature, size_t seed = 0u) ETNoexceptHint;
		ETCpp14Constexpr ETPureFunctionHint size_t HashForeignClass(const char* const module, const char* const className, size_t seed = 0u) ETNoexceptHint;
		void                                       BindFinalizer(WrenVM* vm, WrenHandle* target, void (*finalizer)(void*)) ETNoexceptHint;
		void                                       Bind(WrenVM* vm, WrenHandle* target, const ForeignMethod& method, bool isStatic) ETNoexceptHint;

	} // namespace Detail

	ETCpp14Constexpr ETInlineHint ForeignMethod::ForeignMethod(decltype(nullptr)) ETNoexceptHint : body(nullptr),
																								   signature { '\0' } {
	}

	// ---------------------------------------------------

	ETInlineHint StaticMethod::StaticMethod(StringView signature, Body body) ETNoexceptHint : ForeignMethod(signature, body) {}

	// ---------------------------------------------------

	ETCpp14Constexpr ETInlineHint StaticMethod::StaticMethod(decltype(nullptr)) ETNoexceptHint : ForeignMethod(nullptr) {}

	// ---------------------------------------------------

	ETInlineHint ConstructorMethod::ConstructorMethod(StringView signature, Body body) ETNoexceptHint : ForeignMethod(signature, body) {}

	// ---------------------------------------------------

	ETCpp14Constexpr ETInlineHint ConstructorMethod::ConstructorMethod(decltype(nullptr)) ETNoexceptHint : ForeignMethod(nullptr) {}

	// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint Pair<ForeignMethod, ForeignMethod> DefineGetter(StringView name, ForeignMethod::Body body) ETNoexceptHint {
		return { ForeignMethod(name, body), ForeignMethod(nullptr) };
	}

	// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint Pair<ForeignMethod, ForeignMethod> DefineSetter(StringView name, ForeignMethod::Body body) ETNoexceptHint {
		Utf8Char signature[ForeignMethod::MaxSignatureLength] = { '\0' };
		name.Copy(signature, ETCountOf(signature));

		return { ForeignMethod(nullptr), ForeignMethod(AppendString(signature, "=(_)"), body) };
	}

	// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint Pair<ForeignMethod, ForeignMethod> DefineProperty(StringView name, ForeignMethod::Body getterBody, ForeignMethod::Body setterBody) ETNoexceptHint {
		Utf8Char signature[ForeignMethod::MaxSignatureLength] = { '\0' };
		name.Copy(signature, ETCountOf(signature));

		return { ForeignMethod(name, getterBody), ForeignMethod(AppendString(signature, "=(_)"), setterBody) };
	}

	// ---------------------------------------------------

	template <class Class>
	ETInlineHint void ApiBuilder::CreateClass(
		StringView                                                module,
		StringView                                                name,
		std::initializer_list<ConstructorMethod>                  constructors,
		std::initializer_list<StaticMethod>                       staticMethods,
		std::initializer_list<Pair<ForeignMethod, ForeignMethod>> properties,
		std::initializer_list<ForeignMethod>                      methods) {
		//	Only define types once.
		if (_context->FindForeignClass<Class>() != nullptr) {
			return;
		}

		WrenHandle* const newClass(_context->CreateForeignClass(_vm, module, name, typeid(Class)));
		for (const ConstructorMethod& constructor : constructors) {
			Detail::Bind(_vm, newClass, constructor, true);
		}

		for (const StaticMethod& method : staticMethods) {
			Detail::Bind(_vm, newClass, method, true);
		}

		for (const Pair<ForeignMethod, ForeignMethod>& property : properties) {
			if (property.first.body) {
				Detail::Bind(_vm, newClass, property.first, false);
			}

			if (property.second.body) {
				Detail::Bind(_vm, newClass, property.second, false);
			}
		}

		for (const ForeignMethod& method : methods) {
			Detail::Bind(_vm, newClass, method, false);
		}

		Detail::BindFinalizer(_vm, newClass, [](void* object) {
			//	MSVC bug workaround.
			ETUnreferencedParameter(object);
			static_cast<Class*>(object)->~Class();
		});
	}

	// ---------------------------------------------------

	template <class Class, typename... Arguments>
	ETInlineHint Class* ApiBuilder::CreateVariable(StringView module, StringView name, Arguments&&... arguments) {
		WrenHandle* const classHandle(_context->FindForeignClass<Class>());
		ET_ASSERT(classHandle != nullptr, "Class has not been registered with the API builder!");

		return new (_context->CreateVariable(_vm, module, name, classHandle, sizeof(Class))) Class(eastl::forward<Arguments>(arguments)...);
	}

	// ---------------------------------------------------

	ETInlineHint void ApiBuilder::CreateVariable(StringView module, StringView name, double value) {
		_context->CreateVariable(_vm, module, name, value);
	}

}}} // namespace Eldritch2::Scripting::Wren
