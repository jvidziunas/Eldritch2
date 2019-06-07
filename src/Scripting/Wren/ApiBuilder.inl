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

//------------------------------------------------------------------//

namespace Eldritch2 { namespace Scripting { namespace Wren {

	ETConstexpr ETForceInlineHint ForeignMethod::ForeignMethod() ETNoexceptHint : body(nullptr), signature { '\0' } {}

	// ---------------------------------------------------

	template <class Class>
	ETInlineHint ETForceInlineHint void ApiBuilder::DefineClass(StringSpan name, StringSpan moduleName, InitializerList<ForeignMethod> staticMethods, InitializerList<ForeignMethod> methods) {
		return DefineClass(GetType<Class>(), name, moduleName, staticMethods, methods, /*finalizer =*/[](void* object) ETNoexceptHint { static_cast<Class*>(object)->~Class(); });
	}

	// ---------------------------------------------------

	template <class Class, typename... Arguments>
	ETInlineHint ETForceInlineHint Class* ApiBuilder::DefineVariable(StringSpan name, StringSpan moduleName, Arguments&&... arguments) {
		return new (DefineVariable(GetType<Class>(), sizeof(Class), moduleName, name)) Class(Forward<Arguments>(arguments)...);
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint ApiBuilder::ClassMap& ApiBuilder::GetClasses() ETNoexceptHint {
		return _classByCppType;
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint WrenVM*& ApiBuilder::GetVm() ETNoexceptHint {
		return _vm;
	}

}}} // namespace Eldritch2::Scripting::Wren
