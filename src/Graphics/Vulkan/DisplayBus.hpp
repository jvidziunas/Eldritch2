/*==================================================================*\
  DisplayBus.hpp
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

namespace Eldritch2 {
	namespace Graphics {
		namespace Vulkan {
			class	PresentCoordinator;
			class	OutputWindow;
		}
	}
}

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {

	class DisplayBus {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref DisplayBus instance.
		DisplayBus( PresentCoordinator& presenter );
	//!	Disable copy construction.
		DisplayBus( const DisplayBus& ) = delete;

		~DisplayBus() = default;

	// ---------------------------------------------------
	
	public:
		CountedPointer<OutputWindow>	FindWindowByName( const Utf8Char* const name );

	// - DATA MEMBERS ------------------------------------
	
	private:
		PresentCoordinator& _presenter;
	};

}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2