/*==================================================================*\
  ResidencyCoordinator.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/GpuResources.hpp>
#include <Graphics/Vulkan/HostMixin.hpp>
#include <Graphics/Vulkan/GpuHeap.hpp>
#include <Graphics/Vulkan/GpuBus.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Scheduling {
		class	JobExecutor;
	}

	namespace Graphics {
		namespace Vulkan {
			class	Gpu;
		}

		class	ImageSource;
		class	MeshSource;
	}
}

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {

	struct Mesh {
	public:
		void FreeResources( GpuHeap& heap ) {
			_vertices.FreeResources( heap );
			_indices.FreeResources( heap );
		}

		VertexBuffer	_vertices;
		IndexBuffer		_indices;
	};

	class ResidencyCoordinator {
	// - TYPE PUBLISHING ---------------------------------

	public:
		using HostAllocator = HostMixin<MallocAllocator>;
		template <typename Source, typename Resource>
		using ResidentSet	= HashMap<const Source*, Resource>;

	// ---

	public:
		enum : VkDeviceSize {
			GpuHeapBlockSize	= 256u * 1024u * 1024u,	/* 256MB */
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Disable copy construction.
		ResidencyCoordinator( const ResidencyCoordinator& ) = delete;
	//!	Constructs this @ref ResidencyCoordinator instance.
		ResidencyCoordinator();

		~ResidencyCoordinator() = default;

	// ---------------------------------------------------

	public:
		VkResult	SubmitFrameIo( Scheduling::JobExecutor& executor, Gpu& gpu );

	// ---------------------------------------------------

	public:
		VkResult	Insert( const ImageSource& image, bool andMakeResident = true );
		VkResult	Insert( const MeshSource& mesh, bool andMakeResident = true );

		void		Erase( const ImageSource& image );
		void		Erase( const MeshSource& mesh );

	// ---------------------------------------------------

	public:
		VkResult	BindResources( Gpu& gpu, VkDeviceSize transferBufferSize );

		void		FreeResources( Gpu& gpu );

	// ---------------------------------------------------

	private:
		VkResult	MakeResident( VertexBuffer& target, const MeshSource& source );
		VkResult	MakeResident( IndexBuffer& target, const MeshSource& source );

		VkResult	MakeResident( ShaderImage& target, const ImageSource& source );

	// ---------------------------------------------------

	//!	Disable copy assignment.
		ResidencyCoordinator&	operator=( const ResidencyCoordinator& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		mutable HostAllocator					_allocator;
		GpuHeap									_heap;
		GpuBus									_bus;

		ResidentSet<MeshSource, Mesh>			_meshesBySource;
		ResidentSet<ImageSource, ShaderImage>	_imagesBySource;
	};

}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2