/*==================================================================*\
  Mesh.WrenScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/AssetViews/MeshAsset.hpp>
#include <Graphics/Vulkan/VulkanGraphicsScene.hpp>
#include <Scripting/Wren/AssetReference.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
#include <Animation/Armature.hpp>
//------------------------------------------------------------------//
#include <wren.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {

	using namespace ::Eldritch2::Graphics::Vulkan::AssetViews;
	using namespace ::Eldritch2::Scripting::Wren;
	using namespace ::Eldritch2::Animation;
	using namespace ::Eldritch2::Scripting;

	ET_IMPLEMENT_WREN_CLASS( StaticMeshInstance ) {
		class StaticMeshInstance : public Graphics::MeshInstance {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
		//!	Constructs this @ref StaticMeshInstance instance.
			ETInlineHint StaticMeshInstance( const Armature& armature, const MeshAsset& asset ) : Graphics::MeshInstance( armature, asset ) {}
		//!	Disable copy construction.
			StaticMeshInstance( const StaticMeshInstance& ) = delete;

			~StaticMeshInstance() = default;
		};

		api.CreateClass<StaticMeshInstance>( ET_BUILTIN_WREN_MODULE_NAME( Graphics ), "StaticMesh",
			{/* Constructors */
				DefineConstructor<MeshInstance ( VulkanGraphicsScene, Armature, AssetReference<MeshAsset> )>( "new", [] ( WrenVM* vm ) {
					SetReturn<MeshInstance>( vm, GetSlotAs<Armature>( vm, 2 ), GetSlotAs<AssetReference<MeshAsset>>( vm, 3 ) );
				} )
			},
			{/*	Properties */},
			{/*	Methods */},
			{/*	Static methods */},
			{/*	Operators */}
		);
	}

	ET_IMPLEMENT_WREN_CLASS( MeshInstance ) {
		class MeshInstance : public Graphics::MeshInstance {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
		//!	Constructs this @ref MeshInstance instance.
			ETInlineHint MeshInstance( const Armature& armature, const MeshAsset& asset ) : Graphics::MeshInstance( armature, asset ) {}
		//!	Disable copy construction.
			MeshInstance( const MeshInstance& ) = delete;

			~MeshInstance() = default;
		};

		api.CreateClass<MeshInstance>( ET_BUILTIN_WREN_MODULE_NAME( Graphics ), "Mesh",
			{/* Constructors */
				DefineConstructor<MeshInstance ( VulkanGraphicsScene, Armature, AssetReference<MeshAsset> )>( "new", [] ( WrenVM* vm ) {
					SetReturn<MeshInstance>( vm, GetSlotAs<Armature>( vm, 2 ), GetSlotAs<AssetReference<MeshAsset>>( vm, 3 ) );
				} )
			},
			{/*	Properties */},
			{/*	Methods */},
			{/*	Static methods */},
			{/*	Operators */}
		);
	}

}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2