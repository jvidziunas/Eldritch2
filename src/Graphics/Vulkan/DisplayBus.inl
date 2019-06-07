/*==================================================================*\
  DisplayBus.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	ETInlineHint ETForceInlineHint void DisplayBus::ScheduleFlip(VkSemaphore ready, VkSwapchainKHR swapchain, uint32 index, ResizeDelegate onResize) {
		ET_LOCK_SCOPE(_presentableImagesMutex);
		_presentableImages.EmplaceBack(VK_SUCCESS, Move(ready), Move(swapchain), Move(index), Move(onResize));
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint DisplayBus::DisplayList::SliceType DisplayBus::GetDisplays() ETNoexceptHint {
		return { _displays.Begin(), _displays.End() };
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint Mutex& DisplayBus::GetDisplaysMutex() ETNoexceptHint {
		return _displaysMutex;
	}

}}} // namespace Eldritch2::Graphics::Vulkan
