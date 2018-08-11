/*==================================================================*\
  XInputWorldComponent.hpp
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

namespace Eldritch2 { namespace Input { namespace XInput {

	class XInputWorldComponent : public Core::WorldComponent {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		XInputWorldComponent(const XInputWorldComponent&) = delete;
		//!	Constructs this @ref XInputWorldComponent instance.
		XInputWorldComponent(const ObjectLocator& services);

		~XInputWorldComponent() = default;

		// ---------------------------------------------------

	public:
		void DefineScriptApi(Scripting::Wren::ApiBuilder& api) override;
	};

}}} // namespace Eldritch2::Input::XInput
