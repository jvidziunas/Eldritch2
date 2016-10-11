/*==================================================================*\
  Keyboard.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Input/Win32/RawInputSubscriber.hpp>
#include <Input/Keyboard.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Input {
namespace Win32 {
namespace ScriptComponents {

	class Keyboard : public Input::Win32::RawInputSubscriber, public Input::Keyboard {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref Keyboard instance.
		Keyboard( const Keyboard& ) = default;
	//!	Constructs this @ref Keyboard instance.
		Keyboard() = default;

		~Keyboard() = default;

	// ---------------------------------------------------

		void	ReadInputPacket( const RAWINPUT& inputPacket ) override sealed;
	};

}	// namespace ScriptComponents
}	// namespace Win32
}	// namespace Input
}	// namespace Eldritch2