/*==================================================================*\
  VulkanTools.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/VulkanTools.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	ETPureFunctionHint const FormatDescriptor& DescribeFormat(GpuFormat format) ETNoexceptHint {
		static ETConstexpr FormatDescriptor descriptors[] = {};

		return descriptors[size_t(format)];
	}

}}} // namespace Eldritch2::Graphics::Vulkan
