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

	ETInlineHint VkRect2D DisplaySource::GetOwnedRegion() const {
		return _ownedRegion;
	}

	// ---------------------------------------------------

	ETInlineHint void DisplaySource::SetOwnedRegion(VkRect2D region) {
		_ownedRegion = region;
	}

	// ---------------------------------------------------

	ETInlineHint bool Display::ShouldDestroy() const {
		auto IsOpen([](DisplaySource* source) { return source != nullptr; });

		return FindIf(eastl::begin(_sources), eastl::end(_sources), IsOpen) == eastl::end(_sources);
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
		constexpr VkRect2D EmptyRect{ { 0, 0 }, { 0u, 0u } };

		using ::Eldritch2::Swap;

		for (DisplaySource*& candidate : _sources) {
			if (candidate == eastl::addressof(source)) {
				candidate = eastl::exchange(_sources[LastElement], nullptr);
				source.SetOwnedRegion(EmptyRect);
				return;
			}
		}
	}

	// ---------------------------------------------------

	ETInlineHint void Swap(DisplaySource& lhs, DisplaySource& rhs) {
		using ::Eldritch2::Swap;

		Swap(lhs._ownedRegion, rhs._ownedRegion);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
