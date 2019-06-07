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

#if !defined(ETWrenCall)
#	define ETWrenCall ETCDecl
#endif

#define ET_BUILTIN_WREN_MODULE_NAME(name) "builtin/" #name

struct WrenHandle;
struct WrenVM;

namespace Eldritch2 { namespace Scripting { namespace Wren {

	class ForeignMethod {
		// - TYPE PUBLISHING ---------------------------------

	public:
		using Body = void(ETWrenCall*)(WrenVM*) ETNoexceptHint;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref ForeignMethod instance.
		ETConstexpr ForeignMethod(const ForeignMethod&) ETNoexceptHint = default;
		//!	Constructs this @ref ForeignMethod instance.
		ForeignMethod(StringSpan signature, Body body) ETNoexceptHint;
		//!	Constructs this @ref ForeignMethod instance.
		ETConstexpr ForeignMethod() ETNoexceptHint;

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
		Property(StringSpan name, ForeignMethod::Body getter, ForeignMethod::Body setter) ETNoexceptHint;
		//!	Constructs this @ref Property instance.
		ETConstexpr Property(const Property&) ETNoexceptHint = default;

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
		//!	Disable copy construction.
		ApiBuilder(const ApiBuilder&) = delete;
		//!	Constructs this @ref ApiBuilder instance.
		ApiBuilder(ApiBuilder&&) ETNoexceptHint;
		//!	Constructs this @ref ApiBuilder instance.
		ApiBuilder(WrenVM* vm) ETNoexceptHint;

		~ApiBuilder();

		// ---------------------------------------------------

	public:
		template <class Class>
		void DefineClass(StringSpan name, StringSpan moduleName, InitializerList<ForeignMethod> staticMethods, InitializerList<ForeignMethod> methods);

		template <class Class, typename... Arguments>
		Class* DefineVariable(StringSpan name, StringSpan moduleName, Arguments&&... arguments);
		double DefineVariable(StringSpan name, StringSpan moduleName, double value);

		// ---------------------------------------------------

	public:
		ETConstexpr ClassMap& GetClasses() ETNoexceptHint;

		ETConstexpr WrenVM*& GetVm() ETNoexceptHint;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		ApiBuilder& operator=(const ApiBuilder&) = delete;

		// ---------------------------------------------------

	private:
		void* DefineVariable(CppType type, size_t byteSize, StringSpan name, StringSpan moduleName);

		void DefineClass(CppType type, StringSpan name, StringSpan moduleName, InitializerList<ForeignMethod> staticMethods, InitializerList<ForeignMethod> methods, void(ETWrenCall* finalizer)(void*) ETNoexceptHint);

		// - DATA MEMBERS ------------------------------------

	private:
		WrenVM*  _vm;
		ClassMap _classByCppType;
	};

}}} // namespace Eldritch2::Scripting::Wren

//	Forward declarations for Wren built-in functions used in macros.
void wrenSetSlotBytes(WrenVM* vm, int slot, const char* text, size_t length);
void wrenAbortFiber(WrenVM* vm, int slot);

// clang-format off
#define ET_ABORT_WREN(vm, message, ...)                                 \
	{                                                                   \
		::fmt::memory_buffer text;                                      \
		::fmt::format_to(text, ::fmt::string_view(message), __VA_ARGS__); \
		::wrenSetSlotBytes(vm, 0, text.data(), text.size());              \
		return ::wrenAbortFiber(vm, 0);                                   \
	}
#define ET_ABORT_WREN_IF(condition, vm, message, ...) if ((condition)) ET_ABORT_WREN(vm, message, __VA_ARGS__)
#define ET_ABORT_WREN_UNLESS(condition, vm, message, ...) if (!(condition)) ET_ABORT_WREN(vm, message, __VA_ARGS__)

#define ET_DECLARE_WREN_CLASS(name) void ET_CPP_JOIN(name, WrenDefinition)(::Eldritch2::Scripting::Wren::ApiBuilder&, ::Eldritch2::StringSpan)
#define ET_IMPLEMENT_WREN_CLASS(name, api, moduleName) void ET_CPP_JOIN(name, WrenDefinition)(::Eldritch2::Scripting::Wren::ApiBuilder& api, ::Eldritch2::StringSpan moduleName)
#define ET_REGISTER_WREN_CLASS(name, api, moduleName) ET_CPP_JOIN(name, WrenDefinition)(api, moduleName)
// clang-format on

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Scripting/Wren/ApiBuilder.inl>
//------------------------------------------------------------------//
