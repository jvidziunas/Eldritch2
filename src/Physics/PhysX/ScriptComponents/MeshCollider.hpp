/*==================================================================*\
  MeshCollider.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/CountedPointer.hpp>
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

			namespace AssetViews {
				class	SkeletalColliderView;
			}
		}
	}
}

namespace Eldritch2 {
namespace Physics {
namespace PhysX {
namespace ScriptComponents {

	class MeshCollider {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//! Constructs this @ref MeshCollider instance.
		MeshCollider( ScriptComponents::SkeletalArmature& armature, const AssetViews::SkeletalColliderView& asset );
	//! Constructs this @ref MeshCollider instance.
		MeshCollider( const MeshCollider& );

		~MeshCollider();

	// - SCRIPT API REFERENCE ----------------------------

	public:
	//! Registers all script-callable methods for the @ref MeshCollider type with the specified script type registrar.
		static void	RegisterScriptApi( Scripting::ApiRegistrar& typeRegistrar );

	// ---------------------------------------------------

	public:
		void	Enable();

		void	Disable();

	// - DATA MEMBERS ------------------------------------

	private:
		ScriptComponents::SkeletalArmature&		_armature;
		const AssetViews::SkeletalColliderView*	_asset;
	};

}	// namespace ScriptComponents
}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2