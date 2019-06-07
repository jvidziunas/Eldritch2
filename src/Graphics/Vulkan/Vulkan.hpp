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
//------------------------------------------------------------------//
#include <vulkan/vulkan_core.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Logging {
	class Log;
}} // namespace Eldritch2::Logging

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	class Device : public Gpu, public ResourceBus, public DisplayBus {
		// - TYPE PUBLISHING ---------------------------------

	public:
		enum : VkDeviceSize {
			HeapBlockSize = 256u * 1024u * 1024u, /*256MB*/
		};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		Device(const Device&) = delete;
		//!	Constructs this @ref Device instance.
		Device() ETNoexceptHint = default;

		~Device() = default;

		// ---------------------------------------------------

	public:
		VkResult BindResources(VkInstance vulkan, VkPhysicalDevice device, VkDeviceSize heapBlockSize);

		void FreeResources();
	};

	// ---

	class Vulkan {
		// - TYPE PUBLISHING ---------------------------------

	public:
		using HostAllocator = HostMixin<UsageMixin<MallocAllocator>>;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		Vulkan(const Vulkan&) = delete;
		//!	Constructs this @ref Vulkan instance.
		Vulkan() ETNoexceptHint;

		~Vulkan();

		// ---------------------------------------------------

	public:
		ETConstexpr Device& GetDevice() ETNoexceptHint;

		ETConstexpr operator VkInstance() ETNoexceptHint;

		// ---------------------------------------------------

	public:
		VkResult BindResources(Logging::Log& log, uint32 apiVersion, Span<const char* const*> layers);

		void FreeResources();

		// ---------------------------------------------------

		//!	Disable copy assignment.
		Vulkan& operator=(const Vulkan&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		mutable HostAllocator    _allocator;
		VkInstance               _vulkan;
		VkDebugReportCallbackEXT _debugCallback;
		Device                   _device;
	};

}}} // namespace Eldritch2::Graphics::Vulkan

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/Vulkan/Vulkan.inl>
//------------------------------------------------------------------//
