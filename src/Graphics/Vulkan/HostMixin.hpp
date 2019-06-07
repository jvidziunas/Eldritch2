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
		template <typename... Arguments>
		HostMixin(Arguments&&... arguments) ETNoexceptHintIf(IsNoThrowConstructible<EldritchAllocator, Arguments...>());
		//!	Constructs this @ref HostMixin instance.
		HostMixin(const HostMixin&) = default;
		//!	Constructs this @ref HostMixin instance.
		HostMixin(HostMixin&&) = default;

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
