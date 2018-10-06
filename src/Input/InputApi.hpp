/*==================================================================*\
  InputDevice.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 { namespace Input {
	class InputDevice;
}} // namespace Eldritch2::Input

namespace Eldritch2 { namespace Input {

	using ActionFlags = uint32;
	using DeviceId    = uintptr;
	using ActionId    = uint8;
	using ScanCode    = uint8;

	// ---

	/*	TODO: Packing of these structures when indexed by scan code is very wasteful, approx. 66% used.
	 *	Consider:
	 *	[ 0: 8] Scan code for key
	 *	[ 9:15] PADDING to round up to 16-bit boundary
	 *	 ----- KEY ^^ --- vv VALUE -----
	 *	[16:31] Weight
	 *	[32:39] Action index
	 *	[40:47] PADDING to round up to 16-bit boundary */
	struct Binding {
		int16    weight;
		ActionId action;
	};

	// ---

	class ETPureAbstractHint InputClient {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref InputClient instance.
		InputClient(const InputClient&) ETNoexceptHint = default;
		//!	Constructs this @ref InputClient instance.
		InputClient() ETNoexceptHint = default;

		~InputClient() = default;

		// ---------------------------------------------------

	public:
		virtual void Activate(ActionId id, int32 amount) abstract;

		virtual void OnConnect(const InputDevice& device);

		virtual void OnDisconnect(const InputDevice& device);
	};

	// ---

	class InputDevice {
		// - TYPE PUBLISHING ---------------------------------

	public:
		using BindingMap = ArrayMap<ScanCode, Binding>;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		InputDevice(const InputDevice&) = delete;
		//!	Constructs this @ref InputDevice instance.
		InputDevice(InputDevice&&) = default;
		//!	Constructs this @ref InputDevice instance.
		InputDevice();

		~InputDevice();

		// ---------------------------------------------------

	public:
		void Dispatch(ScanCode code, int32 weight) const;

		// ---------------------------------------------------

	public:
		bool TryAcquire(InputClient& handler, BindingMap actionByScanCode);

		void Release() ETNoexceptHint;

		// ---------------------------------------------------

	public:
		InputDevice& operator=(const InputDevice&) = delete;
		InputDevice& operator=(InputDevice&&) = default;

		// - DATA MEMBERS ------------------------------------

	private:
		Mutex        _mutex;
		InputClient* _handler;
		BindingMap   _actionByScanCode;

		// ---------------------------------------------------

		friend void Swap(InputDevice&, InputDevice&) = delete;
	};

	// ---

	struct DeviceDisposer {
		void operator()(InputDevice* device) const ETNoexceptHint;
	};

}} // namespace Eldritch2::Input

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Input/InputApi.inl>
//------------------------------------------------------------------//
