/*==================================================================*\
  DisplayBus.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/PresentCoordinator.hpp>
#include <Graphics/Vulkan/DisplayBus.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {

	DisplayBus::DisplayBus( PresentCoordinator& presenter ) : _presenter( presenter ) {}

// ---------------------------------------------------

	CountedPointer<OutputWindow> DisplayBus::FindWindowByName( const Utf8Char* const name ) {
		return _presenter.FindWindowByName( name );
	}

}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2