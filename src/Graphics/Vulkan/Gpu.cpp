/*==================================================================*\
  Gpu.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/VulkanTools.hpp>
#include <Graphics/Vulkan/Gpu.hpp>
//------------------------------------------------------------------//
#include <eastl/sort.h>
//------------------------------------------------------------------//

//==================================================================//
// LIBRARIES
//==================================================================//
ET_LINK_LIBRARY( "vulkan-1.lib" )
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {
namespace {

	enum : VkBool32 {
		WaitAll = VK_TRUE
	};

// ---------------------------------------------------

	static const char* extensions[] = {
#	if VK_KHR_swapchain
		VK_KHR_SWAPCHAIN_EXTENSION_NAME,
#	endif
		nullptr
	};

// ---------------------------------------------------

	void FindQueueFamilies( VkPhysicalDevice device, uint32_t (&queues)[QueueConcept::COUNT] ) {
		uint32_t	familyCount( 0u );

	//	Determine the number of queue families exposed by the device.
		vkGetPhysicalDeviceQueueFamilyProperties( device, &familyCount, nullptr );

		const auto	families( static_cast<VkQueueFamilyProperties*>(_alloca( familyCount * sizeof(VkQueueFamilyProperties) )) );
		const auto	familiesEnd( families + familyCount );

		vkGetPhysicalDeviceQueueFamilyProperties( device, &familyCount, families );

		queues[QueueConcept::Drawing]			= FindQueueFamilyByFlags( families, familiesEnd, { static_cast<VkQueueFlagBits>(VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT) } );
		queues[QueueConcept::Presentation]		= FindQueueFamilyByFlags( families, familiesEnd, { VK_QUEUE_COMPUTE_BIT } );
		queues[QueueConcept::SparseBinding]		= FindQueueFamilyByFlags( families, familiesEnd, { VK_QUEUE_SPARSE_BINDING_BIT } );
		queues[QueueConcept::Transfer]			= FindQueueFamilyByFlags( families, familiesEnd, { static_cast<VkQueueFlagBits>(VK_QUEUE_SPARSE_BINDING_BIT | VK_QUEUE_TRANSFER_BIT), VK_QUEUE_TRANSFER_BIT } );
		queues[QueueConcept::BulkComputation]	= FindQueueFamilyByFlags( families, familiesEnd, { VK_QUEUE_COMPUTE_BIT } );
	}

// ---------------------------------------------------

	uint32_t BuildQueueCreateInfo( VkPhysicalDevice physicalDevice, VkDeviceQueueCreateInfo (&out)[QueueConcept::COUNT] ) {
		static const float	QueuePriorities[] = { 1.0f };
		uint32_t			indices[QueueConcept::COUNT];

		FindQueueFamilies( physicalDevice, indices );

		eastl::sort( eastl::begin( indices ), eastl::end( indices ) );
		const auto	lastUnique( eastl::unique( eastl::begin( indices ), eastl::end( indices ) ) );

		return static_cast<uint32_t>(eastl::distance( eastl::begin( out ), Transform( eastl::begin( indices ), lastUnique, out, [] ( uint32_t familyIndex ) {
			return VkDeviceQueueCreateInfo{
				VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
				nullptr,	// No extension data.
				0u,			// No create flags.
				familyIndex,
				_countof(QueuePriorities), QueuePriorities
			};
		} ) ));
	}

}	// anonymous namespace

	void Gpu::CommandQueue::BindResources( VkDevice device, uint32_t family ) {
		vkGetDeviceQueue( device, family, 0u, &queue );
		familyIndex = family;
	}

// ---------------------------------------------------

	Gpu::Gpu(
	) : _allocator( "Gpu Root Allocator" ),
		_physicalDevice( nullptr ),
		_device( nullptr ),
		_pipelineCache( nullptr ) {
	}

// ---------------------------------------------------

	Gpu::~Gpu() {
		ET_ASSERT( _device == nullptr,			"Leaking Vulkan device!" );
		ET_ASSERT( _pipelineCache == nullptr,	"Leaking Vulkan pipeline cache!" );
	}

// ---------------------------------------------------

	VkResult Gpu::BindResources( VkPhysicalDevice physicalDevice ) {
		using ::Eldritch2::Swap;

	//	Create the core Vulkan logical device.
		VkDevice	device;
		{	VkDeviceQueueCreateInfo	queueInfo[QueueConcept::COUNT];
			VkDeviceCreateInfo		createInfo;
			createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		//	No extension data.
			createInfo.pNext = nullptr;
		//	No create flags.
			createInfo.flags = 0u;				
			createInfo.queueCreateInfoCount		= BuildQueueCreateInfo( physicalDevice, queueInfo );
			createInfo.pQueueCreateInfos		= queueInfo;
			createInfo.enabledLayerCount		= 0u;
			createInfo.ppEnabledLayerNames		= nullptr;
			createInfo.enabledExtensionCount	= static_cast<uint32_t>(_countof(extensions) - 1u);
			createInfo.ppEnabledExtensionNames	= extensions;
			createInfo.pEnabledFeatures			= nullptr;
		
			ET_FAIL_UNLESS( vkCreateDevice( physicalDevice, &createInfo, _allocator, &device ) );
		}

		ET_AT_SCOPE_EXIT( vkDestroyDevice( device, _allocator ) );

	//	Create the Vulkan pipeline cache.
		VkPipelineCache pipelineCache;
		{	const VkPipelineCacheCreateInfo	createInfo{
				VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO,
				nullptr,	// extension data
				0u,
				0u,			// initial data size
				nullptr,	// initial data
			};

			ET_FAIL_UNLESS( vkCreatePipelineCache( device, &createInfo, _allocator, &pipelineCache ) );
		}

		ET_AT_SCOPE_EXIT( if (device) vkDestroyPipelineCache( device, pipelineCache, _allocator ); );

		uint32_t queueFamilies[QueueConcept::COUNT];
		FindQueueFamilies( physicalDevice, queueFamilies );

		for (uint32_t concept( 0u ); concept < _countof( _queues ); ++concept) {
			_queues[concept].BindResources( device, queueFamilies[concept] );
		}

	//	Commit resources to the object.
		Swap( _physicalDevice,	physicalDevice );
		Swap( _device,			device );
		Swap( _pipelineCache,	pipelineCache );

		return VK_SUCCESS;
	}

// ---------------------------------------------------

	void Gpu::FreeResources() {
	//	Ensure all rendering operations are complete before destroying the device.
		vkDeviceWaitIdle( _device );

		vkDestroyPipelineCache( _device, eastl::exchange( _pipelineCache, nullptr ), _allocator );
		vkDestroyDevice( eastl::exchange( _device, nullptr ), _allocator );

		_physicalDevice	= nullptr;
	}

}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2