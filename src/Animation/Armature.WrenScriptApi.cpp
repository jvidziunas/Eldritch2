/*==================================================================*\
  Armature.WrenScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Wren/ApiBuilder.hpp>
#include <Scripting/Wren/Context.hpp>
#include <Animation/Armature.hpp>
//------------------------------------------------------------------//

void	wrenSetSlotHandle(WrenVM* vm, int slot, WrenHandle* handle);

namespace Eldritch2 {
	namespace Animation {

		using namespace ::Eldritch2::Scripting::Wren;
		using namespace ::Eldritch2::Scripting;

		ET_IMPLEMENT_WREN_CLASS(Armature) {
			api.CreateClass<Armature>(ET_BUILTIN_WREN_MODULE_NAME(Animation), "Armature",
									  {/* Constructors */
										  ConstructorMethod("new(_)", [](WrenVM* vm) {
											  SetReturn<Armature>(vm, GetSlotAs<Transformation>(vm, 1));
										  })
									  },
										  {/*	Static methods */ },
			{/*	Properties */
				DefineProperty("localToWorld",
				//	Getter
					[](WrenVM* vm) {
						Armature&	self(GetSlotAs<Armature>(vm, 0));

						wrenSetSlotHandle(vm, 0, AsContext(vm).FindForeignClass<Transformation>());
						SetReturn<Transformation>(vm, self.GetLocalToWorld());
					},
				//	Setter
					[](WrenVM* vm) {
						GetSlotAs<Armature>(vm, 0).SetLocalToWorld(GetSlotAs<Transformation>(vm, 1));
					}
				),
				DefineGetter("worldToLocal", [](WrenVM* vm) {
					Armature&	self(GetSlotAs<Armature>(vm, 0));

					wrenSetSlotHandle(vm, 0, AsContext(vm).FindForeignClass<Transformation>());
					SetReturn<Transformation>(vm, self.GetWorldToLocal());
				})
			},
				{/*	Methods */ }
				);
		}

	}	// namespace Animation
}	// namespace Eldritch2