/*==================================================================*\
  DisplayLocator.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/DisplayLocator.hpp>
#include <Graphics/Vulkan/GpuResourceApi.hpp>
#include <Graphics/Vulkan/DisplayBus.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	namespace {

		enum ViewportMasks : uintptr {
			ViewportBits = ETCountOf(Display::displayRects) >> 1u,
			ViewportMask = (1u << ViewportBits) - 1u,
			DisplayMask  = ~ViewportMask
		};

		// ---

		ETForceInlineHint ETPureFunctionHint ViewportId MakeId(Display* display, uint32 viewport) ETNoexceptHint {
			ETStaticAssert(ETAlignOf(Display) >= ViewportMask, "Unable to pack display and viewport!");
			ETAssert((viewport & DisplayMask) == 0, "Viewport index {} is out of range, valid range [0-{})", viewport, ETCountOf(Display::displayRects));

			return ViewportId(uintptr(display) | viewport);
		}

		// ---------------------------------------------------

		ETConstexpr ETForceInlineHint ETPureFunctionHint Display* GetDisplay(ViewportId id) ETNoexceptHint {
			return reinterpret_cast<Display*>(uintptr(id) & DisplayMask);
		}

		// ---------------------------------------------------

		ETConstexpr ETForceInlineHint ETPureFunctionHint uint32 GetViewport(ViewportId id) ETNoexceptHint {
			return uint32(uintptr(id) & ViewportMask);
		}

	} // anonymous namespace

	DisplayLocator::DisplayLocator() ETNoexceptHint : _bus(nullptr) {}

	// ---------------------------------------------------

	VkResult DisplayLocator::PresentImages() const {
		// bus->ScheduleFlip();
		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	ViewportId DisplayLocator::TryAcquireViewport(const PipelineBuilder& pipeline) {
		ET_LOCK_SCOPE(_bus->GetDisplaysMutex());
		for (Display& display : _bus->GetDisplays()) {
			for (uint32 viewport(0u); viewport < ETCountOf(Display::displayRects); ++viewport) {
				if (display.displayRects[viewport].extent.width != 0 || display.displayRects[viewport].extent.height != 0) {
					continue;
				}

				return MakeId(ETAddressOf(display), viewport);
			}
		}

		return ViewportId::Invalid;
		// End of lock scope.
	}

	// ---------------------------------------------------

	void DisplayLocator::ReleaseViewport(ViewportId id) ETNoexceptHint {
		ETAssert(id != ViewportId::Invalid, "Releasing invalid ID (display={} viewport={})", fmt::ptr(GetDisplay(id)), GetViewport(id));
		_framebufferById.Find(id);
	}

	// ---------------------------------------------------

	VkResult DisplayLocator::BindResources(Gpu& gpu, DisplayBus& bus) {
		ArrayMap<ViewportId, Framebuffer> framebufferById(_framebufferById.GetAllocator());
		ET_AT_SCOPE_EXIT(framebufferById.ClearAndDispose([&](Framebuffer& framebuffer) ETNoexceptHint {
			framebuffer.FreeResources(gpu);
		}));

		Swap(_framebufferById, framebufferById);

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void DisplayLocator::FreeResources(Gpu& gpu) ETNoexceptHint {
		_framebufferById.ClearAndDispose([&](Framebuffer& framebuffer) ETNoexceptHint {
			framebuffer.FreeResources(gpu);
		});
	}

}}} // namespace Eldritch2::Graphics::Vulkan
