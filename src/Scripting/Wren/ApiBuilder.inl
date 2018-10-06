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

	ETCpp14Constexpr ETInlineHint ETForceInlineHint ForeignMethod::ForeignMethod() ETNoexceptHint : body(nullptr), signature { '\0' } {}

	// ---------------------------------------------------

	template <class Class>
	ETInlineHint ETForceInlineHint void ApiBuilder::DefineClass(StringView wrenModule, StringView wrenName, std::initializer_list<ForeignMethod> staticMethods, std::initializer_list<ForeignMethod> methods) {
		return DefineClass(typeid(Class), wrenModule, wrenName, constructors, staticMethods, properties, methods, [](void* object) ETNoexceptHint { InvokeDestructor()(static_cast<Class*>(object)); });
	}

	// ---------------------------------------------------

	template <class Class, typename... Arguments>
	ETInlineHint ETForceInlineHint Class* ApiBuilder::DefineVariable(StringView wrenModule, StringView wrenName, Arguments&&... arguments) {
		return new (DefineVariable(typeid(Class), sizeof(Class), wrenModule, wrenName)) Class(eastl::forward<Arguments>(arguments)...);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint ApiBuilder::ClassMap ApiBuilder::ReleaseClasses() {
		return eastl::exchange(_classByCppType, ClassMap(_classByCppType.GetAllocator()));
	}

}}} // namespace Eldritch2::Scripting::Wren
