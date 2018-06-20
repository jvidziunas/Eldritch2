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
#include <Graphics/Vulkan/CommandList.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	BatchCoordinator::BatchCoordinator() :
		_drawParameters(),
		_countsByThing(MallocAllocator("Batch Coordinator Group Count Allocator")) {}

	//---------------------------------------------------

	BatchCoordinator::BatchCoordinator(BatchCoordinator&& coordinator) :
		BatchCoordinator() {
		Swap(*this, coordinator);
	}

	//---------------------------------------------------

	VkResult BatchCoordinator::BindResources(Gpu& gpu) {
		UniformBuffer drawParameters;

		ET_FAIL_UNLESS(drawParameters.BindResources(gpu, ParameterBufferSize));
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
		for (ArrayMap<void*, uint32_t>::ValueType& value : _countsByThing) {
			value.second = 0u;
		}
	}

	//---------------------------------------------------

	void BatchCoordinator::RecordDraws(CommandList& commands, const View& view, VertexBuffer& vertices, IndexBuffer& indices) const {
		commands.BindBuffers({ vertices.Get() }, indices.Get(), VK_INDEX_TYPE_UINT16);
		commands.SetStencilReference(VK_STENCIL_FRONT_AND_BACK, view.id);
		commands.SetStencilWriteMask(VK_STENCIL_FRONT_AND_BACK, 0xFFFF);
		commands.SetStencilReadMask(VK_STENCIL_FRONT_AND_BACK, 0xFFFF);
		commands.SetScissors(0u, 1u, &view.scissorRect);
		commands.SetDepthBias(0.0f, 0.0f, 0.0f);

		//	commands.DrawIndexedIndirect(_drawParameters, 0u, _countsByThing.GetSize());
		//	commands.Draw(0u, 0u, 0u, 0u);
	}

	//---------------------------------------------------

	void Swap(BatchCoordinator& lhs, BatchCoordinator& rhs) {
		using ::Eldritch2::Swap;

		Swap(lhs._drawParameters, rhs._drawParameters);
		Swap(rhs._countsByThing, rhs._countsByThing);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
