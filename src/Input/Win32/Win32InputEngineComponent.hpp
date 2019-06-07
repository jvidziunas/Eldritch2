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
#include <Input/Win32/DeviceCoordinator.hpp>
#include <Core/EngineComponent.hpp>
#include <Core/WorldComponent.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Input {
	class DeviceLocator;
}} // namespace Eldritch2::Input

using HHOOK = struct HHOOK__*;
using HWND  = struct HWND__*;

namespace Eldritch2 { namespace Input { namespace Win32 {

	class Win32InputEngineComponent : public Core::EngineComponent {
		// - TYPE PUBLISHING ---------------------------------

	public:
		class WorldComponent : public Core::WorldComponent {
			// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref WorldComponent instance.
			WorldComponent(const ObjectInjector& services) ETNoexceptHint;
			//!	Disable copy construction.
			WorldComponent(const WorldComponent&) = delete;

			~WorldComponent() = default;

			// - WORLD COMPONENT SANDBOX METHODS -----------------

		public:
			void BindResources(Scheduling::JobExecutor& executor) ETNoexceptHint override;

			void FreeResources(Scheduling::JobExecutor& executor) ETNoexceptHint override;

			// ---------------------------------------------------

		public:
			void PublishApi(Scripting::Wren::ApiBuilder&) ETNoexceptHint override;

			using Core::WorldComponent::PublishApi;

			// ---------------------------------------------------

			//!	Disable copy assignment.
			WorldComponent& operator=(const WorldComponent&) = delete;

			// - DATA MEMBERS ------------------------------------

		private:
			DeviceLocator* _keyboards;
			DeviceLocator* _mice;
		};

		// ---

	public:
		class ReaderThread : public Thread {
			// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref ReaderThread instance.
			ReaderThread(DeviceCoordinator& devices) ETNoexceptHint;
			//!	Disable copy construction.
			ReaderThread(const ReaderThread&) = delete;

			~ReaderThread() = default;

			// ---------------------------------------------------

		public:
			void SetShouldShutDown() ETNoexceptHint override;

			Result EnterOnCaller() ETNoexceptHint override sealed;

			// ---------------------------------------------------

			//!	Disable copy assignment.
			ReaderThread& operator=(const ReaderThread&) = delete;

			// - DATA MEMBERS ------------------------------------

		private:
			/*!	Win32 raw input requires a window for input registration. The handle is exposed primarily for the
				engine component to issue a shutdown request to the sampling thread when the application terminates,
				as there is effectively no need to push state to the window. */
			Atomic<HWND>       _window;
			DeviceCoordinator* _devices;
		};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		Win32InputEngineComponent(const Win32InputEngineComponent&) = delete;
		//!	Constructs this @ref Win32InputEngineComponent instance.
		Win32InputEngineComponent(const ObjectInjector& services) ETNoexceptHint;

		~Win32InputEngineComponent();

		// - ENGINE SERVICE SANDBOX METHODS ------------------

	public:
		void BindResourcesEarly(Scheduling::JobExecutor& executor) ETNoexceptHint override;

		// ---------------------------------------------------

	public:
		void PublishApi(ObjectInjector& services) override;

		using EngineComponent::PublishApi;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		Win32InputEngineComponent& operator=(const Win32InputEngineComponent&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		HHOOK             _keyboardHook;
		DeviceCoordinator _devices;
		ReaderThread      _inputReader;
	};

}}} // namespace Eldritch2::Input::Win32
