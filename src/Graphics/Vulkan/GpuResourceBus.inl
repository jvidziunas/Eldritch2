/*==================================================================*\
  GpuResourceBus.inl
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

	template <typename TargetResource, typename BindStruct>
	GpuResourceBus::PendingBind<TargetResource, BindStruct>::PendingBind(TargetResource target, const BindStruct& bind) :
		BindStruct(bind),
		target(target) {}

}}} // namespace Eldritch2::Graphics::Vulkan
