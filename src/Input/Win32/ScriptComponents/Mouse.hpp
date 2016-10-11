/*==================================================================*\
  Mouse.hpp
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
#include <Input/Mouse.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Input {
namespace Win32 {
namespace ScriptComponents {

	class Mouse : public Input::Win32::RawInputSubscriber, public Input::Mouse {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref Mouse instance.
		Mouse( const Mouse& ) = default;
	//!	Constructs this @ref Mouse instance.
		Mouse() = default;

		~Mouse() = default;

	// ---------------------------------------------------

		void	ReadInputPacket( const RAWINPUT& inputPacket ) override;
	};

}	// namespace ScriptComponents
}	// namespace Win32
}	// namespace Input
}	// namespace Eldritch2