/*==================================================================*\
  ResourceBus.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/GpuResourceApi.hpp>
#include <Graphics/GpuAbi.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics {
	namespace Vulkan {
		class TransferCommandList;
		class Gpu;
	} // namespace Vulkan

	class ImageSource;
	class MeshSource;
}} // namespace Eldritch2::Graphics

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	class ResourceBus {
		// - TYPE PUBLISHING ---------------------------------

	public:
		template <typename Source, typename Resource>
		using ResidentSet      = HashMap<const Source*, Resource>;
		using ResidentImageSet = ResidentSet<ImageSource, ShaderImage>;
		using ResidentMeshSet  = ResidentSet<MeshSource, Mesh>;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		ResourceBus(const ResourceBus&) = delete;
		//!	Constructs this @ref ResourceBus instance.
		ResourceBus(ResourceBus&&) ETNoexceptHint;
		//!	Constructs this @ref ResourceBus instance.
		ResourceBus() ETNoexceptHint;

		~ResourceBus() = default;

		// ---------------------------------------------------

	public:
		const ShaderImage* Find(const ImageSource& image) const ETNoexceptHint;
		const Mesh*        Find(const MeshSource& mesh) const ETNoexceptHint;

		// ---------------------------------------------------

	public:
		VkResult Insert(Gpu& gpu, const ImageSource& image);
		VkResult Insert(Gpu& gpu, const MeshSource& mesh);

		void Erase(Gpu& gpu, const ImageSource& image);
		void Erase(Gpu& gpu, const MeshSource& mesh);

		// ---------------------------------------------------

	public:
		VkResult BindResources(Gpu& gpu, VkDeviceSize vertexCacheSize, VkDeviceSize indexCacheSize);

		void FreeResources(Gpu& gpu);

		// ---------------------------------------------------

		//!	Disable copy assignment.
		ResourceBus& operator=(const ResourceBus&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		GeometryCache      _vertexCache;
		ResidentMeshSet  _meshesBySource;
		ResidentImageSet _imagesBySource;

		// ---------------------------------------------------

		friend void Swap(ResourceBus&, ResourceBus&) ETNoexceptHint;
	};

}}} // namespace Eldritch2::Graphics::Vulkan

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/Vulkan/ResourceBus.inl>
//------------------------------------------------------------------//
