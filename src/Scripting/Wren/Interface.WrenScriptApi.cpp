/*==================================================================*\
  Interface.WrenScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Wren/ScriptExecutor.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
#include <Scripting/Wren/Interface.hpp>
#include <Graphics/RgbColor.hpp>
//------------------------------------------------------------------//
#include <wren.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Scripting { namespace Wren {

	using namespace ::Eldritch2::Graphics;

	namespace {

		ETForceInlineHint nk_flags wrenGetSlotFlags(WrenVM* vm, int slot) ETNoexceptHint {
			return nk_flags(AsUintBits(wrenGetSlotDouble(vm, slot)));
		}

		// ---------------------------------------------------

		ETForceInlineHint nk_text_align wrenGetSlotTextAlignment(WrenVM* vm, int slot) ETNoexceptHint {
			return nk_text_align(AsUintBits(wrenGetSlotDouble(vm, slot)));
		}

		// ---------------------------------------------------

		ETForceInlineHint nk_color wrenGetSlotColor(WrenVM* vm, int slot) ETNoexceptHint {
			const RgbColor& color(wrenGetSlotAs<RgbColor>(vm, slot));
			return nk_rgb_f(color.GetRed(), color.GetGreen(), color.GetBlue());
		}

	} // anonymous namespace

	ET_IMPLEMENT_WREN_CLASS(Interface) {
		api.DefineClass<Interface>(ET_BUILTIN_WREN_MODULE_NAME(Interface), "Interface", // clang-format off
			{/*	Static methods */ },
			{/*	Methods */
				ForeignMethod("doWindow(_,_)", [](WrenVM* vm) ETNoexceptHint {
					// wrenSwapSlots(vm, 0, 3);
					wrenGetSlotAs<Interface>(vm, 0).DoWindow(wrenGetSlotString(vm, 1), wrenGetSlotFlags(vm, 2), {}, [vm](Interface& /*interface*/) {
						GetExecutor(vm)->Call(nullptr);
					});
					// wrenSwapSlots(vm, 0, 3);
				}),
				ForeignMethod("doStaticPopup(_,_)", [](WrenVM* vm) ETNoexceptHint {
					// wrenSwapSlots(vm, 0, 3);
					wrenGetSlotAs<Interface>(vm, 0).DoStaticPopup(wrenGetSlotString(vm, 1), wrenGetSlotFlags(vm, 2), {}, [vm](Interface& /*interface*/) {
						GetExecutor(vm)->Call(nullptr);
					});
					// wrenSwapSlots(vm, 0, 3);
				}),
				ForeignMethod("doDynamicPopup(_,_)", [](WrenVM* vm) ETNoexceptHint {
					// wrenSwapSlots(vm, 0, 3);
					wrenGetSlotAs<Interface>(vm, 0).DoDynamicPopup(wrenGetSlotString(vm, 1), wrenGetSlotFlags(vm, 2), {}, [vm](Interface& /*interface*/) {
						GetExecutor(vm)->Call(nullptr);
					});
					// wrenSwapSlots(vm, 0, 3);
				}),
				ForeignMethod("doTooltip(_,_)", [](WrenVM* vm) ETNoexceptHint {
					// wrenSwapSlots(vm, 0, 3);
					wrenGetSlotAs<Interface>(vm, 0).DoTooltip(wrenGetSlotDouble(vm, 1), [vm](Interface& /*interface*/) {
						GetExecutor(vm)->Call(nullptr);
					});
					// wrenSwapSlots(vm, 0, 3);
				}),
			/*	To consider: Strings are immutable in Wren.
				ForeignMethod("doTextEditBox(_,_)", [](WrenVM* vm) ETNoexceptHint {
					wrenGetSlotAs<Interface>(vm, 0).DoTextEditBox(wrenGetSlotDouble(vm, 1), wrenGetSlotString(vm, 2));
				}),
				ForeignMethod("doButton(_,_,_)", [](WrenVM* vm) ETNoexceptHint {
				//	imageId = wrenGetSlotDouble(vm, 1);
					wrenSetSlotBool(vm, 0, wrenGetSlotAs<Interface>(vm, 0).DoButton(nullptr, wrenGetSlotString(vm, 2), GetSlotTextAlignment(vm, 3)));
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
					wrenGetSlotAs<Interface>(vm, 0).DoText(wrenGetSlotFlags(vm, 1), wrenGetSlotColor(vm, 2), wrenGetSlotString(vm, 3));
				}),
			//	void	DoImage(const Graphics::ImageSource& image);
				ForeignMethod("doProperty(_,_,_,_,_,_)", [](WrenVM* vm) ETNoexceptHint {
					double value(wrenGetSlotDouble(vm, 2));
					wrenGetSlotAs<Interface>(vm, 0).DoProperty(wrenGetSlotDouble(vm, 1), ETAddressOf(value), wrenGetSlotDouble(vm, 3), wrenGetSlotDouble(vm, 4), wrenGetSlotDouble(vm, 5), wrenGetSlotString(vm, 6));
					wrenSetSlotDouble(vm, 0, value);
				}), }); // clang-format on
	}

}}} // namespace Eldritch2::Scripting::Wren
