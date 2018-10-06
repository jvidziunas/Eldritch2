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
#include <Assets/ContentLocator.hpp>
//------------------------------------------------------------------//
#include <wren.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	using namespace ::Eldritch2::Graphics::Vulkan::AssetViews;
	using namespace ::Eldritch2::Scripting::Wren;
	using namespace ::Eldritch2::Animation;
	using namespace ::Eldritch2::Assets;

	namespace {

		class StaticMeshInstance : public MeshInstance {
			// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref StaticMeshInstance instance.
			ETInlineHint ETForceInlineHint StaticMeshInstance(const Armature& armature, const MeshAsset& asset) :
				MeshInstance(armature, asset) {}
			//!	Disable copy construction.
			StaticMeshInstance(const StaticMeshInstance&) = delete;

			~StaticMeshInstance() = default;
		};

		// ---

		class MeshInstance : public Graphics::MeshInstance {
			// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref MeshInstance instance.
			ETInlineHint ETForceInlineHint MeshInstance(const Armature& armature, const MeshAsset& asset) :
				Graphics::MeshInstance(armature, asset) {}
			//!	Disable copy construction.
			MeshInstance(const MeshInstance&) = delete;

			~MeshInstance() = default;
		};

	} // anonymous namespace

	ET_IMPLEMENT_WREN_CLASS(StaticMeshInstance) {
		api.DefineClass<StaticMeshInstance>(ET_BUILTIN_WREN_MODULE_NAME(Graphics), "StaticMesh", // clang-format off
			{ /* Static methods */
				ForeignMethod("new(_,_,_)", [](WrenVM* vm) ETNoexceptHint {
					auto&      scene(GetSlotAs<VulkanGraphicsScene>(vm, 1));
					const auto asset(Cast<MeshAsset>(GetSlotAs<AssetReference>(vm, 3)));
					ET_ABORT_WREN_UNLESS(asset, "Asset must be a MeshAsset.");

					SetReturn<StaticMeshInstance>(vm, /*classSlot =*/0, GetSlotAs<Armature>(vm, 2), *asset);
				}), },
			{ /* Methods */ }); // clang-format on
	}

	ET_IMPLEMENT_WREN_CLASS(MeshInstance) {
		api.DefineClass<MeshInstance>(ET_BUILTIN_WREN_MODULE_NAME(Graphics), "Mesh", // clang-format off
			{ /* Static methods */
				ForeignMethod("new(_,_,_)", [](WrenVM* vm) ETNoexceptHint {
					auto&      scene(GetSlotAs<VulkanGraphicsScene>(vm, 1));
					const auto asset(Cast<MeshAsset>(GetSlotAs<AssetReference>(vm, 3)));
					ET_ABORT_WREN_UNLESS(asset, "Asset must be a MeshAsset.");

					SetReturn<MeshInstance>(vm, /*classSlot =*/0, GetSlotAs<Armature>(vm, 2), *asset);
				}), },
			{ /* Methods */ }); // clang-format on
	}

}}} // namespace Eldritch2::Graphics::Vulkan
