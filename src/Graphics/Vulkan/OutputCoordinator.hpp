/*==================================================================*\
  OutputCoordinator.hpp
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

	class OutputCoordinator {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Disable copy construction.
		OutputCoordinator( const OutputCoordinator& ) = delete;
	//!	Constructs this @ref OutputCoordinator instance.
		OutputCoordinator( PresentCoordinator& presenter );

		~OutputCoordinator() = default;

	// ---------------------------------------------------
	
	public:
		CountedPointer<OutputWindow>	GetWindowByName( const Utf8Char* const name );

	// - DATA MEMBERS ------------------------------------
	
	private:
		PresentCoordinator& _presenter;
	};

}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2

