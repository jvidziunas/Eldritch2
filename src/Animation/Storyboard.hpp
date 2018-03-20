/*==================================================================*\
  Storyboard.hpp
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

namespace Eldritch2 {
namespace Animation {

	struct StoryboardEvent {
		uint32	time;
	};

// ---

	class Storyboard {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref Storyboard instance.
		Storyboard();

		~Storyboard() = default;

	// - DATA MEMBERS ------------------------------------

	private:
		ArrayList<StoryboardEvent>	_events;
	};

}	// namespace Animation
}	// namespace Eldritch2