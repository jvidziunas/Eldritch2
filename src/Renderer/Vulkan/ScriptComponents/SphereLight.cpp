/*==================================================================*\
  SphereLight.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/Vulkan/ScriptComponents/SphereLight.hpp>
#include <Animation/ScriptComponents/Armature.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {
namespace ScriptComponents {

	SphereLight::SphereLight( const Animation::ScriptComponents::Armature& armature, Float4 color ) : _armature( &armature ), _color( color ) {}

// ---------------------------------------------------

	SphereLight::SphereLight( const SphereLight& other ) : _armature( other._armature ), _color( other._color ) {}

// ---------------------------------------------------

	SphereLight::~SphereLight() {}

}	// namespace ScriptComponents
}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2


