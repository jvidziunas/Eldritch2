/*==================================================================*\
  MeshAsset.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/AssetViews/MeshAsset.hpp>
//------------------------------------------------------------------//
#include <Flatbuffers/Mesh_generated.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan { namespace AssetViews {

	using namespace ::Eldritch2::Graphics::FlatBuffers;
	using namespace ::Eldritch2::Logging;
	using namespace ::Eldritch2::Assets;
	using namespace ::flatbuffers;

	MeshAsset::MeshAsset(StringView path) :
		Asset(path),
		_attributes(MallocAllocator("Mesh Asset Attribute Collection Allocator")),
		_surfaces(MallocAllocator("Mesh Asset Element Collection Allocator")) {
	}

	// ---------------------------------------------------

	void MeshAsset::Stream(const VertexStreamRequest& /*request*/) const ETNoexceptHint {}

	// ---------------------------------------------------

	void MeshAsset::Stream(const IndexStreamRequest& /*request*/) const ETNoexceptHint {}

	// ---------------------------------------------------

	ErrorCode MeshAsset::BindResources(const Builder& asset) {
		//	Verify the data we're working with can plausibly represent a mesh.
		Verifier verifier(reinterpret_cast<const uint8_t*>(asset.Begin()), asset.GetSize());
		if (!VerifyMeshBuffer(verifier)) {
			asset.WriteLog(Severity::Error, "Mesh {} is malformed!" ET_NEWLINE, GetPath());
			return Error::Unspecified;
		}

		ArrayList<Attribute>          attributes(_attributes.GetAllocator());
		ArrayList<SurfaceDescription> surfaces(_surfaces.GetAllocator());

		Swap(_attributes, attributes);
		Swap(_surfaces, surfaces);

		return Error::None;
	}

	// ---------------------------------------------------

	void MeshAsset::FreeResources() {
		_attributes.Clear(ReleaseMemorySemantics());
		_surfaces.Clear(ReleaseMemorySemantics());
	}

	// ---------------------------------------------------

	ETPureFunctionHint StringView MeshAsset::GetExtension() ETNoexceptHint {
		return { MeshExtension(), StringLength(MeshExtension()) };
	}

}}}} // namespace Eldritch2::Graphics::Vulkan::AssetViews
