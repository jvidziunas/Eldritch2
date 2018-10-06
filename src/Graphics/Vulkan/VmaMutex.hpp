/*==================================================================*\
  VmaMutex.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	class VmaMutex : public Mutex {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref VmaMutex instance.
		VmaMutex(const VmaMutex&) ETNoexceptHint = default;
		//!	Constructs this @ref VmaMutex instance.
		VmaMutex() ETNoexceptHint = default;

		~VmaMutex() = default;

		// ---------------------------------------------------

	public:
		void Lock() ETNoexceptHint;

		void Unlock() ETNoexceptHint;
	};

}}} // namespace Eldritch2::Graphics::Vulkan

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/Vulkan/VmaMutex.inl>
//------------------------------------------------------------------//
