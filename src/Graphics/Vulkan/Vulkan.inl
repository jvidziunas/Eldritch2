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

	template <typename... Arguments>
	ETInlineHint void Vulkan::WriteLog(Logging::Severity severity, StringView format, Arguments&&... arguments) const {
		_log.Write(severity, format, eastl::forward<Arguments>(arguments)...);
	}

	// ---------------------------------------------------

	ETInlineHint Vulkan::Device& Vulkan::GetPrimaryDevice() ETNoexceptHint {
		return _device;
	}

	// ---------------------------------------------------

	ETInlineHint Vulkan::operator VkInstance() ETNoexceptHint {
		return _vulkan;
	}

}}} // namespace Eldritch2::Graphics::Vulkan
