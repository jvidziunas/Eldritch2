/*==================================================================*\
  WrenWorldComponent.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Wren/Context.hpp>
#include <Core/WorldComponent.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scripting {
namespace Wren {
	class Dispatcher;
	class Game;
}} // namespace Scripting::Wren

namespace Assets {
	class AsynchronousImport;
	class ContentLocator;
} // namespace Assets
} // namespace Eldritch2

namespace Eldritch2 { namespace Scripting { namespace Wren {

	class WrenWorldComponent : public Core::WorldComponent {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref WrenWorldComponent instance.
		WrenWorldComponent(const ObjectLocator& services);
		//!	Disable copy construction.
		WrenWorldComponent(const WrenWorldComponent&) = delete;

		~WrenWorldComponent() = default;

		// - WORLD COMPONENT SANDBOX METHODS -----------------

	protected:
		void BindResourcesEarly(Scheduling::JobExecutor& executor) override;

		void BindResources(Scheduling::JobExecutor& executor) override;

		void FreeResources(Scheduling::JobExecutor& executor) override;

		// - WORLD COMPONENT SANDBOX METHODS -----------------

	protected:
		void OnVariableRateTick(Scheduling::JobExecutor& executor, MicrosecondTime tickDuration, float32 residualFraction) override;

		void OnFixedRateTick(Scheduling::JobExecutor& executor, MicrosecondTime delta) override;

		// - WORLD COMPONENT SANDBOX METHODS -----------------

	protected:
		void DefineScriptApi(Scripting::Wren::ApiBuilder& api) override;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		WrenWorldComponent& operator=(const WrenWorldComponent&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		Context                     _wren;
		Assets::ContentLocator*     _contentLocator;
		Assets::AsynchronousImport* _requiredPackages;
		Dispatcher*                 _dispatcher;
		Game*                       _game;
	};

}}} // namespace Eldritch2::Scripting::Wren
