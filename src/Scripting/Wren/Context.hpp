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
namespace Scripting {
namespace Wren {
namespace AssetViews {
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
		template <typename Value, typename Allocator = MallocAllocator>
		using TypeMap = CachingHashMap<Type, Value, Hash<Type>, EqualTo<Type>, Allocator>;

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
		WrenHandle* GetCallStubForArity() const;

		// ---------------------------------------------------

	public:
		template <typename Type>
		WrenHandle* FindForeignClass() const;

		WrenHandle* CreateForeignClass(WrenVM* vm, const char* module, const char* name, Type type);

		void* CreateVariable(WrenVM* vm, const char* module, const char* name, WrenHandle* klass, size_t size);
		void  CreateVariable(WrenVM* vm, const char* module, const char* name, double value);

		// ---------------------------------------------------

	public:
		const AssetViews::ScriptAsset* FindScriptModule(const char* path) const;

		// ---------------------------------------------------

	public:
		template <size_t formatSize, typename... Arguments>
		void WriteLog(Logging::MessageType type, const Utf8Char (&format)[formatSize], Arguments&&... arguments) const;
		void WriteLog(const Utf8Char* const string, size_t lengthInOctets) const;

		// ---------------------------------------------------

	public:
		void BindResources(WrenVM* vm);

		void FreeResources(WrenVM* vm);

		// - DATA MEMBERS ------------------------------------

	private:
		//!	Log used to track errors and print debug messages.
		mutable Logging::ChildLog _log;
		//!	@ref AssetDatabase used to service asset/script import requests. Never null.
		const Assets::AssetDatabase* _assets;
		//!	Database of Wren classes indexed by C++ type.
		TypeMap<WrenHandle*> _classesByType;
		WrenHandle*          _callStubByArity[16];
	};

	// ---------------------------------------------------

	Context& AsContext(WrenVM* vm);

}}} // namespace Eldritch2::Scripting::Wren

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Scripting/Wren/Context.inl>
//------------------------------------------------------------------//
