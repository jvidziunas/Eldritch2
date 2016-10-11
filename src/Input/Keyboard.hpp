/*==================================================================*\
  Keyboard.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Mpl/IntTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Input {

	class Keyboard {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref Keyboard instance.
		Keyboard() = default;

		~Keyboard() = default;

	// - DATA MEMBERS ------------------------------------

	protected:
		unsigned char	_keyStates[256u];
	};

}	// namespace Input
}	// namespace Eldritch2