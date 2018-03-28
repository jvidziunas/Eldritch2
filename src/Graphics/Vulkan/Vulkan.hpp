/*==================================================================*\
  Vulkan.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/ResidencyCoordinator.hpp>
#include <Graphics/Vulkan/PresentCoordinator.hpp>
#include <Graphics/Vulkan/HostMixin.hpp>
#include <Graphics/Vulkan/Gpu.hpp>
#include <Logging/ChildLog.hpp>
//------------------------------------------------------------------//
#include <vulkan/vulkan_core.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Scheduling {
		class	JobExecutor;
	}
}

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {

	class Vulkan {
	// - TYPE PUBLISHING ---------------------------------

	public:
		using HostAllocator = HostMixin<UsageMixin<MallocAllocator>>;

		enum : size_t { MaxGpus	= 4u };

	// ---

	public:
		class Device : public Gpu, public PresentCoordinator, public ResidencyCoordinator {
		// - TYPE PUBLISHING ---------------------------------

		public:
			enum : VkDeviceSize {
				TransferBufferSize =  128u * 1024u * 1024u, /* 128MB */
			};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
		//!	Disable copy construction.
			Device( const Device& ) = delete;
		//!	Constructs this @ref Device instance.
			Device() = default;

			~Device() = default;

		// ---------------------------------------------------

		public:
			VkResult	BindResources( Vulkan& vulkan, VkPhysicalDevice device, VkDeviceSize transferBufferSize );

			void		FreeResources( Vulkan& vulkan );
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref Vulkan instance.
		Vulkan( Logging::Log& log );
	//!	Disable copy construction.
		Vulkan( const Vulkan& ) = delete;

		~Vulkan();

	// ---------------------------------------------------

	public:
		Device&	GetPrimaryDevice();

		operator VkInstance();

	// ---------------------------------------------------

	public:
		template <size_t formatSize, typename... Arguments>
		void	WriteLog( Logging::MessageType type, const Utf8Char (&format)[formatSize], Arguments&&... arguments ) const;

	// ---------------------------------------------------

	public:
		void	BeginFrame( Scheduling::JobExecutor& executor );

	// ---------------------------------------------------

	public:
		VkResult	BindResources( Scheduling::JobExecutor& executor, const VkApplicationInfo& applicationInfo );

		void		FreeResources( Scheduling::JobExecutor& executor );

	// ---------------------------------------------------

	//!	Disable copy assignment.
		Vulkan&	operator=( const Vulkan& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		mutable HostAllocator		_allocator;
	//!	Mutable so logs may be written in const functions.
		mutable Logging::ChildLog	_log;

		VkInstance					_vulkan;
		VkDebugReportCallbackEXT	_debugCallback;

		uint32_t					_deviceCount;
		Device						_devices[MaxGpus];
	};

}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/Vulkan/Vulkan.inl>
//------------------------------------------------------------------//