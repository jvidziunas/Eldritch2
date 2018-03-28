/*==================================================================*\
  DescriptorTable.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/DescriptorTable.hpp>
#include <Graphics/Vulkan/VulkanTools.hpp>
#include <Graphics/Vulkan/Gpu.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {

	DescriptorTable::DescriptorTable(
	) : _pool( nullptr ),
		_descriptors( nullptr ),
		_setLayout( nullptr ) {}

// ---------------------------------------------------

	DescriptorTable::DescriptorTable(
		DescriptorTable&& set
	) : _pool( eastl::exchange( set._pool, nullptr ) ),
		_descriptors( eastl::exchange( set._descriptors, nullptr ) ),
		_setLayout( eastl::exchange( set._setLayout, nullptr ) ) {
	}

// ---------------------------------------------------

	DescriptorTable::~DescriptorTable() {
		ET_ASSERT( _pool == nullptr,		"Leaking Vulkan descriptor pool!" );
		ET_ASSERT( _descriptors == nullptr,	"Leaking Vulkan descriptor set!"  );
		ET_ASSERT( _setLayout == nullptr,	"Leaking Vulkan pipeline layout!" );
	}

// ---------------------------------------------------

	void DescriptorTable::PushDescriptors( Gpu& gpu, uint32_t slot, uint32_t imageCount, const VkDescriptorImageInfo* images ) {
		const VkWriteDescriptorSet	write{
			VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
			nullptr,
			_descriptors,
			0u,
			slot,
			imageCount,
			VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
			images,
			nullptr,
			nullptr
		};

		vkUpdateDescriptorSets( gpu, 1u, &write, 0u, nullptr );
	}

// ---------------------------------------------------

	VkResult DescriptorTable::BindResources( Gpu& gpu, uint32_t imagePoolSize ) {
		using ::Eldritch2::Swap;

		const VkDescriptorPoolSize			poolSizes[] = { { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, imagePoolSize } };
		const VkDescriptorSetLayoutBinding	layoutBindings[] = { 
			{	0u,
				VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
				imagePoolSize,
				VK_SHADER_STAGE_ALL,
				nullptr
			}
		};
		const VkDescriptorSetLayoutCreateInfo	layoutInfo{
			VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
			nullptr,	// No extension data.
			0u,			// No create flags.
			static_cast<uint32_t>(_countof(layoutBindings)),
			layoutBindings
		};

		const VkDescriptorPoolCreateInfo	poolInfo{
			VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
			nullptr,					// No extension data.
			0u,							// No flags.
			1u,
			static_cast<uint32_t>(_countof(poolSizes)),
			poolSizes
		};

		VkDescriptorPool pool;
		ET_FAIL_UNLESS( vkCreateDescriptorPool( gpu, &poolInfo, gpu.GetAllocationCallbacks(), &pool ) );
		ET_AT_SCOPE_EXIT( vkDestroyDescriptorPool( gpu, pool, gpu.GetAllocationCallbacks() ) );

		VkDescriptorSetLayout layout;
		ET_FAIL_UNLESS( vkCreateDescriptorSetLayout( gpu, &layoutInfo, gpu.GetAllocationCallbacks(), &layout ) );
		ET_AT_SCOPE_EXIT( vkDestroyDescriptorSetLayout( gpu, layout, gpu.GetAllocationCallbacks() ) );

		Swap( _pool,		pool );
		Swap( _setLayout,	layout );

		return VK_SUCCESS;
	}

// ---------------------------------------------------

	void DescriptorTable::FreeResources( Gpu& gpu ) {
		vkDestroyDescriptorSetLayout( gpu, eastl::exchange( _setLayout, nullptr ), gpu.GetAllocationCallbacks() );
		vkDestroyDescriptorPool( gpu, eastl::exchange( _pool, nullptr ), gpu.GetAllocationCallbacks() );
	}

// ---------------------------------------------------

	void Swap( DescriptorTable& set0, DescriptorTable& set1 ) {
		using ::Eldritch2::Swap;

		Swap( set0._pool,			set1._pool );
		Swap( set0._descriptors,	set1._descriptors );
		Swap( set0._setLayout,		set1._setLayout );
	}

}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2