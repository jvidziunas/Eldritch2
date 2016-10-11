/*==================================================================*\
  RenderMesh.ScriptApi.cpp
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
#include <Scripting/ApiRegistrar.hpp>
#include <Assets/AssetLibrary.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Animation;
using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Assets;

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {
namespace ScriptComponents {
namespace {

	void ConstructRenderMesh( void* thisPointer, const AssetLibrary& assetLibrary, const Animation::ScriptComponents::Armature& armature, const Utf8String<>& assetName ) {
		const auto	asset( assetLibrary.ResolveView<AssetViews::MeshView>( assetName ) );

		new(thisPointer) RenderMesh( armature, *asset );
	}

}	// anonymous namespace

	void RenderMesh::RegisterScriptApi( ApiRegistrar& registrar ) {
		registrar.DefineClass<RenderMesh>(	{/* Constructors */
												DefineOverloadedConstructor( void (*)( void*, Injected<const AssetLibrary&>, const Animation::ScriptComponents::Armature&, const Utf8String<>& ), ConstructRenderMesh )
											},
											{/*	Properties */},
											{/*	Methods */},
											{/*	Operators */},
											{/*	Fields */} );
	}

}	// namespace ScriptComponents
}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2



