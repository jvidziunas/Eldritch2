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

struct WrenHandle;

namespace Eldritch2 { namespace Scripting { namespace Wren {

	class ForeignMethod {
		// - TYPE PUBLISHING ---------------------------------

	public:
		using Body = void (*)(WrenVM*) ETNoexceptHint;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref ForeignMethod instance.
		ForeignMethod(StringView signature, Body body) ETNoexceptHint;
		//!	Constructs this @ref ForeignMethod instance.
		ForeignMethod(const ForeignMethod&) ETNoexceptHint = default;
		//!	Constructs this @ref ForeignMethod instance.
		ETCpp14Constexpr ForeignMethod() ETNoexceptHint;

		~ForeignMethod() = default;

		// - DATA MEMBERS ------------------------------------

	public:
		char signature[64];
		Body body;
	};

	// ---

	class Property {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref Property instance.
		Property(StringView name, ForeignMethod::Body getter, ForeignMethod::Body setter) ETNoexceptHint;
		//!	Constructs this @ref Property instance.
		Property(const Property&) ETNoexceptHint = default;

		// - DATA MEMBERS ------------------------------------

	public:
		ForeignMethod getter;
		ForeignMethod setter;
	};

	// ---

	class ApiBuilder {
		// - TYPE PUBLISHING ---------------------------------

	public:
		using ClassMap = CachingHashMap<CppType, WrenHandle*>;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref ApiBuilder instance.
		ApiBuilder(WrenVM* vm) ETNoexceptHint;
		//!	Disable copy construction.
		ApiBuilder(const ApiBuilder&) = delete;

		~ApiBuilder();

		// ---------------------------------------------------

	public:
		template <class Class>
		void DefineClass(StringView wrenModule, StringView wrenName, std::initializer_list<ForeignMethod> staticMethods, std::initializer_list<ForeignMethod> methods);

		template <class Class, typename... Arguments>
		Class* DefineVariable(StringView wrenModule, StringView wrenName, Arguments&&... arguments);
		double DefineVariable(StringView wrenModule, StringView wrenName, double value);

		// ---------------------------------------------------

	public:
		ClassMap ReleaseClasses();

		// ---------------------------------------------------

		//!	Disable copy assignment.
		ApiBuilder& operator=(const ApiBuilder&) = delete;

		// ---------------------------------------------------

	private:
		void* DefineVariable(CppType type, size_t byteSize, StringView wrenModule, StringView wrenName);

		void DefineClass(CppType type, StringView wrenModule, StringView wrenName, std::initializer_list<ForeignMethod> staticMethods, std::initializer_list<ForeignMethod> methods, void (*finalizer)(void*) ETNoexceptHint);

		// - DATA MEMBERS ------------------------------------

	private:
		WrenVM*  _vm;
		ClassMap _classByCppType;
	};

}}} // namespace Eldritch2::Scripting::Wren

#define ET_BUILTIN_WREN_MODULE_NAME(name) "builtin/" #name

//	Forward declarations for Wren built-in function used in the macros below.
void wrenSetSlotBytes(WrenVM* vm, int slot, const char* text, size_t length);
void wrenAbortFiber(WrenVM* vm, int slot);

#define ET_ABORT_WREN(message)                                      \
	{                                                               \
		::wrenSetSlotBytes(vm, 0, message, ETCountOf(message) - 1); \
		return ::wrenAbortFiber(vm, 0);                             \
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
