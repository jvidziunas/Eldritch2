/*==================================================================*\
  XInputEngineComponent.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Core/EngineComponent.hpp>
#include <Core/WorldComponent.hpp>
//------------------------------------------------------------------//
#include <XInput.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Input {
	class DeviceLocator;
}} // namespace Eldritch2::Input

namespace Eldritch2 { namespace Input { namespace XInput {

	class XInputEngineComponent : public Core::EngineComponent {
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

			// ---------------------------------------------------

		public:
			void BindResources(Scheduling::JobExecutor& executor) ETNoexceptHint override;

			// ---------------------------------------------------

		public:
			void PublishApi(Scripting::Wren::ApiBuilder&) override;

			using Core::WorldComponent::PublishApi;

			// ---------------------------------------------------

			//!	Disable copy assignment.
			WorldComponent& operator=(const WorldComponent&) = delete;

			// - DATA MEMBERS ------------------------------------

		private:
			DeviceLocator* _gamepads;
		};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref XInputEngineComponent instance.
		XInputEngineComponent(const ObjectInjector& services) ETNoexceptHint;
		//!	Disable copy construction.
		XInputEngineComponent(const XInputEngineComponent&) = delete;

		~XInputEngineComponent() = default;

		// - ENGINE SERVICE SANDBOX METHODS ------------------

	public:
		void TickEarly(Scheduling::JobExecutor& executor) ETNoexceptHint override;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		XInputEngineComponent& operator=(const XInputEngineComponent&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		XINPUT_STATE _gamepads[4];
	};

}}} // namespace Eldritch2::Input::XInput
