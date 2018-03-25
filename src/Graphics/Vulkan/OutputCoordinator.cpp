/*==================================================================*\
  OutputCoordinator.cpp
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
#include <Graphics/Vulkan/OutputCoordinator.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {

	CountedPointer<OutputWindow> OutputCoordinator::GetWindowByName( const Utf8Char* const name ) {
		return _presenter.GetWindowByName( name );
	}

}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2