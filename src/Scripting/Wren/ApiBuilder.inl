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

	ETPureFunctionHint size_t	HashForeignMethod(const char* const module, const char* const className, bool isStatic, const char* const signature, size_t seed = 0u);
	ETPureFunctionHint size_t	HashForeignClass(const char* const module, const char* const className, size_t seed = 0u);
	void						BindFinalizer(WrenVM* vm, WrenHandle* target, void(*finalizer)(void*));
	void						Bind(WrenVM* vm, WrenHandle* target, const ForeignMethod& method, bool isStatic);

}	// namespace Detail

	ETInlineHint ForeignMethod::ForeignMethod(decltype(nullptr)) : body(nullptr) {
		signature[0] = '\0';
	}

// ---------------------------------------------------

	ETInlineHint StaticMethod::StaticMethod(Utf8Literal signature, Body body) : ForeignMethod(signature, body) {}

// ---------------------------------------------------

	ETInlineHint StaticMethod::StaticMethod(decltype(nullptr)) : ForeignMethod(nullptr) {}

// ---------------------------------------------------

	ETInlineHint ConstructorMethod::ConstructorMethod(Utf8Literal signature, Body body) : ForeignMethod(signature, body) {}

// ---------------------------------------------------

	ETInlineHint ConstructorMethod::ConstructorMethod(decltype(nullptr)) : ForeignMethod(nullptr) {}

// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint Pair<ForeignMethod, ForeignMethod> DefineGetter(Utf8Literal name, ForeignMethod::Body body) {
		return { ForeignMethod(name, body), ForeignMethod(nullptr) };
	}

// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint Pair<ForeignMethod, ForeignMethod> DefineSetter(Utf8Literal name, ForeignMethod::Body body) {
		Utf8Char signature[ForeignMethod::MaxSignatureLength];

		AppendString(CopyString(signature, name), "=(_)");

		return { ForeignMethod(nullptr), ForeignMethod(signature, body) };
	}

// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint Pair<ForeignMethod, ForeignMethod> DefineProperty(Utf8Literal name, ForeignMethod::Body getterBody, ForeignMethod::Body setterBody) {
		Utf8Char signature[ForeignMethod::MaxSignatureLength];

		AppendString(CopyString(signature, name), "=(_)");

		return { ForeignMethod(name, getterBody), ForeignMethod(signature, setterBody) };
	}

// ---------------------------------------------------

	template <class Class>
	ETInlineHint void ApiBuilder::CreateClass(
		const char* module,
		const char* name,
		std::initializer_list<ConstructorMethod> constructors,
		std::initializer_list<StaticMethod> staticMethods,
		std::initializer_list<Pair<ForeignMethod, ForeignMethod>> properties,
		std::initializer_list<ForeignMethod> methods
	) {
	//	Only define types once.
		if (_context->FindForeignClass<Class>() != nullptr) { return; }

		WrenHandle* const	newClass(_context->CreateForeignClass(_vm, module, name, typeid(Class)));

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
	ETInlineHint Class* ApiBuilder::CreateVariable(const char* module, const char* name, Arguments&&... arguments) {
		WrenHandle* const	classHandle(_context->FindForeignClass<Class>());

		ET_ASSERT(classHandle != nullptr, "Class has not been registered with the API builder!");

		return new(_context->CreateVariable(_vm, module, name, classHandle, sizeof(Class))) Class(eastl::forward<Arguments>(arguments)...);
	}

// ---------------------------------------------------

	ETInlineHint void ApiBuilder::CreateVariable(const char* module, const char* name, double value) {
		_context->CreateVariable(_vm, module, name, value);
	}

}	// namespace Wren
}	// namespace Scripting
}	// namespace Eldritch2