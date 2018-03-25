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

namespace Eldritch2 {
	namespace Graphics {
		namespace Vulkan {
			class	Gpu;
		}
	}
}

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {

	class DescriptorTable {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Disable copy construction.
		DescriptorTable( const DescriptorTable& ) = delete;
	//!	Constructs this @ref DescriptorTable instance.
		DescriptorTable( DescriptorTable&& );
	//!	Constructs this @ref DescriptorTable instance.
		DescriptorTable();

		~DescriptorTable();

	// ---------------------------------------------------

	public:
		void	PushDescriptors( Gpu& gpu, uint32_t slot, uint32_t imageCount, const VkDescriptorImageInfo* images );
		template <uint32_t imageCount>
		void	PushDescriptors( Gpu& gpu, uint32_t slot, const VkDescriptorImageInfo (&images)[imageCount] );

	// ---------------------------------------------------

	public:
		VkResult	BindResources( Gpu& gpu, uint32_t poolSize );

		void		FreeResources( Gpu& gpu );

	// ---------------------------------------------------

	public:
	//!	Disable copy assignment.
		DescriptorTable&	operator=( const DescriptorTable& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		VkDescriptorPool		_pool;
		VkDescriptorSet			_descriptors;
		VkDescriptorSetLayout	_setLayout;

	// ---------------------------------------------------

		friend void	Swap( DescriptorTable&, DescriptorTable& );
	};

}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/Vulkan/DescriptorTable.inl>
//------------------------------------------------------------------//