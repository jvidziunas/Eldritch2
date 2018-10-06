/*==================================================================*\
  HostMixin.hpp
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

	template <class EldritchAllocator>
	class HostMixin : public EldritchAllocator {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref HostMixin instance.
		template <typename... Arguments, class = eastl::enable_if<eastl::is_constructible<EldritchAllocator, Arguments...>::value>::type>
		HostMixin(Arguments&&... arguments);
		//!	Disable copy construction.
		HostMixin(const HostMixin&) = delete;
		//!	Constructs this @ref HostMixin instance.
		HostMixin(HostMixin&&);

		~HostMixin() = default;

		// ---------------------------------------------------

	public:
		ETConstexpr const VkAllocationCallbacks* GetCallbacks() const ETNoexceptHint;

		ETConstexpr operator const VkAllocationCallbacks*() const ETNoexceptHint;

		// - DATA MEMBERS ------------------------------------

	private:
		VkAllocationCallbacks _callbacks;
	};

}}} // namespace Eldritch2::Graphics::Vulkan

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/Vulkan/HostMixin.inl>
//------------------------------------------------------------------//
