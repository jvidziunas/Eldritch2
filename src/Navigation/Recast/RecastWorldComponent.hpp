/*==================================================================*\
  RecastWorldComponent.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Core/WorldComponent.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Navigation { namespace Recast {
	class NavigationScene;
}}} // namespace Eldritch2::Navigation::Recast

namespace Eldritch2 { namespace Navigation { namespace Recast {

	class RecastWorldComponent : public Core::WorldComponent {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref RecastWorldComponent instance.
		RecastWorldComponent(const ObjectLocator& services);
		//!	Disable copy construction.
		RecastWorldComponent(const RecastWorldComponent&) = delete;

		~RecastWorldComponent() = default;

		// ---------------------------------------------------

	public:
		void OnFixedRateTickLate(Scheduling::JobExecutor& executor, MicrosecondTime duration) override;

		// ---------------------------------------------------

	public:
		void DefineScriptApi(Scripting::Wren::ApiBuilder& api) override;

		// - DATA MEMBERS ------------------------------------

	private:
		NavigationScene* _scene;
	};

}}} // namespace Eldritch2::Navigation::Recast
