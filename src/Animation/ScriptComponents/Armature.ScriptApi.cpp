/*==================================================================*\
  Armature.ScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Animation/ScriptComponents/Armature.hpp>
#include <Scripting/ApiRegistrar.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Scripting;

namespace Eldritch2 {
namespace Animation {
namespace ScriptComponents {

	void Armature::RegisterScriptApi( ApiRegistrar& registrar ) {
		registrar.DefineClass<Armature>(	{/* Constructors */},
											{/*	Properties */
												DefineGetter( "RootTransform", Armature::GetRootTransform )
											},
											{/*	Methods */},
											{/*	Operators */},
											{/*	Fields */} );
	}

}	// namespace ScriptComponents
}	// namespace Animation
}	// namespace Eldritch2



