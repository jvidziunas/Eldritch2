/*==================================================================*\
  Display.inl
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	ETInlineHint VkExtent2D DisplaySource::GetValidExtent() const {
		return _validExtent;
	}

	// ---------------------------------------------------

	ETInlineHint void DisplaySource::SetValidExtent(VkExtent2D extent) {
		_validExtent = extent;
	}

	// ---------------------------------------------------

	ETInlineHint bool Display::ShouldDestroy() const {
		return eastl::end(_sources) == FindIf(eastl::begin(_sources), eastl::end(_sources), [](DisplaySource* source) { return source != nullptr; });
	}

	// ---------------------------------------------------

	ETInlineHint bool Display::TryAcquire(DisplaySource& source) {
		for (DisplaySource*& candidate : _sources) {
			if (candidate == nullptr) {
				candidate = eastl::addressof(source);
				return true;
			}
		}

		return false;
	}

	// ---------------------------------------------------

	ETInlineHint void Display::Release(DisplaySource& source) {
		enum : size_t { LastElement = _countof(_sources) - 1 };

		using ::Eldritch2::Swap;

		for (DisplaySource*& candidate : _sources) {
			if (candidate == eastl::addressof(source)) {
				candidate = eastl::exchange(_sources[LastElement], nullptr);
				return;
			}
		}
	}

}}} // namespace Eldritch2::Graphics::Vulkan
