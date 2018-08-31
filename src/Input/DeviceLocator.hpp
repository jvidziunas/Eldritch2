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
#include <Input/InputDevice.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Input {

	class DeviceLocator {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref InputBus instance.
		DeviceLocator(const Mutex& mutex, ArrayList<InputDevice>& devices);
		//!	Constructs this @ref InputBus instance.
		DeviceLocator(const DeviceLocator&) = default;

		~DeviceLocator() = default;

		// ---------------------------------------------------

	public:
		bool TryAcquireDevice(DeviceId id, InputDevice::BindingMap bindingByCode, InputHandler& handler);

		void ReleaseDevice(DeviceId id);

		// ---------------------------------------------------

	public:
		size_t GetDeviceCount() const;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		DeviceLocator& operator=(const DeviceLocator&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		const Mutex*            _mutex;
		ArrayList<InputDevice>* _devices;
	};

}} // namespace Eldritch2::Input

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Input/DeviceLocator.inl>
//------------------------------------------------------------------//
