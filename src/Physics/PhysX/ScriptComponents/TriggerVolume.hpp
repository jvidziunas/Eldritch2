/*==================================================================*\
  TriggerVolume.hpp
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
#include <PxRigidStatic.h>
#include <PxShape.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Scripting {
		class	ApiRegistrar;
	}
}

namespace Eldritch2 {
namespace Physics {
namespace PhysX {
namespace ScriptComponents {

	class TriggerVolume {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref TriggerVolume instance.
		TriggerVolume( PhysX::UniquePointer<physx::PxRigidStatic> actor, PhysX::UniquePointer<physx::PxShape> shape );
	//!	Disable copying.
		TriggerVolume( const TriggerVolume& ) = delete;

		~TriggerVolume() = default;

	// - SCRIPT API REFERENCE ----------------------------

	public:
	//!	Registers all script-callable methods for the @ref TriggerVolume type with the specified script type registrar.
		static void	RegisterScriptApi( Scripting::ApiRegistrar& typeRegistrar );

	// ---------------------------------------------------

	public:
		void	Enable();

		void	Disable();

	// ---------------------------------------------------

	//!	Disable copying.
		TriggerVolume&	operator=( const TriggerVolume& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		const PhysX::UniquePointer<physx::PxRigidStatic>	_actor;
		PhysX::UniquePointer<physx::PxShape>				_collisionShape;
	};

}	// namespace ScriptComponents
}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2