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
#include <Graphics/Vulkan/FrameTransferBus.hpp>
#include <Graphics/Vulkan/GpuResources.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scheduling {
	class JobExecutor;
}

namespace Graphics {
	namespace Vulkan {
		class Gpu;
	}

	class ImageSource;
	class MeshSource;
} // namespace Graphics
} // namespace Eldritch2

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	class ResourceBus {
		// - TYPE PUBLISHING ---------------------------------

	public:
		template <typename Source, typename Resource>
		using ResidentSet = HashMap<const Source*, Resource>;

		// ---

	public:
		enum : VkDeviceSize {
			HeapBlockSize = 256u * 1024u * 1024u, /* 256MB */
		};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		ResourceBus(const ResourceBus&) = delete;
		//!	Constructs this @ref ResidencyCoordinator instance.
		ResourceBus();

		~ResourceBus() = default;

		// ---------------------------------------------------

	public:
		const FrameTransferBus& GetTransferBus() const;
		FrameTransferBus&       GetTransferBus();

		// ---------------------------------------------------

	public:
		VkResult SubmitFrameIo(Scheduling::JobExecutor& executor, Gpu& gpu);

		// ---------------------------------------------------

	public:
		VkResult Insert(Gpu& gpu, const ImageSource& image, bool andMakeResident = true);
		VkResult Insert(Gpu& gpu, const MeshSource& mesh, bool andMakeResident = true);

		void Erase(Gpu& gpu, const ImageSource& image);
		void Erase(Gpu& gpu, const MeshSource& mesh);

		// ---------------------------------------------------

	public:
		VkResult BindResources(Gpu& gpu, VkDeviceSize transferBufferSize);

		void FreeResources(Gpu& gpu);

		// ---------------------------------------------------

	private:
		VkResult MakeResident(VertexBuffer& target, const MeshSource& source);
		VkResult MakeResident(IndexBuffer& target, const MeshSource& source);

		VkResult MakeResident(ShaderImage& target, const ImageSource& source);

		// ---------------------------------------------------

		//!	Disable copy assignment.
		ResourceBus& operator=(const ResourceBus&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		FrameTransferBus                      _bus;
		ResidentSet<MeshSource, Mesh>         _meshesBySource;
		ResidentSet<ImageSource, ShaderImage> _imagesBySource;
	};

}}} // namespace Eldritch2::Graphics::Vulkan

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/Vulkan/ResourceBus.inl>
//------------------------------------------------------------------//
