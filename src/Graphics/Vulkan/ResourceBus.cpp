/*==================================================================*\
  ResourceBus.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/ResourceBus.hpp>
#include <Graphics/Vulkan/VulkanTools.hpp>
#include <Scheduling/JobExecutor.hpp>
#include <Graphics/ImageSource.hpp>
#include <Graphics/MeshSource.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	using namespace ::Eldritch2::Scheduling;

	ResourceBus::ResourceBus() :
		_meshesBySource(MallocAllocator("Geometry By Source Collection Allocator")),
		_imagesBySource(MallocAllocator("Image By Source Collection Allocator")) {
	}

	// ---------------------------------------------------

	VkResult ResourceBus::SubmitFrameIo(JobExecutor& executor, Gpu& gpu) {
		executor.AwaitCondition(_bus.CheckCommandsConsumed(gpu));

		return _bus.SubmitCommands(gpu);
	}

	// ---------------------------------------------------

	VkResult ResourceBus::BindResources(Gpu& gpu, VkDeviceSize transferBufferSize) {
		using Eldritch2::Swap;

		FrameTransferBus bus;
		ET_FAIL_UNLESS(bus.BindResources(gpu, transferBufferSize));
		ET_AT_SCOPE_EXIT(bus.FreeResources(gpu));

		Swap(_bus, bus);

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void ResourceBus::FreeResources(Gpu& gpu) {
		for (ResidentSet<ImageSource, ShaderImage>::ValueType& image : _imagesBySource) {
			image.second.FreeResources(gpu);
		}

		for (ResidentSet<MeshSource, Mesh>::ValueType& mesh : _meshesBySource) {
			mesh.second.FreeResources(gpu);
		}

		_imagesBySource.Clear();
		_meshesBySource.Clear();
	}

	// ---------------------------------------------------

	VkResult ResourceBus::Insert(Gpu& gpu, const MeshSource& source, bool andMakeResident) {
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

	VkResult ResourceBus::Insert(Gpu& gpu, const ImageSource& source, bool andMakeResident) {
		const ImageSource::SubimageDescription description(source.GetDescription(0));

		ShaderImage image;
		ET_FAIL_UNLESS(image.BindResources(gpu, VK_FORMAT_R8G8B8A8_SRGB, VkExtent3D { description.widthInTexels, description.heightInTexels, description.depthInTexels }, source.GetSliceCount()));
		ET_AT_SCOPE_EXIT(image.FreeResources(gpu));

		if (andMakeResident) {
			ET_FAIL_UNLESS(MakeResident(image, source));
		}

		_imagesBySource.Emplace(eastl::addressof(source), eastl::move(image));

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void ResourceBus::Erase(Gpu& gpu, const MeshSource& source) {
		const auto candidate(_meshesBySource.Find(eastl::addressof(source)));
		if (candidate != _meshesBySource.End()) {
			candidate->second.FreeResources(gpu);

			_meshesBySource.Erase(candidate);
		}
	}

	// ---------------------------------------------------

	void ResourceBus::Erase(Gpu& gpu, const ImageSource& source) {
		const auto candidate(_imagesBySource.Find(eastl::addressof(source)));
		if (candidate != _imagesBySource.End()) {
			candidate->second.FreeResources(gpu);

			_imagesBySource.Erase(candidate);
		}
	}

	// ---------------------------------------------------

	VkResult ResourceBus::MakeResident(VertexBuffer& target, const MeshSource& source) {
		const MeshSource::Dimensions dimensions(source.GetDimensions());
		for (VkDeviceSize offset(0u), chunkSize(4096u); offset < dimensions.vertexCount * sizeof(uint32); offset += chunkSize) {
			_bus.SendToGpu(target, offset, chunkSize);
		}

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	VkResult ResourceBus::MakeResident(IndexBuffer& target, const MeshSource& source) {
		const MeshSource::Dimensions dimensions(source.GetDimensions());
		for (VkDeviceSize offset(0u), chunkSize(4096u); offset < dimensions.indexCount * sizeof(uint32); offset += chunkSize) {
			_bus.SendToGpu(target, offset, chunkSize);
		}

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	VkResult ResourceBus::MakeResident(ShaderImage& target, const ImageSource& source) {
		for (uint32 mip(0u), mipCount(source.GetMipCount()); mip < mipCount; ++mip) {
			for (uint32 slice(0u), sliceCount(source.GetSliceCount()); slice < sliceCount; ++slice) {
				const ImageSource::SubimageDescription description(source.GetDescription(GetSubimageIndex(slice, mip, mipCount)));
				const VkExtent3D                       dimensions { description.widthInTexels, description.heightInTexels, description.depthInTexels };
				VkOffset3D                             offset { 0u, 0u, 0u };
				VkExtent3D                             chunkSize {};

				_bus.SendToGpu(target, offset, chunkSize, VkImageSubresourceLayers {
															  VK_IMAGE_ASPECT_COLOR_BIT,
															  mip,
															  slice,
														  });
			}
		}

		return VK_SUCCESS;
	}

}}} // namespace Eldritch2::Graphics::Vulkan
