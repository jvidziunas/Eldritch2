/*==================================================================*\
  BipedalAgent.hpp
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
	namespace Navigation {
		namespace Recast {
			class	NavigationScene;
		}
	}
	namespace Animation {
		class	Armature;
	}
}

namespace Eldritch2 {
namespace Navigation {
namespace Recast {

	class BipedalAgent {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref BipedalAgent instance.
		BipedalAgent( Animation::Armature& armature );
	//!	Disable copy construction.
		BipedalAgent( const BipedalAgent& ) = delete;
	//!	Constructs this @ref BipedalAgent instance.
		BipedalAgent( BipedalAgent&& );

		~BipedalAgent();

	// ---------------------------------------------------

	public:
		bool ETSimdCall	BeginNavigation( NavigationScene& scene, Vector targetPosition );

		void			StopNavigation();

	// - DATA MEMBERS ------------------------------------

	private:
		Animation::Armature*	_armature;
	//!	@ref NavigationScene that owns the agent.
		NavigationScene*		_scene;
	//!	Agent index for this entity within the crowd.
		int						_id;
	};

}	// namespace Recast
}	// namespace Navigation
}	// namespace Eldritch2