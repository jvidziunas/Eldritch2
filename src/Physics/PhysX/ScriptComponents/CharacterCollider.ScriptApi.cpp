/*==================================================================*\
  CharacterCollider.ScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/PhysX/ScriptComponents/CharacterCollider.hpp>
#include <Physics/PhysX/ScriptComponents/SkeletalArmature.hpp>
#include <Physics/PhysX/TypeMarshalers.hpp>
#include <Scripting/ApiRegistrar.hpp>
//------------------------------------------------------------------//
#if ET_COMPILER_IS_MSVC
#	pragma warning( push )
//	(6326) MSVC doesn't like some of the compile-time constant comparison PhysX does. We can't fix this, but we can at least disable the warning.
#	pragma warning( disable : 6326 )
#endif
#include <characterkinematic/PxControllerManager.h>
#if ET_COMPILER_IS_MSVC
#	pragma warning( pop )
#endif
//------------------------------------------------------------------//

using namespace ::Eldritch2::Scripting;
using namespace ::physx;

namespace Eldritch2 {
namespace Physics {
namespace PhysX {
namespace ScriptComponents {
namespace {

	void ConstructCharacterCollider( void* thisPointer, PxControllerManager& controllerManager, ScriptComponents::SkeletalArmature& armature ) {
		PxCapsuleControllerDesc	desc;
		const auto				transform( armature.GetRootTransform() );

	//	Copy transformation data into the controller.
		desc.upDirection	= PxVec3FromFloat4( transform.orientation.RotateVector( { 0.0f, 1.0f, 0.0f, 0.0f } ) );
		desc.position		= PxExtendedVec3FromFloat4( transform.translation );

		desc.climbingMode	= PxCapsuleClimbingMode::eCONSTRAINED;
		
		new(thisPointer) CharacterCollider( static_cast<PxCapsuleController*>(controllerManager.createController( desc )), armature );
	}

}	// anonymous namespace

	void CharacterCollider::RegisterScriptApi( ApiRegistrar& registrar ) {
		registrar.DefineClass<CharacterCollider>(	{/* Constructors */
														DefineOverloadedConstructor( void (*)( void*, Injected<PxControllerManager&>, ScriptComponents::SkeletalArmature& ), ConstructCharacterCollider )
													},
													{/*	Properties */
														DefineGetter( "FootPosition", CharacterCollider::GetFootPosition )
													},
													{/*	Methods */},
													{/*	Operators */},
													{/*	Fields */} );
	}

}	// namespace ScriptComponents
}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2



