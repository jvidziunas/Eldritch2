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

namespace Eldritch2 { namespace Scripting { namespace Wren {

	class ForeignMethod {
		// - TYPE PUBLISHING ---------------------------------

	public:
		enum : size_t { MaxSignatureLength = 64u };

		using Body = void (*)(WrenVM*);

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref ForeignMethod instance.
		ForeignMethod(StringView signature, Body body) ETNoexceptHint;
		//!	Constructs this @ref ForeignMethod instance.
		ForeignMethod(const ForeignMethod&) ETNoexceptHint = default;
		//!	Constructs this @ref ForeignMethod instance.
		ETCpp14Constexpr ForeignMethod(decltype(nullptr)) ETNoexceptHint;

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
		StaticMethod(StringView signature, Body body) ETNoexceptHint;
		//!	Constructs this @ref StaticMethod instance.
		StaticMethod(const StaticMethod&) ETNoexceptHint = default;
		//!	Constructs this @ref StaticMethod instance.
		ETCpp14Constexpr StaticMethod(decltype(nullptr)) ETNoexceptHint;

		~StaticMethod() = default;
	};

	// ---

	class ConstructorMethod : public ForeignMethod {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref ConstructorMethod instance.
		ConstructorMethod(StringView signature, Body body) ETNoexceptHint;
		//!	Constructs this @ref ConstructorMethod instance.
		ConstructorMethod(const ConstructorMethod&) ETNoexceptHint = default;
		//!	Constructs this @ref ConstructorMethod instance.
		ETCpp14Constexpr ConstructorMethod(decltype(nullptr)) ETNoexceptHint;

		~ConstructorMethod() = default;
	};

	// ---

	class ApiBuilder {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref ApiBuilder instance.
		ApiBuilder(WrenVM* vm, Context& wren) ETNoexceptHint;
		//!	Disable copy construction.
		ApiBuilder(const ApiBuilder&) = delete;

		~ApiBuilder() = default;

		// ---------------------------------------------------

	public:
		template <class Class>
		void CreateClass(
			StringView                                                module,
			StringView                                                name,
			std::initializer_list<ConstructorMethod>                  constructors,
			std::initializer_list<StaticMethod>                       staticMethods,
			std::initializer_list<Pair<ForeignMethod, ForeignMethod>> properties,
			std::initializer_list<ForeignMethod>                      methods);

		template <class Class, typename... Arguments>
		Class* CreateVariable(StringView module, StringView name, Arguments&&... arguments);
		void   CreateVariable(StringView module, StringView name, double value);

		// - DATA MEMBERS ------------------------------------

	private:
		WrenVM*  _vm;
		Context* _context;
	};

	// ---------------------------------------------------

	ETPureFunctionHint Pair<ForeignMethod, ForeignMethod> DefineProperty(StringView name, ForeignMethod::Body getterBody, ForeignMethod::Body setterBody) ETNoexceptHint;

	ETPureFunctionHint Pair<ForeignMethod, ForeignMethod> DefineGetter(StringView name, ForeignMethod::Body body) ETNoexceptHint;

	ETPureFunctionHint Pair<ForeignMethod, ForeignMethod> DefineSetter(StringView name, ForeignMethod::Body body) ETNoexceptHint;

}}} // namespace Eldritch2::Scripting::Wren

#define ET_BUILTIN_WREN_MODULE_NAME(name) "builtin/" #name

#define ET_ABORT_WREN(message)                                     \
	{                                                              \
		::wrenSetSlotBytes(vm, 0, message, ETCountOf(message) - 1); \
		return ::wrenAbortFiber(vm, 0);                            \
	}
#define ET_ABORT_WREN_FMT(message, ...)                                 \
	{                                                                   \
		::fmt::memory_buffer text;                                      \
		::fmt::format_to(text, fmt::string_view(message), __VA_ARGS__); \
		wrenSetSlotBytes(vm, 0, text.data(), text.size());              \
		return wrenAbortFiber(vm, 0);                                   \
	}
#define ET_ABORT_WREN_IF(condition, message)                       \
	if ((condition)) {                                             \
		::wrenSetSlotBytes(vm, 0, message, StringLength(message)); \
		return ::wrenAbortFiber(vm, 0);                            \
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

#define ET_DECLARE_WREN_CLASS(className) void className##WrenDefinition(::Eldritch2::Scripting::Wren::ApiBuilder& api)
#define ET_IMPLEMENT_WREN_CLASS(className) void className##WrenDefinition(::Eldritch2::Scripting::Wren::ApiBuilder& api)
#define ET_REGISTER_WREN_CLASS(className, builder) className##WrenDefinition(builder)

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Scripting/Wren/ApiBuilder.inl>
//------------------------------------------------------------------//
