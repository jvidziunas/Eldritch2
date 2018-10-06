/*==================================================================*\
  Interface.WrenScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Wren/ApiBuilder.hpp>
#include <Scripting/Wren/Interface.hpp>
#include <Scripting/Wren/Context.hpp>
#include <Graphics/RgbColor.hpp>
//------------------------------------------------------------------//
#include <wren.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Scripting { namespace Wren {

	using namespace ::Eldritch2::Graphics;

	namespace {

		ETInlineHint ETForceInlineHint nk_flags GetSlotFlags(WrenVM* vm, int slot) ETNoexceptHint {
			return nk_flags(AsUintBits(wrenGetSlotDouble(vm, slot)));
		}

		// ---------------------------------------------------

		ETInlineHint ETForceInlineHint nk_text_align GetSlotTextAlignment(WrenVM* vm, int slot) ETNoexceptHint {
			return nk_text_align(AsUintBits(wrenGetSlotDouble(vm, slot)));
		}

		// ---------------------------------------------------

		ETInlineHint ETForceInlineHint nk_color GetSlotColor(WrenVM* vm, int slot) ETNoexceptHint {
			const RgbColor& color(GetSlotAs<RgbColor>(vm, slot));
			return nk_rgb_f(color.GetRed(), color.GetGreen(), color.GetBlue());
		}

	} // anonymous namespace

	ET_IMPLEMENT_WREN_CLASS(Interface) {
		api.DefineClass<Interface>(ET_BUILTIN_WREN_MODULE_NAME(Interface), "Interface", // clang-format off
			{/*	Static methods */ },
			{/*	Methods */
				ForeignMethod("doWindow(_,_)", [](WrenVM* vm) ETNoexceptHint {
					GetSlotAs<Interface>(vm, 0).DoWindow(wrenGetSlotString(vm, 1), GetSlotFlags(vm, 2), {}, [vm](Interface& /*interface*/) {
						// wrenSwapSlots(vm, 0, 3);
						wrenCall(vm, GetContext(vm)->GetCallStubForArity<1>());
					});
				}),
				ForeignMethod("doStaticPopup(_,_)", [](WrenVM* vm) ETNoexceptHint {
					GetSlotAs<Interface>(vm, 0).DoStaticPopup(wrenGetSlotString(vm, 1), GetSlotFlags(vm, 2), {}, [vm](Interface& /*interface*/) {
						// wrenSwapSlots(vm, 0, 3);
						wrenCall(vm, GetContext(vm)->GetCallStubForArity<1>());
					});
				}),
				ForeignMethod("doDynamicPopup(_,_)", [](WrenVM* vm) ETNoexceptHint {
					GetSlotAs<Interface>(vm, 0).DoDynamicPopup(wrenGetSlotString(vm, 1), GetSlotFlags(vm, 2), {}, [vm](Interface& /*interface*/) {
						// wrenSwapSlots(vm, 0, 3);
						wrenCall(vm, GetContext(vm)->GetCallStubForArity<1>());
					});
				}),
				ForeignMethod("doTooltip(_,_)", [](WrenVM* vm) ETNoexceptHint {
					GetSlotAs<Interface>(vm, 0).DoTooltip(wrenGetSlotDouble(vm, 1), [vm](Interface& /*interface*/) {
						// wrenSwapSlots(vm, 0, 3);
						wrenCall(vm, GetContext(vm)->GetCallStubForArity<1>());
					});
				}),
			/*	To consider: Strings are immutable in Wren.
				ForeignMethod("doTextEditBox(_,_)", [](WrenVM* vm) ETNoexceptHint {
					GetSlotAs<Interface>(vm, 0).DoTextEditBox(wrenGetSlotDouble(vm, 1), wrenGetSlotString(vm, 2));
				}),
				ForeignMethod("doButton(_,_,_)", [](WrenVM* vm) ETNoexceptHint {
				//	imageId = wrenGetSlotDouble(vm, 1);
					wrenSetSlotBool(vm, 0, GetSlotAs<Interface>(vm, 0).DoButton(nullptr, wrenGetSlotString(vm, 2), GetSlotTextAlignment(vm, 3)));
				}) */
			//	bool	DoCheckbox(unsigned int* flags, unsigned int value, const Utf8Char* const text);
			//	bool	DoCheckbox(bool* active, const Utf8Char* const text);
			//	bool	DoRadioButton(int* isActive, const Utf8Char* const text);
			//	bool	DoRadioLabel(nk_text_align alignment, int* value, const Utf8Char* const text);
			//	bool	DoSlider(float min, float* val, float max, float step);
			//	bool	DoSlider(int min, int* val, int max, int step);
			//	bool	DoProgressBar(nk_size* cur, nk_size max, bool isMutable);
			//	bool	DoColorPicker(nk_colorf* color, nk_color_format format);
				ForeignMethod("doText(_,_,_)", [](WrenVM* vm) ETNoexceptHint {
					GetSlotAs<Interface>(vm, 0).DoText(GetSlotFlags(vm, 1), GetSlotColor(vm, 2), wrenGetSlotString(vm, 3));
				}),
			//	void	DoImage(const Graphics::ImageSource& image);
				ForeignMethod("doProperty(_,_,_,_,_,_)", [](WrenVM* vm) ETNoexceptHint {
					double value(wrenGetSlotDouble(vm, 2));
					GetSlotAs<Interface>(vm, 0).DoProperty(wrenGetSlotDouble(vm, 1), ETAddressOf(value), wrenGetSlotDouble(vm, 3), wrenGetSlotDouble(vm, 4), wrenGetSlotDouble(vm, 5), wrenGetSlotString(vm, 6));
					wrenSetSlotDouble(vm, 0, value);
				}), }); // clang-format on
	}

}}} // namespace Eldritch2::Scripting::Wren
