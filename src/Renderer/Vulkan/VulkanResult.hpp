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
		@param[in] object Vulkan object that the @ref VulkanResult will assume responsibility for.
		@param[in] deleter Vulkan deleter that will handle cleanup when the Vulkan object leaves scope. */
		VulkanResult( Vulkan::UniquePointer<VulkanObject> object );
	//!	Constructs this @ref VulkanResult instance.
	/*!	Use this overload when object construction was not performed successfully.
		The constructor parameter will indicate to the outside function why construction failed.
		@param[in] result Vulkan result code containing information on why the operation failed. */
		VulkanResult( VkResult result );
	//!	Constructs this @ref VulkanResult instance.
		VulkanResult( VulkanResult&& );
	//!	Disable copying.
		VulkanResult( const VulkanResult& ) = delete;

		~VulkanResult() = default;

	// ---------------------------------------------------

	public:
		const Vulkan::UniquePointer<VulkanObject>&	operator*() const;
		Vulkan::UniquePointer<VulkanObject>&		operator*();

		const Vulkan::UniquePointer<VulkanObject>*	operator->() const;
		Vulkan::UniquePointer<VulkanObject>*		operator->();

		operator									VkResult() const;

		explicit operator							bool() const;

	// ---------------------------------------------------

	//!	Disable assignment.
		VulkanResult&	operator=( const VulkanResult& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		Vulkan::UniquePointer<VulkanObject>	_object;
		VkResult							_result;
	};

}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Renderer/Vulkan/VulkanResult.inl>
//------------------------------------------------------------------//
