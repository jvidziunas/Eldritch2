/*==================================================================*\
  DescriptorTable.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <vulkan/vulkan_core.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {
	class Gpu;
}}} // namespace Eldritch2::Graphics::Vulkan

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	class DescriptorTable {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		DescriptorTable(const DescriptorTable&) = delete;
		//!	Constructs this @ref DescriptorTable instance.
		DescriptorTable(DescriptorTable&&);
		//!	Constructs this @ref DescriptorTable instance.
		DescriptorTable();

		~DescriptorTable();

		// ---------------------------------------------------

	public:
		VkDescriptorSet GetDescriptorSet() const;

		// ---------------------------------------------------

	public:
		void PushDescriptors(Gpu& gpu, uint32_t slot, uint32_t count, const VkDescriptorImageInfo* images);
		template <uint32_t count>
		void PushDescriptors(Gpu& gpu, uint32_t slot, const VkDescriptorImageInfo (&images)[count]);

		// ---------------------------------------------------

	public:
		VkResult BindResources(Gpu& gpu, VkDescriptorSetLayout layout, uint32_t poolCount, const VkDescriptorPoolSize* pools);

		void FreeResources(Gpu& gpu);

		// ---------------------------------------------------

	public:
		//!	Disable copy assignment.
		DescriptorTable& operator=(const DescriptorTable&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		VkDescriptorPool _pool;
		VkDescriptorSet  _descriptors;

		// ---------------------------------------------------

		friend void Swap(DescriptorTable&, DescriptorTable&);
	};

}}} // namespace Eldritch2::Graphics::Vulkan

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/Vulkan/DescriptorTable.inl>
//------------------------------------------------------------------//
