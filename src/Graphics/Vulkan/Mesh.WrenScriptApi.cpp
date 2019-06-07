/*==================================================================*\
  Mesh.WrenScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/AssetViews/MeshAsset.hpp>
#include <Graphics/Vulkan/VulkanGraphicsScene.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
#include <Core/ContentDatabase.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	using namespace ::Eldritch2::Animation;
	using namespace ::Eldritch2::Core;

	// ---------------------------------------------------

	namespace {

		class MeshInstance : public RenderMesh {
			// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref MeshInstance instance.
			ETForceInlineHint MeshInstance(const Armature& armature, const MeshSource& asset) ETNoexceptHint : RenderMesh(armature, asset) {}
			//!	Disable copy construction.
			MeshInstance(const MeshInstance&) = delete;

			~MeshInstance() = default;
		};

	} // anonymous namespace

	ET_IMPLEMENT_WREN_CLASS(MeshInstance, api, moduleName) {
		using namespace ::Eldritch2::Graphics::Vulkan::AssetViews;
		using namespace ::Eldritch2::Scripting::Wren;

		api.DefineClass<MeshInstance>("Mesh", moduleName, // clang-format off
			{ /* Static methods */
				ForeignMethod("new(_,_)", [](WrenVM* vm) ETNoexceptHint {
					const auto asset(Get<MeshAsset>(wrenGetSlotAs<AssetReference>(vm, 2)));
					ET_ABORT_WREN_IF(asset == nullptr, vm, "Asset must be a MeshAsset.");

					wrenSetReturn<MeshInstance>(vm, /*classSlot =*/0, wrenGetSlotAs<Armature>(vm, 1), *asset);
				}), },
			{ /* Methods */ }); // clang-format on
	}

}}} // namespace Eldritch2::Graphics::Vulkan
