/*==================================================================*\
  Transformation.WrenScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Wren/ApiBuilder.hpp>
#include <Scripting/Wren/Context.hpp>
//------------------------------------------------------------------//

void	wrenSetSlotBytes(WrenVM* vm, int slot, const char* text, size_t length);

namespace Eldritch2 {
	namespace Scripting {
		namespace Wren {

			ET_IMPLEMENT_WREN_CLASS(Transformation) {
				api.CreateClass<Transformation>(ET_BUILTIN_WREN_MODULE_NAME(Math), "Transformation",
												{/* Constructors */
													ConstructorMethod("new(_,_)", [](WrenVM* vm) {
														SetReturn<Transformation>(vm, GetSlotAs<Vector>(vm, 1), GetSlotAs<Quaternion>(vm, 2));
													})
												},
			{/*	Static methods */
				StaticMethod("identity", [](WrenVM* vm) {
					SetReturn<Transformation>(vm, Vector(0.0f, 0.0f, 0.0f, 0.0f), Quaternion(1.0f, 0.0f, 0.0f, 0.0f));
				})
			},
			{/*	Properties */
				DefineProperty("translation",
				//	Getter
					[](WrenVM* vm) {
						const Transformation&	self(GetSlotAs<Transformation>(vm, 0));

						wrenSetSlotHandle(vm, 0, AsContext(vm).FindForeignClass<Vector>());
						SetReturn<Vector>(vm, self.translation);
					},
				//	Setter
					[](WrenVM* vm) {
						GetSlotAs<Transformation>(vm, 0).translation = GetSlotAs<Vector>(vm, 1);
					}
				),
				DefineProperty("rotation",
				//	Getter
					[](WrenVM* vm) {
						const Transformation&	self(GetSlotAs<Transformation>(vm, 0));

						wrenSetSlotHandle(vm, 0, AsContext(vm).FindForeignClass<Quaternion>());
						SetReturn<Quaternion>(vm, self.rotation);
					},
				//	Setter
					[](WrenVM* vm) {
						GetSlotAs<Transformation>(vm, 0).rotation = GetSlotAs<Quaternion>(vm, 1);
					}
				),
				DefineGetter("toString", [](WrenVM* vm) {
					const Transformation&	self(GetSlotAs<Transformation>(vm, 0));
					fmt::memory_buffer		string;
					float32					translation[4];
					float32					rotation[4];

					self.translation.ExtractCoefficients(translation);
					self.rotation.ExtractCoefficients(rotation);

					fmt::format_to(string, "<<x={}, y={}, z={}>, <i={}, j={}, k={}, w={}>>", translation[3], translation[2], translation[1], rotation[3], rotation[2], rotation[1], rotation[0]);

					wrenSetSlotBytes(vm, 0, string.data(), string.size());
				})
			},
			{/*	Methods */
				ForeignMethod("~", [](WrenVM* vm) {
					const Transformation&	self(GetSlotAs<Transformation>(vm, 0));

					wrenSetSlotHandle(vm, 0, AsContext(vm).FindForeignClass<Transformation>());
					SetReturn<Transformation>(vm, self.GetInverse());
				})
			}
			);
			}

		}	// namespace Wren
	}	// namespace Scripting
}	// namespace Eldritch2