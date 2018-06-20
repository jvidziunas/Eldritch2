/*==================================================================*\
  ApiBuilder.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Wren/Marshal.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Scripting { namespace Wren {
	class Context;
}}} // namespace Eldritch2::Scripting::Wren

void wrenSetSlotBytes(WrenVM* vm, int slot, const char* text, size_t length);
void wrenAbortFiber(WrenVM* vm, int slot);

namespace Eldritch2 {
namespace Scripting {
namespace Wren {

	class ForeignMethod {
		// - TYPE PUBLISHING ---------------------------------

	public:
		using Body = void (*)(WrenVM*);

		// ---

		enum : size_t { MaxSignatureLength = 64u };

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref ForeignMethod instance.
		ForeignMethod(Utf8Literal signature, Body body);
		//!	Constructs this @ref ForeignMethod instance.
		ForeignMethod(const ForeignMethod&) = default;
		//!	Constructs this @ref ForeignMethod instance.
		ForeignMethod(decltype(nullptr));

		~ForeignMethod() = default;

		// - DATA MEMBERS ------------------------------------

	public:
		char signature[MaxSignatureLength];
		Body body;
	};

	// ---

	class StaticMethod : public ForeignMethod {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref StaticMethod instance.
		StaticMethod(Utf8Literal signature, Body body);
		//!	Constructs this @ref StaticMethod instance.
		StaticMethod(const StaticMethod&) = default;
		//!	Constructs this @ref StaticMethod instance.
		StaticMethod(decltype(nullptr));

		~StaticMethod() = default;
	};

	// ---

	class ConstructorMethod : public ForeignMethod {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref ConstructorMethod instance.
		ConstructorMethod(Utf8Literal signature, Body body);
		//!	Constructs this @ref ConstructorMethod instance.
		ConstructorMethod(const ConstructorMethod&) = default;
		//!	Constructs this @ref ConstructorMethod instance.
		ConstructorMethod(decltype(nullptr));

		~ConstructorMethod() = default;
	};

	// ---

	class ApiBuilder {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref ApiBuilder instance.
		ApiBuilder(WrenVM* vm, Context& wren);
		//!	Disable copy construction.
		ApiBuilder(const ApiBuilder&) = delete;

		~ApiBuilder() = default;

		// ---------------------------------------------------

	public:
		template <class Class>
		void CreateClass(
			const char*                                               module,
			const char*                                               name,
			std::initializer_list<ConstructorMethod>                  constructors,
			std::initializer_list<StaticMethod>                       staticMethods,
			std::initializer_list<Pair<ForeignMethod, ForeignMethod>> properties,
			std::initializer_list<ForeignMethod>                      methods);

		template <class Class, typename... Arguments>
		Class* CreateVariable(const char* module, const char* name, Arguments&&... arguments);
		void   CreateVariable(const char* module, const char* name, double value);

		// - DATA MEMBERS ------------------------------------

	private:
		WrenVM*  _vm;
		Context* _context;
	};

	// ---------------------------------------------------

	ETPureFunctionHint Pair<ForeignMethod, ForeignMethod> DefineProperty(Utf8Literal name, ForeignMethod::Body getterBody, ForeignMethod::Body setterBody);

	ETPureFunctionHint Pair<ForeignMethod, ForeignMethod> DefineGetter(Utf8Literal name, ForeignMethod::Body body);

	ETPureFunctionHint Pair<ForeignMethod, ForeignMethod> DefineSetter(Utf8Literal name, ForeignMethod::Body body);

}
}
} // namespace Eldritch2::Scripting::Wren

#define ET_BUILTIN_WREN_MODULE_NAME(name) "builtin/" #name

#define ET_ABORT_WREN(message)                                   \
	{                                                            \
		wrenSetSlotBytes(vm, 0, message, _countof(message) - 1); \
		return wrenAbortFiber(vm, 0);                            \
	}
#define ET_ABORT_WREN_FMT(message, ...)                               \
	{                                                                 \
		fmt::memory_buffer text;                                      \
		fmt::format_to(text, fmt::string_view(message), __VA_ARGS__); \
		wrenSetSlotBytes(vm, 0, text.data(), text.size());            \
		return wrenAbortFiber(vm, 0);                                 \
	}
#define ET_ABORT_WREN_IF(condition, message)                     \
	if ((condition)) {                                           \
		wrenSetSlotBytes(vm, 0, message, StringLength(message)); \
		return wrenAbortFiber(vm, 0);                            \
	}
#define ET_ABORT_WREN_IF_FMT(condition, message, ...) \
	if ((condition)) {                                \
		ET_ABORT_WREN_FMT(message, __VA_ARGS__);      \
	}
#define ET_ABORT_WREN_UNLESS(condition, message) \
	if (!(condition)) {                          \
		ET_ABORT_WREN(message);                  \
	}
#define ET_ABORT_WREN_UNLESS_FMT(condition, message, ...) \
	if (!(condition)) {                                   \
		ET_ABORT_WREN_FMT(message, __VA_ARGS__);          \
	}

#define ET_DECLARE_WREN_CLASS(className) void className##WrenDefinition(Scripting::Wren::ApiBuilder& api);
#define ET_IMPLEMENT_WREN_CLASS(className) void className##WrenDefinition(Scripting::Wren::ApiBuilder& api)
#define ET_REGISTER_WREN_CLASS(className, builder) className##WrenDefinition(builder)

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Scripting/Wren/ApiBuilder.inl>
//------------------------------------------------------------------//
