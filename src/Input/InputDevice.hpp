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

	using DeviceId = uintptr;
	using ActionId = uint8;
	using ScanCode = uint8;

	// ---

	ET_PUSH_COMPILER_WARNING_STATE()
	ET_SET_MSVC_WARNING_STATE(disable : 4369 4309)

	enum : ActionId { InvalidActionId = ~0u };

	ET_POP_COMPILER_WARNING_STATE()

	// ---

	class ETPureAbstractHint InputHandler {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref InputHandler instance.
		InputHandler(const InputHandler&) = default;
		//!	Constructs this @ref InputHandler instance.
		InputHandler() = default;

		~InputHandler() = default;

		// ---------------------------------------------------

	public:
		virtual void Activate(ActionId id, int32 amount) abstract;

		virtual void OnConnect();

		virtual void OnDisconnect();
	};

	// ---

	class InputDevice {
		// - TYPE PUBLISHING ---------------------------------

	public:
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
			ActionId actionId;
		};

		// ---

	public:
		template <class Allocator = MallocAllocator>
		using BindingMap = ArrayMap<ScanCode, Binding, LessThan<ScanCode>, Allocator>;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
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
		bool TryAcquire(BindingMap<> bindingByScanCode, InputHandler& handler);

		void Release();

		// ---------------------------------------------------

	public:
		InputDevice& operator=(InputDevice&&) = default;

		// - DATA MEMBERS ------------------------------------

	private:
		BindingMap<>  _bindingByScanCode;
		InputHandler* _handler;
	};

}} // namespace Eldritch2::Input
