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
#include <Graphics/Vulkan/ResourceBus.hpp>
#include <Graphics/Vulkan/DisplayBus.hpp>
#include <Graphics/Vulkan/HostMixin.hpp>
#include <Graphics/Vulkan/Gpu.hpp>
#include <Logging/ChildLog.hpp>
//------------------------------------------------------------------//
#include <vulkan/vulkan_core.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Scheduling {
	class JobExecutor;
}} // namespace Eldritch2::Scheduling

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	class Vulkan {
		// - TYPE PUBLISHING ---------------------------------

	public:
		class Device : public Gpu, public DisplayBus, public ResourceBus {
			// - TYPE PUBLISHING ---------------------------------

		public:
			enum : VkDeviceSize {
				HeapBlockSize      = 256u * 1024u * 1024u, /*256MB*/
				TransferBufferSize = 128u * 1024u * 1024u, /*128MB*/
			};

			// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Disable copy construction.
			Device(const Device&) = delete;
			//!	Constructs this @ref Device instance.
			Device() = default;

			~Device() = default;

			// ---------------------------------------------------

		public:
			void BeginFrame(Scheduling::JobExecutor& executor, Vulkan& vulkan);

			// ---------------------------------------------------

		public:
			VkResult BindResources(VkPhysicalDevice device, VkDeviceSize heapBlockSize = HeapBlockSize, VkDeviceSize transferBufferSize = TransferBufferSize);

			void FreeResources();
		};

		// ---

	public:
		using HostAllocator = HostMixin<UsageMixin<MallocAllocator>>;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref Vulkan instance.
		Vulkan(Logging::Log& log);
		//!	Disable copy construction.
		Vulkan(const Vulkan&) = delete;

		~Vulkan();

		// ---------------------------------------------------

	public:
		template <size_t formatSize, typename... Arguments>
		void WriteLog(Logging::MessageType type, const Utf8Char (&format)[formatSize], Arguments&&... arguments) const;

		// ---------------------------------------------------

	public:
		Device& GetPrimaryDevice();

		operator VkInstance();

		// ---------------------------------------------------

	public:
		void BeginFrame(Scheduling::JobExecutor& executor);

		// ---------------------------------------------------

	public:
		VkResult BindResources(Scheduling::JobExecutor& executor, const VkApplicationInfo& applicationInfo);

		void FreeResources(Scheduling::JobExecutor& executor);

		// ---------------------------------------------------

		//!	Disable copy assignment.
		Vulkan& operator=(const Vulkan&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		mutable HostAllocator     _allocator;
		mutable Logging::ChildLog _log;
		VkInstance                _vulkan;
		VkDebugReportCallbackEXT  _debugCallback;
		Device                    _device;
	};

}}} // namespace Eldritch2::Graphics::Vulkan

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/Vulkan/Vulkan.inl>
//------------------------------------------------------------------//
