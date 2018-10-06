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
	class GpuImage;
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
		ETConstexpr auto GetDescriptorSetLayouts() const ETNoexceptHint -> const VkDescriptorSetLayout (&)[1];

		ETConstexpr auto GetDescriptorSets() const ETNoexceptHint -> const VkDescriptorSet (&)[1];

		// ---------------------------------------------------

	public:
		template <typename InputIterator>
		void PushShaderResources(uint32_t& outSlot, InputIterator begin, InputIterator end);

		void PushToGpu(Gpu& gpu);

		// ---------------------------------------------------

	public:
		VkResult BindResources(Gpu& gpu, std::initializer_list<VkDescriptorPoolSize> pools, std::initializer_list<VkDescriptorSetLayoutBinding> bindings);

		void FreeResources(Gpu& gpu);

		// ---------------------------------------------------

	public:
		//!	Disable copy assignment.
		DescriptorTable& operator=(const DescriptorTable&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		uint32                                    _lastCurrent;
		SoArrayList<const GpuImage*, VkImageView> _resources;
		VkDescriptorPool                          _pool;
		VkDescriptorSetLayout                     _setLayouts[1];
		VkDescriptorSet                           _sets[1];

		// ---------------------------------------------------

		friend void Swap(DescriptorTable&, DescriptorTable&);
	};

}}} // namespace Eldritch2::Graphics::Vulkan

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/Vulkan/DescriptorTable.inl>
//------------------------------------------------------------------//
