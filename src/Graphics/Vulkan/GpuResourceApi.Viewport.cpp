/*==================================================================*\
  GpuResourceApi.Viewport.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/GpuResourceApi.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	void Swap(Viewport& lhs, Viewport& rhs) ETNoexceptHint {
		Swap(static_cast<Framebuffer&>(lhs), rhs);
		lhs._generator.store(rhs._generator.exchange(lhs._generator.load(std::memory_order_consume), std::memory_order_release), std::memory_order_release);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
