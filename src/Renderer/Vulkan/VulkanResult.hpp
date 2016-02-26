/*==================================================================*\
  VulkanResult.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/Vulkan/SmartPointers.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {

	template <typename VulkanObject>
	class VulkanResult {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref VulkanResult instance.
		/*! Use this overload when object construction was performed successfully.
			@param[in] pointer Unique pointer that the @ref VulkanResult will assume responsibility for.
			*/
		ETInlineHint VulkanResult( Vulkan::UniquePointer<VulkanObject>&& pointer );
		//!	Constructs this @ref VulkanResult instance.
		/*!	Use this overload when object construction was not performed successfully.
			The constructor parameter will indicate to the outside function why construction failed.
			@param[in] result <Parameter Description>
			@returns <Return Value Description>
			*/
		ETInlineHint VulkanResult( const ::VkResult result );

		~VulkanResult() = default;

	// ---------------------------------------------------

		ETInlineHint operator bool() const;

	// - DATA MEMBERS ------------------------------------

		Vulkan::UniquePointer<VulkanObject>	object;
		::VkResult							result;
	};

}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Renderer/Vulkan/VulkanResult.inl>
//------------------------------------------------------------------//
