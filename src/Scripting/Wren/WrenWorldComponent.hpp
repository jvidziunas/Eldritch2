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
	class PackageManager;
	class AssetManager;
} // namespace Assets
} // namespace Eldritch2

struct WrenVM;

namespace Eldritch2 { namespace Scripting { namespace Wren {

	class WrenWorldComponent : public Core::WorldComponent {
		// - TYPE PUBLISHING ---------------------------------

	public:
		enum : size_t { WrenHeapSizeInBytes = 2u * 1024u * 1024u /* 2MB */ };

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		WrenWorldComponent(const WrenWorldComponent&) = delete;
		//!	Constructs this @ref WrenWorldComponent instance.
		WrenWorldComponent(const Core::World& owner);

		~WrenWorldComponent() = default;

		// - WORLD COMPONENT SANDBOX METHODS -----------------

	public:
		void AcceptVisitor(Scheduling::JobExecutor& executor, const EarlyInitializationVisitor) override;
		void AcceptVisitor(Scheduling::JobExecutor& executor, const LateInitializationVisitor) override;
		void AcceptVisitor(Scheduling::JobExecutor& executor, const StandardTickVisitor&) override;
		void AcceptVisitor(Scheduling::JobExecutor& executor, const VariableTickVisitor&) override;
		void AcceptVisitor(Scheduling::JobExecutor& executor, const TearDownVisitor) override;
		void AcceptVisitor(Scripting::Wren::ApiBuilder& api) override;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		WrenWorldComponent& operator=(const WrenWorldComponent&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		//!	Wren virtual machine that will execute script code for the world.
		WrenVM* _vm;
		Context _wren;
		Game    _game;

		Assets::PackageManager* _packages;
		Assets::AssetManager*   _assets;

		//!	Central dispatch point for in-world events.
		Dispatcher* _dispatcher;
	};

}}} // namespace Eldritch2::Scripting::Wren
