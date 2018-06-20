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
#include <Scripting/Wren/ApiBuilder.hpp>
#include <Assets/AssetManager.hpp>
#include <Animation/Armature.hpp>
//------------------------------------------------------------------//
#include <wren.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	using namespace ::Eldritch2::Graphics::Vulkan::AssetViews;
	using namespace ::Eldritch2::Scripting::Wren;
	using namespace ::Eldritch2::Animation;
	using namespace ::Eldritch2::Scripting;
	using namespace ::Eldritch2::Assets;

	ET_IMPLEMENT_WREN_CLASS(StaticMeshInstance) {
		class StaticMeshInstance : public Graphics::MeshInstance {
			// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref StaticMeshInstance instance.
			ETInlineHint StaticMeshInstance(const Armature& armature, const MeshAsset& asset) :
				Graphics::MeshInstance(armature, asset) {}
			//!	Disable copy construction.
			StaticMeshInstance(const StaticMeshInstance&) = delete;

			~StaticMeshInstance() = default;
		};

		// clang-format off
		api.CreateClass<StaticMeshInstance>(ET_BUILTIN_WREN_MODULE_NAME(Graphics), "StaticMesh",
			{ /*	Constructors */
				ConstructorMethod("new(_,_,_)", [](WrenVM* vm) {
					VulkanGraphicsScene&   scene(GetSlotAs<VulkanGraphicsScene>(vm, 1));
					const MeshAsset* const asset(Cast<MeshAsset>(GetSlotAs<AssetReference>(vm, 3)));

					ET_ABORT_WREN_UNLESS(asset, "Asset must be a MeshAsset.");

					SetReturn<MeshInstance>(vm, GetSlotAs<Armature>(vm, 2), *asset);
				})
			},
			{ /*	Static methods */ },
			{ /*	Properties */ },
			{ /*	Methods */ }
		); // clang-format on
	}

	ET_IMPLEMENT_WREN_CLASS(MeshInstance) {
		class MeshInstance : public Graphics::MeshInstance {
			// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref MeshInstance instance.
			ETInlineHint MeshInstance(const Armature& armature, const MeshAsset& asset) :
				Graphics::MeshInstance(armature, asset) {}
			//!	Disable copy construction.
			MeshInstance(const MeshInstance&) = delete;

			~MeshInstance() = default;
		};

		// clang-format off
		api.CreateClass<MeshInstance>(ET_BUILTIN_WREN_MODULE_NAME(Graphics), "Mesh",
			{ /*	Constructors */
				ConstructorMethod("new(_,_,_)", [](WrenVM* vm) {
				VulkanGraphicsScene&   scene(GetSlotAs<VulkanGraphicsScene>(vm, 1));
				const MeshAsset* const asset(Cast<MeshAsset>(GetSlotAs<AssetReference>(vm, 3)));

				ET_ABORT_WREN_UNLESS(asset, "Asset must be a MeshAsset.");

				SetReturn<MeshInstance>(vm, GetSlotAs<Armature>(vm, 2), *asset);
				})
			},
			{ /*	Static methods */ },
			{ /*	Properties */ },
			{ /*	Methods */ }
		); // clang-format on
	}

}
}
} // namespace Eldritch2::Graphics::Vulkan
