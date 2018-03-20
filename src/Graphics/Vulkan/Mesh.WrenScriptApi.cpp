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
namespace {

	using namespace ::Eldritch2::Graphics::Vulkan::AssetViews;
	using namespace ::Eldritch2::Scripting::Wren;
	using namespace ::Eldritch2::Animation;
	using namespace ::Eldritch2::Scripting;

	struct StaticMesh : public Graphics::Mesh {
	public:
	//!	Constructs this @ref StaticMesh instance.
		StaticMesh( const Armature& armature, const MeshAsset& asset ) : Graphics::Mesh( armature, asset ), asset( asset ) {}
	//!	Disable copy construction.
		StaticMesh( const StaticMesh& ) = delete;

		~StaticMesh() = default;

	public:
		const MeshAsset&	asset;
	};

// ---

	struct Mesh : public StaticMesh {
	//!	Constructs this @ref Mesh instance.
		Mesh( const Armature& armature, const MeshAsset& asset ) : StaticMesh( armature, asset ) {}
	//!	Disable copy construction.
		Mesh( const Mesh& ) = delete;

		~Mesh() = default;
	};

}	// anonymous namespace

	ET_IMPLEMENT_WREN_CLASS( StaticMesh ) {
		api.CreateClass<StaticMesh>( ET_BUILTIN_WREN_MODULE_NAME( Graphics ), "StaticMesh",
			{/* Constructors */
				DefineConstructor<Mesh ( VulkanGraphicsScene, Armature, AssetReference<MeshAsset> )>( "new", [] ( WrenVM* vm ) {
					SetReturn<Mesh>( vm, GetSlotAs<Armature>( vm, 2 ), GetSlotAs<AssetReference<MeshAsset>>( vm, 3 ) );
				} )
			},
			{/*	Properties */},
			{/*	Methods */},
			{/*	Operators */}
		);
	}

	ET_IMPLEMENT_WREN_CLASS( Mesh ) {
		api.CreateClass<Mesh>( ET_BUILTIN_WREN_MODULE_NAME( Graphics ), "Mesh",
			{/* Constructors */
				DefineConstructor<Mesh ( VulkanGraphicsScene, Armature, AssetReference<MeshAsset> )>( "new", [] ( WrenVM* vm ) {
					SetReturn<Mesh>( vm, GetSlotAs<Armature>( vm, 2 ), GetSlotAs<AssetReference<MeshAsset>>( vm, 3 ) );
				} )
			},
			{/*	Properties */},
			{/*	Methods */},
			{/*	Operators */}
		);
	}

}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2