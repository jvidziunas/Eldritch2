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

	class InputBus {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref InputBus instance.
		InputBus(const Mutex& mutex, ArrayList<InputDevice>& devices);
		//!	Constructs this @ref InputBus instance.
		InputBus(const InputBus&) = default;

		~InputBus() = default;

		// ---------------------------------------------------

	public:
		bool TryAcquireDevice(DeviceId id, InputDevice::BindingMap<> bindingByCode, InputHandler& handler);

		void ReleaseDevice(DeviceId id);

		// ---------------------------------------------------

	public:
		size_t GetDeviceCount() const;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		InputBus& operator=(const InputBus&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		const Mutex*            _mutex;
		ArrayList<InputDevice>* _devices;
	};

}} // namespace Eldritch2::Input

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Input/InputBus.inl>
//------------------------------------------------------------------//
