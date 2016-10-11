/*==================================================================*\
  TerrainCollider.hpp
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

namespace Eldritch2 {
	namespace Scripting {
		class	ApiRegistrar;
	}

	namespace Physics {
		namespace PhysX {
			namespace AssetViews {
				class	TerrainView;
			}
		}
	}
}

namespace physx {
	class	PxRigidStatic;
	class	PxScene;
}

namespace Eldritch2 {
namespace Physics {
namespace PhysX {
namespace ScriptComponents {

	class TerrainCollider {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//! Constructs this @ref TerrainCollider instance.
		TerrainCollider( PhysX::UniquePointer<physx::PxRigidStatic> actor, const AssetViews::TerrainView& asset );
	//!	Disable copying.
		TerrainCollider( const TerrainCollider& ) = delete;

		~TerrainCollider() = default;

	// - SCRIPT API REFERENCE ----------------------------

	public:
	//!	Registers all script-callable methods for the @ref TerrainCollider type with the specified script type registrar.
		static void	RegisterScriptApi( Scripting::ApiRegistrar& typeRegistrar );

	// ---------------------------------------------------

	public:
		void	Enable();

		void	Disable();

	// ---------------------------------------------------

	//!	Disable assignment.
		TerrainCollider&	operator=( const TerrainCollider& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		const PhysX::UniquePointer<physx::PxRigidStatic>	_actor;
		const AssetViews::TerrainView*						_asset;
	};

}	// namespace ScriptComponents
}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2