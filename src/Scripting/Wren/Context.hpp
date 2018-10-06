/*==================================================================*\
  Context.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Logging/ChildLog.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scripting { namespace Wren {
	class ApiBuilder;
}} // namespace Scripting::Wren
namespace Assets {
	class AssetDatabase;
} // namespace Assets
} // namespace Eldritch2

struct WrenHandle;
struct WrenVM;

namespace Eldritch2 { namespace Scripting { namespace Wren {

	class Context {
		// - TYPE PUBLISHING ---------------------------------

	public:
		using ClassMap = CachingHashMap<CppType, WrenHandle*>;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		Context(const Context&) = delete;
		//!	Constructs this @ref Context instance.
		Context();

		~Context();

		// ---------------------------------------------------

	public:
		template <typename... Arguments>
		bool Call(WrenHandle* receiver, Arguments... arguments);

		bool Interpret(const char* source);

		// ---------------------------------------------------

	public:
		WrenHandle* BindCallStub(const char* signature) ETNoexceptHint;

		void Free(WrenHandle* handle) ETNoexceptHint;

		// ---------------------------------------------------

	public:
		ErrorCode BindResources(const Assets::AssetDatabase& assets, Logging::Log& log, Function<void(ApiBuilder&)> apiBuilder);

		void FreeResources();

		// - DATA MEMBERS ------------------------------------

	private:
		Logging::ChildLog            _log;
		const Assets::AssetDatabase* _assets;
		WrenVM*                      _vm;
		ClassMap                     _classByCppType;
		WrenHandle*                  _callStubByArity[8];

		// ---------------------------------------------------

		template <typename Class>
		friend void ::wrenSetSlotCppTypeHandle(WrenVM* vm, int slot) ETNoexceptHint;

		friend void Swap(Context&, Context&);
	};

	// ---------------------------------------------------

	ETPureFunctionHint Context* GetContext(WrenVM* vm) ETNoexceptHint;

}}} // namespace Eldritch2::Scripting::Wren

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Scripting/Wren/Context.inl>
//------------------------------------------------------------------//
