/*==================================================================*\
  Win32InputWorldComponent.hpp
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

namespace Eldritch2 { namespace Input { namespace Win32 {

	class Win32InputWorldComponent : public Core::WorldComponent {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		Win32InputWorldComponent(const Win32InputWorldComponent&) = delete;
		//!	Constructs this @ref Win32InputWorldComponent instance.
		Win32InputWorldComponent(const Core::World& owner);

		~Win32InputWorldComponent() = default;

		// - WORLD COMPONENT SANDBOX METHODS -----------------

	public:
		void AcceptVisitor(Scripting::Wren::ApiBuilder& api) override;
	};

}}} // namespace Eldritch2::Input::Win32
