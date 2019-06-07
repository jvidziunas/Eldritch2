/*==================================================================*\
  ScriptExecutor.hpp
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
namespace Core {
	class AssetDatabase;
} // namespace Core
} // namespace Eldritch2

struct WrenHandle;
struct WrenVM;

namespace Eldritch2 { namespace Scripting { namespace Wren {

	class ScriptExecutor {
		// - TYPE PUBLISHING ---------------------------------

	public:
		using ClassMap = CachingHashMap<CppType, WrenHandle*>;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		ScriptExecutor(const ScriptExecutor&) = delete;
		//!	Constructs this @ref ScriptExecutor instance.
		ScriptExecutor() ETNoexceptHint;

		~ScriptExecutor();

		// ---------------------------------------------------

	public:
		template <typename... Arguments>
		bool Call(WrenHandle* receiver, Arguments... arguments) ETNoexceptHint;

		template <typename... Arguments>
		bool New(WrenHandle* wrenClass, Arguments... arguments) ETNoexceptHint;

		bool Interpret(const char* source) ETNoexceptHint;

		// ---------------------------------------------------

	public:
		WrenHandle* BindCallStub(const char* signature) ETNoexceptHint;

		void Free(WrenHandle* handle) ETNoexceptHint;

		// ---------------------------------------------------

	public:
		ApiBuilder CreateApiBuilder(uint32 heapByteSize);

		// ---------------------------------------------------

	public:
		Result BindResources(Logging::Log& log, const Core::AssetDatabase& assets, ApiBuilder api);

		void FreeResources() ETNoexceptHint;

		// - DATA MEMBERS ------------------------------------

	private:
		Logging::ChildLog          _log;
		const Core::AssetDatabase* _assets;
		WrenVM*                    _vm;
		ClassMap                   _classByCppType;
		WrenHandle*                _callStubByArity[8];
		WrenHandle*                _newStubByArity[8];

		// ---------------------------------------------------

		template <typename Class>
		friend void wrenSetSlotCppTypeHandle(WrenVM* vm, int slot) ETNoexceptHint;

		friend void Swap(ScriptExecutor&, ScriptExecutor&) ETNoexceptHint;
	};

	// ---------------------------------------------------

	ETPureFunctionHint ScriptExecutor* GetExecutor(WrenVM* vm) ETNoexceptHint;

}}} // namespace Eldritch2::Scripting::Wren

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Scripting/Wren/ScriptExecutor.inl>
//------------------------------------------------------------------//
