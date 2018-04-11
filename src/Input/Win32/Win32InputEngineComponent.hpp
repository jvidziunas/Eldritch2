/*==================================================================*\
  Win32InputEngineComponent.hpp
  ------------------------------------------------------------------
  Purpose:
  Defines a Win32 application decorator object that invokes input
  system event objects when the user presses the corresponding key,
  moves a stick or shifts the mouse.

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Core/EngineComponent.hpp>
#include <Scheduling/Thread.hpp>
#include <Input/InputDevice.hpp>
#include <Logging/ChildLog.hpp>
//------------------------------------------------------------------//

using HANDLE	= void*;
using HHOOK		= struct HHOOK__*;
using HWND		= struct HWND__*;

namespace Eldritch2 {
namespace Input {
namespace Win32 {

	class Win32InputEngineComponent : public Core::EngineComponent {
	// - TYPE PUBLISHING ---------------------------------

	private:
		class ReaderThread : public Scheduling::Thread {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
		//!	Constructs this @ref ReaderThread instance.
			ReaderThread( Win32InputEngineComponent& owner );
		//!	Disable copy construction.
			ReaderThread( const ReaderThread& ) = delete;

			~ReaderThread() = default;

		// ---------------------------------------------------
			
		public:
			Utf8Literal	GetName() const override sealed;

		// ---------------------------------------------------

		public:
			void	SetShouldShutDown() override;

			void	Run() override sealed;

		// ---------------------------------------------------

		//!	Disable copy assignment.
			ReaderThread&	operator=( const ReaderThread& ) = delete;

		// - DATA MEMBERS ------------------------------------

		private:
			Win32InputEngineComponent&	_owner;
		/*!	Win32 raw input requires a window for input registration. The handle is exposed primarily for the
		 *	engine component to issue a shutdown request to the sampling thread when the application terminates,
		 *	as there is little need to push state to the window. */
			Atomic<HWND>				_window;
		};

	// ---

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref Win32InputEngineComponent instance.
		Win32InputEngineComponent( const Blackboard& services, Logging::Log& log );
	//!	Disable copy construction.
		Win32InputEngineComponent( const Win32InputEngineComponent& ) = delete;

		~Win32InputEngineComponent();

	// - ENGINE SERVICE SANDBOX METHODS ------------------

	protected:
		Result<UniquePointer<Core::WorldComponent>>	CreateWorldComponent( Allocator& allocator, const Core::World& world ) override;

		void										AcceptVisitor( Scheduling::JobExecutor& executor, const InitializationVisitor ) override;

	// ---------------------------------------------------

	protected:
	//!	Polls the system for all attached raw input devices and creates dispatch structures for routing input events to game systems.
		void	ScanDevices();

	//!	Broadcasts a single Win32 raw input event to the appropriate subscriber.
		void	Dispatch( const RAWINPUT& event ) const;

	// ---------------------------------------------------

	//!	Disable copy assignment.
		Win32InputEngineComponent&	operator=( const Win32InputEngineComponent& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
	//!	Mutable so logs may be written in const methods.
		mutable Logging::ChildLog		_log;
		const HHOOK						_keyboardHook;

		ReaderThread					_inputReader;
		ArrayMap<HANDLE, InputDevice>	_devicesByHandle;
		ArrayList<InputDevice*>			_mice;
		ArrayList<InputDevice*>			_keyboards;
	};

}	// namespace Win32
}	// namespace Input
}	// namespace Eldritch2