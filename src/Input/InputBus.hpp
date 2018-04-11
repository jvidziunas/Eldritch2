/*==================================================================*\
  InputBus.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Input {
		class	InputDevice;
	}
}

namespace Eldritch2 {
namespace Input {

	class InputBus {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Disable copy construction.
		InputBus( const InputBus& ) = delete;
	//!	Constructs this @ref InputBus instance.
		InputBus();

	// ---------------------------------------------------

	public:
		InputDevice*	Find( uintptr deviceId );
	};

}	// namespace Input
}	// namespace Eldritch2