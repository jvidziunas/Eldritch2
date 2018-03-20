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
#include <Input/Win32/InputDevices.hpp>
#include <Core/EngineComponent.hpp>
#include <Scheduling/Thread.hpp>
#include <Logging/ChildLog.hpp>
//------------------------------------------------------------------//

struct HHOOK__;
struct HWND__;

using HANDLE	= void*;
using HHOOK		= HHOOK__*;
using HWND		= HWND__*;

namespace Eldritch2 {
namespace Input {
namespace Win32 {

	class Win32InputEngineComponent : public Core::EngineComponent {
	// - TYPE PUBLISHING ---------------------------------

	private:
		class SamplingThread : public Scheduling::Thread {
		// - TYPE PUBLISHING ---------------------------------

		public:
			template <typename Value>
			using DeviceMap = ArrayMap<HANDLE, Value>;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
		//!	Disable copy construction.
			SamplingThread( const SamplingThread& ) = delete;
		//!	Constructs this @ref SamplingThread instance.
			SamplingThread( Logging::Log& log );

			~SamplingThread() = default;

		// ---------------------------------------------------
			
		public:
			Utf8Literal	GetName() const override sealed;

		// ---------------------------------------------------

		public:
			void	ScanDevices();

		// ---------------------------------------------------

		public:
			void	SetShouldShutDown() override;

			void	Run() override sealed;

		// ---------------------------------------------------

		//!	Disable copy assignment.
			SamplingThread&	operator=( const SamplingThread& ) = delete;

		// - DATA MEMBERS ------------------------------------

		private:
		//!	Mutable so logs may be written in const methods.
			mutable Logging::ChildLog	_log;
		/*!	Win32 raw input requires a window for input registration. The handle is exposed primarily for the
		 *	engine component to issue a shutdown request to the sampling thread when the application terminates,
		 *	as there is little need to push state to the window. */
			std::atomic<HWND>			_window;

			DeviceMap<Keyboard>			_keyboardsByHandle;
			DeviceMap<Mouse>			_miceByHandle;
		};

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

	//!	Disable copy assignment.
		Win32InputEngineComponent&	operator=( const Win32InputEngineComponent& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
	//!	Mutable so logs may be written in const methods.
		mutable Logging::ChildLog	_log;
		const HHOOK					_keyboardHook;
		SamplingThread				_inputSampler;
	};

}	// namespace Win32
}	// namespace Input
}	// namespace Eldritch2