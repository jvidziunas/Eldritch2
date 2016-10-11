/*==================================================================*\
  CharacterCollider.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/PhysX/SmartPointers.hpp>
//------------------------------------------------------------------//
#if ET_COMPILER_IS_MSVC
#	pragma warning( push )
//	(6326) MSVC doesn't like some of the compile-time constant comparison PhysX does. We can't fix this, but we can at least disable the warning.
#	pragma warning( disable : 6326 )
#endif
#include <characterkinematic/PxCapsuleController.h>
#if ET_COMPILER_IS_MSVC
#	pragma warning( pop )
#endif
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Scripting {
		class	ApiRegistrar;
	}

	namespace Physics {
		namespace PhysX {
			namespace ScriptComponents {
				class	SkeletalArmature;
			}
		}
	}

	class	Float4;
}

namespace Eldritch2 {
namespace Physics {
namespace PhysX {
namespace ScriptComponents {

	class CharacterCollider {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//! Constructs this @ref CharacterCollider instance.
		CharacterCollider( PhysX::UniquePointer<physx::PxCapsuleController> controller, ScriptComponents::SkeletalArmature& armature );
	//! Constructs this @ref CharacterCollider instance.
		CharacterCollider( const CharacterCollider& ) = delete;

		~CharacterCollider() = default;

	// - SCRIPT API REFERENCE ----------------------------

	public:
	//! Registers all script-callable methods for the @ref CharacterCollider type with the specified script type registrar.
		static void	RegisterScriptApi( Scripting::ApiRegistrar& typeRegistrar );

	// ---------------------------------------------------

	public:
		Eldritch2::Float4 ETSimdCall	GetFootPosition() const;

	// - DATA MEMBERS ------------------------------------

	private:
		PhysX::UniquePointer<physx::PxCapsuleController>	_controller;
		ScriptComponents::SkeletalArmature&					_armature;
	};

}	// namespace ScriptComponents
}	// namespace PhysX
}	// namespace Renderer
}	// namespace Eldritch2