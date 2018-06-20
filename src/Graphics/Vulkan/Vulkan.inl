/*==================================================================*\
  Vulkan.inl
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

	template <size_t formatSize, typename... Arguments>
	ETInlineHint void Vulkan::WriteLog(Logging::MessageType type, const Utf8Char (&format)[formatSize], Arguments&&... arguments) const {
		_log.Write(type, format, eastl::forward<Arguments>(arguments)...);
	}

	// ---------------------------------------------------

	ETInlineHint Vulkan::Device& Vulkan::GetPrimaryDevice() {
		return _devices[0];
	}

	// ---------------------------------------------------

	ETInlineHint Vulkan::operator VkInstance() {
		return _vulkan;
	}

}}} // namespace Eldritch2::Graphics::Vulkan
