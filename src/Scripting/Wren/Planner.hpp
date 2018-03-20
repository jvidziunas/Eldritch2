/*==================================================================*\
  WrenPlanner.hpp
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

struct	WrenHandle;

namespace Eldritch2 {
namespace Scripting {
namespace Wren {

	class Action {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref Action instance.
		Action( WrenHandle* action );
	//!	Disable copy construction.
		Action( const Action& ) = delete;
	//!	Constructs this @ref Action instance.
		Action( Action&& );

		~Action();

	// - DATA MEMBERS ------------------------------------

	protected:
		WrenHandle*	_action;
	};

// ---

	class Planner : public GoapPlanner<Action, float32> {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Disable copy construction.
		Planner( const Planner& ) = delete;
	//!	Constructs this @ref Planner instance.
		Planner();

		~Planner() = default;
	};

}	// namespace Wren
}	// namespace Scripting
}	// namespace Eldritch2