/*==================================================================*\
  Planner.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

struct WrenHandle;
struct WrenVM;

namespace Eldritch2 { namespace Scripting { namespace Wren {

	class Tactic {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref Tactic instance.
		Tactic(WrenHandle* action) ETNoexceptHint;
		//!	Disable copy construction.
		Tactic(const Tactic&) = delete;
		//!	Constructs this @ref Tactic instance.
		Tactic(Tactic&&) ETNoexceptHint;

		~Tactic();

		// ---------------------------------------------------

		//!	Disable copy assignment.
		Tactic& operator=(const Tactic&) = delete;

		// - DATA MEMBERS ------------------------------------

	protected:
		WrenHandle* _action;

		// ---------------------------------------------------

		friend void Swap(Tactic&, Tactic&);
	};

	// ---

	class Planner : public GoapPlanner<Tactic, float32> {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		Planner(const Planner&) = delete;
		//!	Constructs this @ref Planner instance.
		Planner();

		~Planner() = default;

		// ---------------------------------------------------

	public:
		void BindResources(WrenVM* vm);

		void FreeResources(WrenVM* vm);
	};

}}} // namespace Eldritch2::Scripting::Wren
