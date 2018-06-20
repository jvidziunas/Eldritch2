/*==================================================================*\
  DeviceCoordinator.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Input/InputDevice.hpp>
#include <Logging/ChildLog.hpp>
//------------------------------------------------------------------//

using RAWINPUT = struct tagRAWINPUT;
using HANDLE   = void*;

namespace Eldritch2 { namespace Input { namespace Win32 {

	class DeviceCoordinator {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref DeviceSet instance.
		DeviceCoordinator(Logging::Log& log);
		//!	Disable copy construction.
		DeviceCoordinator(const DeviceCoordinator&) = delete;

		~DeviceCoordinator() = default;

		// ---------------------------------------------------

	public:
		//!	Send a single Win32 raw input event to the appropriate subscriber.
		void Route(const RAWINPUT& input) const;

		// ---------------------------------------------------

	public:
		ArrayList<InputDevice>& GetKeyboards();

		ArrayList<InputDevice>& GetMice();

		const Mutex& GetMutex() const;

		// ---------------------------------------------------

	public:
		//!	Polls the system for all attached raw input devices and creates dispatch structures to route input events to game systems.
		bool Enumerate();

		// ---------------------------------------------------

		//!	Disable copy assignment.
		DeviceCoordinator& operator=(const DeviceCoordinator&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		mutable Logging::ChildLog _log;
		mutable Mutex             _deviceMutex;
		ArrayMap<HANDLE, UINT>    _indexByHandle;
		ArrayList<InputDevice>    _keyboards;
		ArrayList<InputDevice>    _mice;
	};

}}} // namespace Eldritch2::Input::Win32

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Input/Win32/DeviceCoordinator.inl>
//------------------------------------------------------------------//
