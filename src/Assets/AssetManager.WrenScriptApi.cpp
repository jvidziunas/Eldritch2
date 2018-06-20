/*==================================================================*\
  AssetManager.WrenScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Wren/ApiBuilder.hpp>
#include <Assets/AssetManager.hpp>
//------------------------------------------------------------------//

const char* wrenGetSlotString(WrenVM* vm, int slot);

namespace Eldritch2 {
	namespace Assets {

		using namespace ::Eldritch2::Scripting::Wren;

		ET_IMPLEMENT_WREN_CLASS(Asset) {
			api.CreateClass<AssetReference>(ET_BUILTIN_WREN_MODULE_NAME(Core), "Asset",
											{/* Constructors */
												ConstructorMethod("require(_,_)", [](WrenVM* vm) {
													const Utf8Char* const	path(wrenGetSlotString(vm, 2));
													const Asset* const		asset(GetSlotAs<AssetManager>(vm, 1).Find(path));

													ET_ABORT_WREN_UNLESS_FMT(asset, "Asset '{}' not loaded.", path);

													SetReturn<AssetReference>(vm, *asset);
												})
											},
												{/*	Static methods */ },
												{/*	Properties */ },
												{/*	Methods */ }
												);
		}

		ET_IMPLEMENT_WREN_CLASS(AssetManager) {
			api.CreateClass<AssetManager>(ET_BUILTIN_WREN_MODULE_NAME(Core), "AssetManager",
										  {/* Constructors */ },
										  {/*	Static methods */ },
										  {/*	Properties */ },
										  {/*	Methods */ }
			);
		}

	}	// namespace Assets
}	// namespace Eldritch2