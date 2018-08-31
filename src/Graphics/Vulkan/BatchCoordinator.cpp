/*==================================================================*\
  BatchCoordinator.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/BatchCoordinator.hpp>
#include <Graphics/Vulkan/VulkanTools.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	BatchCoordinator::BatchCoordinator() :
		_drawParameters(),
		_countsByThing(MallocAllocator("Batch Coordinator Group Count Allocator")) {}

	//---------------------------------------------------

	BatchCoordinator::BatchCoordinator(BatchCoordinator&& batches) :
		BatchCoordinator() {
		Swap(*this, batches);
	}

	//---------------------------------------------------

	VkResult BatchCoordinator::BindResources(Gpu& gpu, VkDeviceSize parameterBufferSize, VkDeviceSize /*instanceBufferSize*/) {
		UniformBuffer drawParameters;

		ET_ABORT_UNLESS(drawParameters.BindResources(gpu, parameterBufferSize));
		ET_AT_SCOPE_EXIT(drawParameters.FreeResources(gpu));

		Swap(_drawParameters, drawParameters);

		return VK_SUCCESS;
	}

	//---------------------------------------------------

	void BatchCoordinator::FreeResources(Gpu& gpu) {
		_drawParameters.FreeResources(gpu);
	}

	//---------------------------------------------------

	void BatchCoordinator::AddThing(void* thing) {
		_countsByThing.TryEmplace(thing, 0u);
	}

	//---------------------------------------------------

	void BatchCoordinator::ResetCounts() {
		for (ArrayMap<void*, uint32>::ValueType& value : _countsByThing) {
			value.second = 0u;
		}
	}

	//---------------------------------------------------

	void Swap(BatchCoordinator& lhs, BatchCoordinator& rhs) {
		using ::Eldritch2::Swap;

		Swap(lhs._drawParameters, rhs._drawParameters);
		Swap(rhs._countsByThing, rhs._countsByThing);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
