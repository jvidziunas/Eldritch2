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
namespace Scripting { namespace Wren { namespace AssetViews {
	class ScriptAsset;
}}} // namespace Scripting::Wren::AssetViews
namespace Assets {
	class AssetReference;
	class AssetDatabase;
} // namespace Assets
} // namespace Eldritch2

struct WrenHandle;
struct WrenVM;

namespace Eldritch2 { namespace Scripting { namespace Wren {

	class Context {
		// - TYPE PUBLISHING ---------------------------------

	public:
		using ClassMap = CachingHashMap<CppType, WrenHandle*, Hash<CppType>, EqualTo<CppType>, MallocAllocator>;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref Context instance.
		Context(const Assets::AssetDatabase& assets, Logging::Log& log);
		//!	Disable copy construction.
		Context(const Context&) = delete;

		~Context();

		// ---------------------------------------------------

	public:
		template <size_t arity>
		WrenHandle* GetCallStubForArity() const ETNoexceptHint;

		// ---------------------------------------------------

	public:
		template <typename... Arguments>
		void WriteLog(Logging::Severity type, StringView format, Arguments&&... arguments) const;
		void WriteLog(const Utf8Char* const string, size_t lengthInOctets) const ETNoexceptHint;

		// ---------------------------------------------------

	public:
		template <typename CppType>
		WrenHandle* FindForeignClass() const ETNoexceptHint;

		WrenHandle* CreateForeignClass(WrenVM* vm, StringView module, StringView name, CppType type);

		void*  CreateVariable(WrenVM* vm, StringView module, StringView name, WrenHandle* wrenClass, size_t size) ETNoexceptHint;
		double CreateVariable(WrenVM* vm, StringView module, StringView name, double value) ETNoexceptHint;

		// ---------------------------------------------------

	public:
		const AssetViews::ScriptAsset* FindScriptModule(StringView path) const ETNoexceptHint;

		// ---------------------------------------------------

	public:
		void BindResources(WrenVM* vm);

		void FreeResources(WrenVM* vm);

		// - DATA MEMBERS ------------------------------------

	private:
		mutable Logging::ChildLog    _log;
		const Assets::AssetDatabase* _assets;
		ClassMap                     _classesByType;
		WrenHandle*                  _callStubByArity[16];
	};

	// ---------------------------------------------------

	ETPureFunctionHint Context* GetContext(WrenVM* vm) ETNoexceptHint;

}}} // namespace Eldritch2::Scripting::Wren

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Scripting/Wren/Context.inl>
//------------------------------------------------------------------//
