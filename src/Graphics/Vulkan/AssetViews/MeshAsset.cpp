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

	namespace {

		ETConstexpr SkinnedVertex MeshVertices[] = {
			{ { 0.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 0.0f } },
			{ { 0.0f, 1.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 0.0f } },
			{ { 1.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 0.0f } }
		};

		// ---

		ETConstexpr uint16 MeshIndices[] = { 0u, 1u, 2u };

		// ---

		ETConstexpr Box MeshBounds({ 0.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f });

	} // anonymous namespace

	MeshAsset::MeshAsset(StringView path) :
		Asset(path),
		_surfaces(MallocAllocator("Mesh Asset Element Collection Allocator")) {
	}

	// ---------------------------------------------------

	MeshDescription MeshAsset::GetDescription() const ETNoexceptHint {
		return MeshDescription { uint32(_surfaces.GetSize()), sizeof(MeshVertices), sizeof(MeshIndices) };
	}

	// ---------------------------------------------------

	void MeshAsset::Stream(MeshElementRequest<StridingIterator<SkinnedVertex>> request) const ETNoexceptHint {
		while (request.first != request.last) {
			CopyMemoryNonTemporal(ETAddressOf(*request.out++), ETAddressOf(MeshVertices[request.first++]), sizeof(MeshVertices[0]));
		}
	}

	// ---------------------------------------------------

	void MeshAsset::Stream(MeshElementRequest<MeshIndex*> request) const ETNoexceptHint {
		CopyMemoryNonTemporal(request.out, MeshIndices + request.first, (request.last - request.first) * sizeof(*MeshIndices));
	}

	// ---------------------------------------------------

	ErrorCode MeshAsset::BindResources(const Builder& asset) {
		//	Verify the data we're working with can plausibly represent a mesh.
		Verifier verifier(reinterpret_cast<const uint8_t*>(asset.Begin()), asset.GetSize());
		if (!VerifyMeshBuffer(verifier)) {
			asset.WriteLog(Severity::Error, "Mesh {} is malformed!" ET_NEWLINE, GetPath());
			return Error::Unspecified;
		}

		ArrayList<MeshSurface> surfaces(_surfaces.GetAllocator(), { MeshSurface(GpuPrimitive::Triangle, 0u, ETCountOf(MeshIndices), MeshBounds, "lol") });

		Swap(_surfaces, surfaces);

		return Error::None;
	}

	// ---------------------------------------------------

	void MeshAsset::FreeResources() {
		_surfaces.Clear(ReleaseMemorySemantics());
	}

	// ---------------------------------------------------

	ETPureFunctionHint StringView MeshAsset::GetExtension() ETNoexceptHint {
		return { MeshExtension(), StringLength(MeshExtension()) };
	}

}}}} // namespace Eldritch2::Graphics::Vulkan::AssetViews
