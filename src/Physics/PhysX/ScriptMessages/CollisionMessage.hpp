/*==================================================================*\
  CollisionMessage.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Mpl/FloatTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Scripting {
		class	ApiRegistrar;
	}
}

namespace Eldritch2 {
namespace Physics {
namespace PhysX {
namespace ScriptMessages {

	class CollisionMessage {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref CollisionMessage instance.
		CollisionMessage( Eldritch2::float32 force );
	//!	Constructs this @ref CollisionMessage instance.
		CollisionMessage( const CollisionMessage& ) = default;

		~CollisionMessage() = default;

	// - SCRIPT API REFERENCE ----------------------------

	public:
	//! Registers all script-callable methods for the @ref CollisionMessage type with the specified script type registrar.
		static void	RegisterScriptApi( Scripting::ApiRegistrar& registrar );

	// - DATA MEMBERS ------------------------------------

	public:
		Eldritch2::float32	force;
	};

}	// namespace ScriptMessages
}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2