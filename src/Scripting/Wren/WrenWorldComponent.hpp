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
#include <Scripting/Wren/Game.hpp>
#include <Core/WorldComponent.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scripting {
namespace Wren {
	class Dispatcher;
}} // namespace Scripting::Wren

namespace Assets {
	class PackageLocator;
	class AssetLocator;
} // namespace Assets
} // namespace Eldritch2

struct WrenVM;

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
		//!	Wren virtual machine that will execute script code for the world.
		WrenVM*                 _vm;
		Context                 _wren;
		Game                    _game;
		Assets::PackageLocator* _packageLocator;
		Assets::AssetLocator*   _assetLocator;
		Dispatcher*             _dispatcher;
	};

}}} // namespace Eldritch2::Scripting::Wren
