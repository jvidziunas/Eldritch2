/*==================================================================*\
  Win32InputService.hpp
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
#include <Utility/Containers/FlatOrderedMap.hpp>
#include <Foundation/GameEngineService.hpp>
#include <Utility/Concurrency/Lock.hpp>
#include <Scheduler/Thread.hpp>
#include <Input/Keyboard.hpp>
#include <Input/Mouse.hpp>
//------------------------------------------------------------------//
#ifndef WIN32_LEAN_AND_MEAN
#	define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Input {

	namespace Foundation	= ::Eldritch2::Foundation;
	namespace Input			= ::Eldritch2::Input;

// ---------------------------------------------------

	class Win32InputService : public Foundation::GameEngineService {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		// Constructs this Win32InputService instance.
		Win32InputService( Foundation::GameEngine& owningEngine );

		// Destroys this Win32InputService instance.
		~Win32InputService();

	// ---------------------------------------------------

		const ::Eldritch2::UTF8Char* const	GetName() const override sealed;

	// ---------------------------------------------------

	protected:
		void	AcceptInitializationVisitor( Scripting::ScriptAPIRegistrationInitializationVisitor& visitor ) override sealed;

		void	AcceptTaskVisitor( ::Eldritch2::Allocator& subtaskAllocator, Scheduler::Task& visitingTask, Scheduler::WorkerContext& executingContext, const PreConfigurationLoadedTaskVisitor ) override sealed;

	// - TYPE PUBLISHING ---------------------------------

	private:
		class RawInputPollingThread : public Scheduler::Thread {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			// Constructs this RawInputPollingThread instance.
			RawInputPollingThread( Input::Win32InputService& hostingService );

			// Destroys this RawInputPollingThread instance.
			~RawInputPollingThread();

		// ---------------------------------------------------

			// Retrieves a pointer to a null-terminated character array containing the UTF8-encoded name of this thread.
			const ::Eldritch2::UTF8Char* const	GetHumanReadableName() const override sealed;

		// ---------------------------------------------------

			::Eldritch2::ErrorCode	Run() override sealed;

			void					RequestGracefulShutdown() override;

		// - DATA MEMBERS ------------------------------------

		private:
			Input::Win32InputService&	_hostingInputService;
			::DWORD						_threadID;
		};

	// ---

		class ETPureAbstractHint RawInputSubscriber {
		public:
			virtual ~RawInputSubscriber() {}

		// ---------------------------------------------------

			virtual void	ReadInputPacket( const ::RAWINPUT& inputPacket ) abstract;
		};

	// ---

		class Keyboard : public RawInputSubscriber, public Input::Keyboard {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			// Constructs this Keyboard instance.
			Keyboard( const ::HANDLE deviceHandle, Input::Win32InputService& owner );

		// ---------------------------------------------------

			void	Dispose() override sealed;

		// ---------------------------------------------------

			void	ReadInputPacket( const ::RAWINPUT& inputPacket ) override sealed;
		};

	// ---

		class Mouse : public RawInputSubscriber, public Input::Mouse {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			// Constructs this Mouse instance.
			Mouse( const ::HANDLE deviceHandle, Input::Win32InputService& owner );

		// ---------------------------------------------------

			void	Dispose() override sealed;

		// ---------------------------------------------------

			void	ReadInputPacket( const ::RAWINPUT& inputPacket ) override;
		};

	// ---------------------------------------------------

		void	EnumerateAvailableRawInputDevices();

	// ---------------------------------------------------

		void	HandleDeviceAttach( const ::HANDLE deviceHandle );

	// - DATA MEMBERS ------------------------------------

		::Eldritch2::ChildAllocator									_allocator;
		const ::HHOOK												_keyboardHook;
		Utility::ReaderWriterUserMutex*								_deviceDirectoryMutex;
		::Eldritch2::FlatOrderedMap<::HANDLE, RawInputSubscriber*>	_deviceDirectory;
		RawInputPollingThread										_pollingThread;
	};

}	// namespace Input
}	// namespace Eldritch2