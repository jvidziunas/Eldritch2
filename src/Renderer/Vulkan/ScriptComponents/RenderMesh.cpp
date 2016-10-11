/*==================================================================*\
  RenderMesh.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/Vulkan/ScriptComponents/RenderMesh.hpp>
#include <Renderer/Vulkan/AssetViews/MeshView.hpp>
#include <Animation/ScriptComponents/Armature.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Animation;
using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Assets;

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {
namespace ScriptComponents {

	RenderMesh::RenderMesh( const Animation::ScriptComponents::Armature& armature, const AssetViews::MeshView& asset ) : _armature( &armature ), _asset( &asset ) {}

// ---------------------------------------------------

	RenderMesh::RenderMesh( const RenderMesh& meshRenderer ) : _armature( meshRenderer._armature ), _asset( meshRenderer._asset ) {}

// ---------------------------------------------------

	RenderMesh::~RenderMesh() {}

}	// namespace ScriptComponents
}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2
