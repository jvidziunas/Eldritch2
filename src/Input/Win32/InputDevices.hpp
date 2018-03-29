/*==================================================================*\
  InputDevices.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Input/ScanCode.hpp>
//------------------------------------------------------------------//

struct	tagRAWINPUT;
using RAWINPUT	= tagRAWINPUT;

namespace Eldritch2 {
namespace Input {
namespace Win32 {

	class Mouse {
	// - TYPE PUBLISHING ---------------------------------

	public:
		enum : ScanCode {
			LeftButton,
			RightButton,
			MiddleButton,
			ExtendedButton0,
			ExtendedButton1,
			Wheel,
			HorizontalAxis,
			VerticalAxis
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref Mouse instance.
		Mouse( const Mouse& ) = default;
	//!	Constructs this @ref Mouse instance.
		Mouse() = default;

		~Mouse() = default;

	// ---------------------------------------------------

	public:
		void	ReadPacket( const RAWINPUT& packet );

	// ---------------------------------------------------

	public:
		Mouse&	operator=( Mouse&& ) ETNoexceptHint = default;

	// ---------------------------------------------------

	private:
		void	PressButton( ScanCode code, int16 weight ) const;

	// - DATA MEMBERS ------------------------------------

	private:
		mutable Mutex	_mutex;
		BindingMap<>	_bindingsByCode;
	};

// ---

	class Keyboard {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref Keyboard instance.
		Keyboard( const Keyboard& ) = default;
	//!	Constructs this @ref Keyboard instance.
		Keyboard() = default;

		~Keyboard() = default;

	// ---------------------------------------------------

	public:
		void	ReadPacket( const RAWINPUT& packet );

	// ---------------------------------------------------

	public:
		Keyboard&	operator=( Keyboard&& ) ETNoexceptHint = default;

	// ---------------------------------------------------

	private:
		void	PressButton( ScanCode code, int16 weight ) const;

	// - DATA MEMBERS ------------------------------------

	private:
		mutable Mutex	_mutex;
		BindingMap<>	_bindingsByCode;
	};

}	// namespace Win32
}	// namespace Input
}	// namespace Eldritch2