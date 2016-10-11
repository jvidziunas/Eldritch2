/*==================================================================*\
  CollisionMessage.ScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/PhysX/ScriptMessages/CollisionMessage.hpp>
#include <Scripting/ApiRegistrar.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Scripting;

namespace Eldritch2 {
namespace Physics {
namespace PhysX {
namespace ScriptMessages {
namespace {

	void ConstructCollisionMessage( void* thisPointer, float32 force ) {
		new(thisPointer) CollisionMessage( force );
	}

}	// anonymous namespace

	void CollisionMessage::RegisterScriptApi( ApiRegistrar& registrar ) {
		registrar.DefineClass<CollisionMessage>(	{/* Constructors */
														DefineConstructor( ConstructCollisionMessage )
													},
													{/*	Properties */},
													{/*	Methods */},
													{/*	Operators */},
													{/*	Fields */
														DefineField( "force", CollisionMessage::force )
													} );
	}

}	// namespace ScriptMessages
}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2