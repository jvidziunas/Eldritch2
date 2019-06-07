/*==================================================================*\
  WrenEngineComponent.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Wren/ScriptExecutor.hpp>
#include <Core/EngineComponent.hpp>
#include <Core/WorldComponent.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scripting {
namespace Wren {
	class Dispatcher;
	class Game;
}} // namespace Scripting::Wren

namespace Core {
	class AsynchronousImport;
	class ContentLocator;
} // namespace Core
} // namespace Eldritch2

namespace Eldritch2 { namespace Scripting { namespace Wren {

	class WrenEngineComponent : public Core::EngineComponent {
		// - TYPE PUBLISHING ---------------------------------

	public:
		class WorldComponent : public Core::WorldComponent {
			// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref WrenWorldComponent instance.
			WorldComponent(const ObjectInjector& services) ETNoexceptHint;
			//!	Disable copy construction.
			WorldComponent(const WorldComponent&) = delete;

			~WorldComponent() = default;

			// - WORLD COMPONENT SANDBOX METHODS -----------------

		public:
			void BindResourcesEarly(Scheduling::JobExecutor& executor) ETNoexceptHint override;

			void BindResources(Scheduling::JobExecutor& executor) ETNoexceptHint override;

			void FreeResources(Scheduling::JobExecutor& executor) ETNoexceptHint override;

			// - WORLD COMPONENT SANDBOX METHODS -----------------

		public:
			void OnVariableRateTick(Scheduling::JobExecutor& executor, MicrosecondTime tickDuration, float32 residualFraction) ETNoexceptHint override;

			void OnFixedRateTick(Scheduling::JobExecutor& executor, MicrosecondTime delta) ETNoexceptHint override;

			// - WORLD COMPONENT SANDBOX METHODS -----------------

		public:
			void PublishApi(Scripting::Wren::ApiBuilder& api) override;

			using Core::WorldComponent::PublishApi;

			// ---------------------------------------------------

			//!	Disable copy assignment.
			WorldComponent& operator=(const WorldComponent&) = delete;

			// - DATA MEMBERS ------------------------------------

		private:
			ScriptExecutor            _wren;
			Core::ContentLocator*     _content;
			Core::AsynchronousImport* _worldPackages;
			Dispatcher*               _dispatcher;
			Game*                     _game;
		};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref WrenEngineComponent instance.
		WrenEngineComponent(const ObjectInjector& services) ETNoexceptHint;
		//!	Disable copy construction.
		WrenEngineComponent(const WrenEngineComponent&) = delete;

		~WrenEngineComponent() = default;

		// - ENGINE SERVICE SANDBOX METHODS ------------------

	public:
		void PublishApi(Core::AssetApiBuilder& api) override;

		using EngineComponent::PublishApi;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		WrenEngineComponent& operator=(const WrenEngineComponent&) = delete;
	};

}}} // namespace Eldritch2::Scripting::Wren
