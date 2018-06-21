/*==================================================================*\
  ResidencyCoordinator.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/ResidencyCoordinator.hpp>
#include <Graphics/Vulkan/VulkanTools.hpp>
#include <Scheduling/JobExecutor.hpp>
#include <Graphics/ImageSource.hpp>
#include <Graphics/MeshSource.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	using namespace ::Eldritch2::Scheduling;

	ResidencyCoordinator::ResidencyCoordinator() :
		_meshesBySource(MallocAllocator("Geometry By Source Collection Allocator")),
		_imagesBySource(MallocAllocator("Image By Source Collection Allocator")) {
	}

	// ---------------------------------------------------

	VkResult ResidencyCoordinator::SubmitFrameIo(JobExecutor& executor, Gpu& gpu) {
		executor.AwaitCondition(_bus.CheckCommandsConsumed(gpu));

		return _bus.SubmitCommands(gpu);
	}

	// ---------------------------------------------------

	VkResult ResidencyCoordinator::BindResources(Gpu& gpu, VkDeviceSize transferBufferSize) {
		using Eldritch2::Swap;

		GpuResourceBus bus;
		ET_FAIL_UNLESS(bus.BindResources(gpu, transferBufferSize));
		ET_AT_SCOPE_EXIT(bus.FreeResources(gpu));

		Swap(_bus, bus);

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void ResidencyCoordinator::FreeResources(Gpu& gpu) {
		_imagesBySource.Clear();
		_meshesBySource.Clear();
	}

	// ---------------------------------------------------

	VkResult ResidencyCoordinator::Insert(Gpu& gpu, const MeshSource& source, bool andMakeResident) {
		const MeshSource::Dimensions dimensions(source.GetDimensions());

		Mesh mesh;
		ET_FAIL_UNLESS(mesh.BindResources(gpu, dimensions.vertexCount, dimensions.vertexCount));
		ET_AT_SCOPE_EXIT(mesh.FreeResources(gpu));

		if (andMakeResident) {
			ET_FAIL_UNLESS(MakeResident(mesh.GetIndices(), source));
			ET_FAIL_UNLESS(MakeResident(mesh.GetVertices(), source));
		}

		_meshesBySource.Emplace(eastl::addressof(source), eastl::move(mesh));

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	VkResult ResidencyCoordinator::Insert(Gpu& gpu, const ImageSource& source, bool andMakeResident) {
		const ImageSource::Dimensions dimensions(source.GetDimensions());

		ShaderImage image;
		ET_FAIL_UNLESS(image.BindResources(gpu, VK_FORMAT_R8G8B8A8_SRGB, VkExtent3D{ dimensions.widthInTexels, dimensions.heightInTexels, dimensions.depthInTexels }, 0u));
		ET_AT_SCOPE_EXIT(image.FreeResources(gpu));

		if (andMakeResident) {
			ET_FAIL_UNLESS(MakeResident(image, source));
		}

		_imagesBySource.Emplace(eastl::addressof(source), eastl::move(image));

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void ResidencyCoordinator::Erase(Gpu& gpu, const MeshSource& source) {
		const auto candidate(_meshesBySource.Find(eastl::addressof(source)));

		if (candidate != _meshesBySource.End()) {
			candidate->second.FreeResources(gpu);

			_meshesBySource.Erase(candidate);
		}
	}

	// ---------------------------------------------------

	void ResidencyCoordinator::Erase(Gpu& gpu, const ImageSource& source) {
		const auto candidate(_imagesBySource.Find(eastl::addressof(source)));

		if (candidate != _imagesBySource.End()) {
			candidate->second.FreeResources(gpu);

			_imagesBySource.Erase(candidate);
		}
	}

	// ---------------------------------------------------

	VkResult ResidencyCoordinator::MakeResident(VertexBuffer& target, const MeshSource& source) {
		const MeshSource::Dimensions dimensions(source.GetDimensions());

		for (VkDeviceSize offset(0u); )

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	VkResult ResidencyCoordinator::MakeResident(IndexBuffer& target, const MeshSource& source) {
		const MeshSource::Dimensions dimensions(source.GetDimensions());

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	VkResult ResidencyCoordinator::MakeResident(ShaderImage& target, const ImageSource& source) {
		const ImageSource::Dimensions dimensions(source.GetDimensions());

		for (uint16 mip(0u); mip < dimensions.mips; ++mip) {
			for (uint16 layer(0u); layer < dimensions.sliceCount; ++layer) {
				VkExtent3D mipDimensions{dimensions.widthInTexels >> mip, dimensions.depthInTexels >> mip, dimensions.depthInTexels >> mip};
				VkExtent3D offset{0u, 0u, 0u};

				ET_FAIL_UNLESS(_bus.TransferToGpu(target, offset, chunkSize, VkImageSubresourceLayers{
					VK_IMAGE_ASPECT_COLOR_BIT,
					mip,
					layer,
					/*layerCount =*/1u // Always load one layer at a time
				}));
			}
		}

		return VK_SUCCESS;
	}

}}} // namespace Eldritch2::Graphics::Vulkan
