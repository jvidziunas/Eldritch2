/*==================================================================*\
  DebugController.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/NonCopyable.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Utility {

	class DebugController : private Utility::Noncopyable {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref DebugController instance.
		DebugController() = default;

		//! Destroys this @ref DebugController instance.
		~DebugController() = default;

	// ---------------------------------------------------

		Utility::DebugController&	SetMemoryTrackingEnabled( bool enabled = true );

		Utility::DebugController&	SetLeakTrackingEnabled( bool enabled = true );
	};

}	// namespace Utility
}	// namespace Eldritch2