/*==================================================================*\
  ResourceBus.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/ResourceBus.hpp>
#include <Graphics/Vulkan/VulkanTools.hpp>
#include <Graphics/Vulkan/CommandList.hpp>
#include <Graphics/ImageSource.hpp>
#include <Graphics/MeshSource.hpp>
#include <Graphics/Vulkan/Gpu.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	ResourceBus::ResourceBus(ResourceBus&& bus) ETNoexceptHint : ResourceBus() {
		Swap(*this, bus);
	}

	// ---------------------------------------------------

	ResourceBus::ResourceBus() ETNoexceptHint : _meshesBySource(ResidentMeshSet::AllocatorType("Resident Mesh Set Allocator")), _imagesBySource(ResidentImageSet::AllocatorType("Resident Image Set Allocator")) {}

	// ---------------------------------------------------

	VkResult ResourceBus::BindResources(Gpu& gpu, VkDeviceSize vertexCacheByteSize, VkDeviceSize indexCacheByteSize) {
		using Eldritch2::Swap;

		GeometryCache vertexCache;
		ET_ABORT_UNLESS(vertexCache.BindResources(gpu, vertexCacheByteSize, indexCacheByteSize));
		ET_AT_SCOPE_EXIT(vertexCache.FreeResources(gpu));
		ResidentMeshSet  meshesBySource(_meshesBySource.GetAllocator());
		ResidentImageSet imagesBySource(_imagesBySource.GetAllocator());

		Swap(_vertexCache, vertexCache);
		Swap(_imagesBySource, imagesBySource);
		Swap(_meshesBySource, meshesBySource);

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void ResourceBus::FreeResources(Gpu& gpu) {
		_imagesBySource.ClearAndDispose([&gpu](ShaderImage& image) ETNoexceptHint { image.FreeResources(gpu); });
		_meshesBySource.ClearAndDispose([this](Mesh& mesh) { mesh.FreeResources(_vertexCache); });

		_vertexCache.FreeResources(gpu);
	}

	// ---------------------------------------------------

	VkResult ResourceBus::Insert(Gpu& gpu, const ImageSource& source) {
		ShaderImage image;
		ET_ABORT_UNLESS(image.BindResources(gpu, source.DescribeSelf()));
		ET_AT_SCOPE_EXIT(image.FreeResources(gpu));

		_imagesBySource.Emplace(ETAddressOf(source), Move(image));
		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	VkResult ResourceBus::Insert(Gpu& /*gpu*/, const MeshSource& source) {
		Mesh mesh;
		ET_ABORT_UNLESS(mesh.BindResources(_vertexCache, source.DescribeSelf()));
		ET_AT_SCOPE_EXIT(mesh.FreeResources(_vertexCache));

		_meshesBySource.Emplace(ETAddressOf(source), Move(mesh));
		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void ResourceBus::Erase(Gpu& gpu, const ImageSource& source) {
		const auto image(_imagesBySource.Find(ETAddressOf(source)));
		if (ET_UNLIKELY(image == _imagesBySource.End())) {
			return;
		}

		image->second.FreeResources(gpu);
		_imagesBySource.Erase(image);
	}

	// ---------------------------------------------------

	void ResourceBus::Erase(Gpu& /*gpu*/, const MeshSource& source) {
		const auto mesh(_meshesBySource.Find(ETAddressOf(source)));
		if (ET_UNLIKELY(mesh == _meshesBySource.End())) {
			return;
		}

		mesh->second.FreeResources(_vertexCache);
		_meshesBySource.Erase(mesh);
	}

	// ---------------------------------------------------

	void Swap(ResourceBus& lhs, ResourceBus& rhs) ETNoexceptHint {
		using ::Eldritch2::Swap;

		Swap(lhs._vertexCache, rhs._vertexCache);
		Swap(lhs._imagesBySource, rhs._imagesBySource);
		Swap(lhs._meshesBySource, rhs._meshesBySource);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
