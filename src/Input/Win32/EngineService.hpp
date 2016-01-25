/*==================================================================*\
  EngineService.hpp
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
namespace Win32 {

	class EngineService : public Foundation::GameEngineService {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref EngineService instance.
		EngineService( Foundation::GameEngine& owningEngine );

		//!	Destroys this @ref EngineService instance.
		~EngineService();

	// ---------------------------------------------------

		const ::Eldritch2::UTF8Char* const	GetName() const override sealed;

	// ---------------------------------------------------

	protected:
		void	AcceptInitializationVisitor( Scripting::ScriptAPIRegistrationInitializationVisitor& visitor ) override sealed;

		void	AcceptTaskVisitor( Scheduler::WorkerContext& executingContext, Scheduler::WorkerContext::FinishCounter& finishCounter, const PreConfigurationLoadedTaskVisitor ) override sealed;

	// - TYPE PUBLISHING ---------------------------------

	private:
		class RawInputPollingThread : public Scheduler::Thread {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref RawInputPollingThread instance.
			RawInputPollingThread( EngineService& hostingService );

			//!	Destroys this @ref RawInputPollingThread instance.
			~RawInputPollingThread() = default;

		// ---------------------------------------------------

			const ::Eldritch2::UTF8Char* const	GetHumanReadableName() const override sealed;

		// ---------------------------------------------------

			void	Run() override sealed;

			void	RequestGracefulShutdown() override;

		// - DATA MEMBERS ------------------------------------

		private:
			EngineService&	_hostingInputService;
			::DWORD			_threadID;
		};

	// ---

		class ETPureAbstractHint RawInputSubscriber {
		// ---------------------------------------------------

		public:
			//!	Converts a generic Win32 raw input packet into HID-specific input events.
			/*!	@param[in] inputPacket Packet to parse.
				*/
			virtual void	ReadInputPacket( const ::RAWINPUT& inputPacket ) abstract;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		protected:
			//!	Constructs this @ref RawInputSubscriber instance.
			RawInputSubscriber() = default;

		public:
			//!	Destroys this @ref RawInputSubscriber instance.
			virtual ~RawInputSubscriber() = default;
		};

	// ---

		class Keyboard : public RawInputSubscriber, public Input::Keyboard {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref Keyboard instance.
			Keyboard( const ::HANDLE deviceHandle, EngineService& owner );

			//!	Destroys this @ref Keyboard instance.
			~Keyboard() = default;

		// ---------------------------------------------------

			void	Dispose() override sealed;

		// ---------------------------------------------------

			void	ReadInputPacket( const ::RAWINPUT& inputPacket ) override sealed;
		};

	// ---

		class Mouse : public RawInputSubscriber, public Input::Mouse {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref Mouse instance.
			Mouse( const ::HANDLE deviceHandle, EngineService& owner );

			//!	Destroys this @ref Mouse instance.
			~Mouse() = default;

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

}	// namespace Win32
}	// namespace Input
}	// namespace Eldritch2