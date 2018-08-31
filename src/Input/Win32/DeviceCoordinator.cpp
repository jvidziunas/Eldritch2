/*==================================================================*\
  DeviceCoordinator.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Input/Win32/DeviceCoordinator.hpp>
#include <Logging/Log.hpp>
//------------------------------------------------------------------//
#include <windows.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Input { namespace Win32 {

	using namespace ::Eldritch2::Logging;

	namespace {

		void ReadPacket(const RAWKEYBOARD& packet, const InputDevice& device) {
			UINT scanCode(packet.MakeCode);
			UINT key(packet.VKey);

			if (key == 0xFF) {
				//	Discard "fake keys" which are part of an escaped sequence.
				return;
			}

			if (key == VK_SHIFT) {
				//	Left-/right-hand SHIFT
				key = MapVirtualKeyW(scanCode, MAPVK_VSC_TO_VK_EX);
			} else if (key == VK_NUMLOCK) {
				//	Correct PAUSE/BREAK and NUM LOCK silliness, and set the extended bit.
				scanCode = MapVirtualKeyW(key, MAPVK_VK_TO_VSC) | 0x100;
			}

			/*	E0 and E1 are escape sequences used for certain special keys, such as PRINT and PAUSE/BREAK;
				 *	see http://www.win.tue.nl/~aeb/linux/kbd/scancodes-1.html */
			const bool hasE0((packet.Flags & RI_KEY_E0) != 0);

			if ((packet.Flags & RI_KEY_E1) != 0u) {
				/*	For escaped sequences, turn the virtual key into the correct scan code using MapVirtualKey.
					 *	However, MapVirtualKey is unable to map VK_PAUSE (this is a known bug), hence we map that by hand. */
				scanCode = (key == VK_PAUSE) ? 0x45 : MapVirtualKeyW(key, MAPVK_VK_TO_VSC);
			}

			switch (key) {
				//	Right-hand CONTROL/ALT have their E0 bit set.
			case VK_CONTROL: key = (hasE0 ? VK_RCONTROL : VK_LCONTROL); break;
			case VK_MENU:
				key = (hasE0 ? VK_RMENU : VK_LMENU);
				break;
				//	NUMPAD enter/separator has the E0 bit set.
			case VK_RETURN:
				if (hasE0)
					key = VK_SEPARATOR;
				break;
				/*	The standard INSERT, DELETE, HOME, END, PRIOR and NEXT keys will always have their e0 bit set,
				 *	but the corresponding keys on the NUMPAD will not. */
			case VK_INSERT:
				if (!hasE0)
					key = VK_NUMPAD0;
				break;
			case VK_DELETE:
				if (!hasE0)
					key = VK_DECIMAL;
				break;
			case VK_HOME:
				if (!hasE0)
					key = VK_NUMPAD7;
				break;
			case VK_END:
				if (!hasE0)
					key = VK_NUMPAD1;
				break;
			case VK_PRIOR:
				if (!hasE0)
					key = VK_NUMPAD9;
				break;
			case VK_NEXT:
				if (!hasE0)
					key = VK_NUMPAD3;
				break;
				/*	The standard arrow keys will always have their E0 bit set, but the corresponding keys on the
				 *	NUMPAD will not. */
			case VK_LEFT:
				if (!hasE0)
					key = VK_NUMPAD4;
				break;
			case VK_RIGHT:
				if (!hasE0)
					key = VK_NUMPAD6;
				break;
			case VK_UP:
				if (!hasE0)
					key = VK_NUMPAD8;
				break;
			case VK_DOWN:
				if (!hasE0)
					key = VK_NUMPAD2;
				break;
				//	NUMPAD 5 doesn't have its e0 bit set.
			case VK_CLEAR:
				if (!hasE0)
					key = VK_NUMPAD5;
				break;
			} // switch (virtualKey)

			device.Dispatch(ScanCode(key), packet.Flags & RI_KEY_BREAK ? 0x00 : 0xFF);
		}

		// ---------------------------------------------------

		void ReadPacket(const RAWMOUSE& packet, const InputDevice& device) {
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

			if (packet.usButtonFlags & RI_MOUSE_BUTTON_1_DOWN) {
				device.Dispatch(LeftButton, 1);
			}

			if (packet.usButtonFlags & RI_MOUSE_BUTTON_1_UP) {
				device.Dispatch(LeftButton, -1);
			}

			if (packet.usButtonFlags & RI_MOUSE_BUTTON_2_DOWN) {
				device.Dispatch(RightButton, 1);
			}

			if (packet.usButtonFlags & RI_MOUSE_BUTTON_2_UP) {
				device.Dispatch(RightButton, -1);
			}

			if (packet.usButtonFlags & RI_MOUSE_BUTTON_3_DOWN) {
				device.Dispatch(MiddleButton, 1);
			}

			if (packet.usButtonFlags & RI_MOUSE_BUTTON_3_UP) {
				device.Dispatch(MiddleButton, -1);
			}

			if (packet.usButtonFlags & RI_MOUSE_BUTTON_4_DOWN) {
				device.Dispatch(ExtendedButton0, 1);
			}

			if (packet.usButtonFlags & RI_MOUSE_BUTTON_4_UP) {
				device.Dispatch(ExtendedButton0, -1);
			}

			if (packet.usButtonFlags & RI_MOUSE_BUTTON_5_DOWN) {
				device.Dispatch(ExtendedButton1, 1);
			}

			if (packet.usButtonFlags & RI_MOUSE_BUTTON_5_UP) {
				device.Dispatch(ExtendedButton1, -1);
			}

			if (packet.usButtonFlags & RI_MOUSE_WHEEL) {
				device.Dispatch(Wheel, reinterpret_cast<const SHORT&>(packet.usButtonData));
			}

			device.Dispatch(HorizontalAxis, packet.lLastX);
			device.Dispatch(VerticalAxis, packet.lLastY);
		}

		// ---------------------------------------------------

		//	(6001) MSVC doesn't like the name query code and complains that the value is being used before being initialized. This is not the case, since GetRawInputDeviceInfo() writes to the provided buffer.
		//	(6054) MSVC complains about the name string potentially not having a null terminator; GetRawInputDeviceInfo writes the terminator for us.
		ET_PUSH_MSVC_WARNING_STATE(disable : 6001 6054)
		ETInlineHint bool ShouldIgnoreDevice(const RAWINPUTDEVICELIST& device) {
			wchar_t name[MAX_PATH];
			UINT    nameLength(ETCountOf(name));

			return device.dwType == RIM_TYPEHID                                                                                              /* We can only handle mice and keyboards with the current implementation. */
				|| GetRawInputDeviceInfoW(device.hDevice, RIDI_DEVICENAME, name, &nameLength) < 0                                            /* Aggregate devices do not have a name that can be queried. */
				|| CreateFileW(name, 0u, (FILE_SHARE_READ | FILE_SHARE_WRITE), nullptr, OPEN_EXISTING, 0u, nullptr) == INVALID_HANDLE_VALUE; /* Virtual devices cannot be opened for read with CreateFile(). */
		}
		ET_POP_MSVC_WARNING_STATE()

		// ---------------------------------------------------

		ETCpp14Constexpr ETInlineHint ETPureFunctionHint StringView GetDeviceTypeName(DWORD type) {
			switch (type) {
			case RIM_TYPEMOUSE: return "mouse";
			case RIM_TYPEKEYBOARD: return "keyboard";
			case RIM_TYPEHID: return "generic HID";
			default: return "<unknown>";
			};
		}

	} // anonymous namespace

	DeviceCoordinator::DeviceCoordinator(Log& log) :
		_log(log),
		_indexByHandle(MallocAllocator("Win32 Raw Input Device By Handle Map Allocator")),
		_keyboards(MallocAllocator("Win32 Raw Input Keyboard List Allocator")),
		_mice(MallocAllocator("Win32 Raw Input Mouse List Allocator")) {
	}

	// ---------------------------------------------------

	bool DeviceCoordinator::Enumerate() {
		_log.Write(Severity::Message, "Enumerating Win32 raw input devices." ET_NEWLINE);

		//	Ask Windows how many raw input devices it knows about.
		UINT count(0u);
		if (GetRawInputDeviceList(nullptr, &count, sizeof(RAWINPUTDEVICELIST)) == UINT(-1)) {
			_log.Write(Severity::Error, "Failed to enumerate Win32 input devices!" ET_NEWLINE);
			return false;
		}

		//	Fill a temporary array with more detailed information about each device, including its type and the Win32 handle.
		RAWINPUTDEVICELIST* const devices(ETStackAlloc(RAWINPUTDEVICELIST, count));
		if (GetRawInputDeviceList(devices, &count, sizeof(RAWINPUTDEVICELIST)) == UINT(-1)) {
			_log.Write(Severity::Error, "Failed to enumerate Win32 input devices!" ET_NEWLINE);
			return false;
		}

		ArrayMap<HANDLE, UINT> indexByHandle(_indexByHandle.GetAllocator(), LessThan<HANDLE>(), /*capacity=*/count);
		ArrayList<InputDevice> keyboards(_keyboards.GetAllocator());
		ArrayList<InputDevice> mice(_mice.GetAllocator());

		for (UINT id(0u); id < count; ++id) {
			if (ShouldIgnoreDevice(devices[id])) {
				_log.Write(Severity::VerboseWarning, "Ignoring unknown/virtual {} (handle: {})." ET_NEWLINE, GetDeviceTypeName(devices[id].dwType), devices[id].hDevice);
				continue;
			}

			UINT index;
			switch (devices[id].dwType) {
			case RIM_TYPEMOUSE:
				indexByHandle.Emplace(devices[id].hDevice, UINT(mice.GetSize()));
				mice.EmplaceBack();
				break;
			case RIM_TYPEKEYBOARD:
				indexByHandle.Emplace(devices[id].hDevice, UINT(keyboards.GetSize()));
				keyboards.EmplaceBack();
				break;
			default: continue;
			} // switch( event.header.dwType )

			_log.Write(Severity::Message, "Bound {} (handle: {})." ET_NEWLINE, GetDeviceTypeName(devices[id].dwType), devices[id].hDevice);
		}
		{
			Lock _(_deviceMutex);

			//	Publish the new set of devices.
			Swap(_indexByHandle, indexByHandle);
			Swap(_keyboards, keyboards);
			Swap(_mice, mice);
		} // End of lock scope.

		_log.Write(Severity::Message, "Win32 raw input enumeration complete." ET_NEWLINE);
		return true;
	}

	// ---------------------------------------------------

	void DeviceCoordinator::Route(const RAWINPUT& input) const {
		const ArrayMap<HANDLE, UINT>::ConstIterator candidate(_indexByHandle.Find(input.header.hDevice));
		if (ET_UNLIKELY(candidate == _indexByHandle.End())) {
			//	We may be receiving input notifications for virtual or other devices we do not have registered handlers for. Ignore incoming data in this case.
			return;
		}

		switch (input.header.dwType) {
		case RIM_TYPEMOUSE: return ReadPacket(input.data.mouse, _mice[candidate->second]);
		case RIM_TYPEKEYBOARD: return ReadPacket(input.data.keyboard, _keyboards[candidate->second]);
		} // switch(input.header.dwType)
	}

}}} // namespace Eldritch2::Input::Win32
