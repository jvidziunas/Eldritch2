/*==================================================================*\
  Planner.WrenScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Wren/ApiBuilder.hpp>
#include <Scripting/Wren/Planner.hpp>
#include <Scripting/Wren/Marshal.hpp>
//------------------------------------------------------------------//

void	wrenGetListElement(WrenVM* vm, int listSlot, int index, int elementSlot);
void	wrenEnsureSlots(WrenVM* vm, int numSlots);
int		wrenGetListCount(WrenVM* vm, int slot);

namespace Eldritch2 {
	namespace Scripting {
		namespace Wren {

			ET_IMPLEMENT_WREN_CLASS(Planner) {
				api.CreateClass<Planner>(ET_BUILTIN_WREN_MODULE_NAME(Core), "Planner",
										 {/* Constructors */
											 ConstructorMethod("ofActions(_)", [](WrenVM* vm) {
												 wrenEnsureSlots(vm, 3);

												 for (int i(0), count(wrenGetListCount(vm, 1)); i < count; ++i) {
													 wrenGetListElement(vm, 1, i, 2);
												 }

												 SetReturn<Planner>(vm);
											 })
										 },
											 {/*	Static methods */ },
											 {/*	Properties */ },
											 {/*	Methods */ }
											 );
			}

		}	// namespace Wren
	}	// namespace Scripting
}	// namespace Eldritch2