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
#include <Graphics/Vulkan/IoBuilder.hpp>
#include <Graphics/Vulkan/GpuHeap.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Scheduling {
		class	JobExecutor;
	}

	namespace Graphics {
		namespace Vulkan {
			class	Gpu;
		}
	}
}

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {

	class ResidencyCoordinator {
	// - TYPE PUBLISHING ---------------------------------

	public:
		template <typename Resource>
		struct ResourceHash {
			ETPureFunctionHint size_t	operator()( const Resource& resource, size_t seed = 0u ) const;
			template <typename Source>
			ETPureFunctionHint size_t	operator()( const Source* source, size_t seed = 0u ) const;
		};

	// ---

	public:
		template <typename Resource>
		struct ResourceEquals {
			ETPureFunctionHint bool	operator()( const Resource& resource, const Resource& resource1 ) const;
			template <typename Source>
			ETPureFunctionHint bool	operator()( const Resource& resource, const Source* source ) const;
		};

	// ---

	public:
		template <typename Resource>
		using ResidentSet	= HashSet<Resource, ResourceHash<Resource>, ResourceEquals<Resource>>;
		using HostAllocator = HostMixin<MallocAllocator>;

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

		~ResidencyCoordinator();

	// ---------------------------------------------------

	public:
		VkResult	SubmitFrameIo( Scheduling::JobExecutor& executor, Gpu& gpu );

	// ---------------------------------------------------

	public:
		VkResult	BindResources( Gpu& gpu, VkDeviceSize transferBufferSize );

		void		FreeResources( Gpu& gpu );

	// ---------------------------------------------------

	public:
		VkResult	Insert( const GeometrySource& geometry, bool andUpload = true, bool allowSwap = false );
		VkResult	Insert( const ImageSource& image, bool andUpload = true, bool allowSwap = false );

		void		Erase( const GeometrySource& geometry );
		void		Erase( const ImageSource& image );

	// ---------------------------------------------------

	//!	Disable copy assignment.
		ResidencyCoordinator&	operator=( const ResidencyCoordinator& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		mutable HostAllocator			_allocator;
		GpuHeap							_heap;
		IoBuilder						_ioBuilder;

		ResidentSet<SparseShaderImage>	_sparseShaderImages;
		ResidentSet<Geometry>			_geometry;
		ResidentSet<ShaderImage>		_shaderImages;
	};

}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/Vulkan/ResidencyCoordinator.inl>
//------------------------------------------------------------------//