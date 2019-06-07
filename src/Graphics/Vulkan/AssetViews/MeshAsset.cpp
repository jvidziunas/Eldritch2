/*==================================================================*\
  MeshAsset.cpp
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
#include <Flatbuffers/FlatBufferTools.hpp>
#include <Logging/Log.hpp>
//------------------------------------------------------------------//
#include <Flatbuffers/Mesh_generated.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan { namespace AssetViews {

	using namespace ::Eldritch2::Logging;
	using namespace ::Eldritch2::Core;

	namespace {

		ETConstexpr SkinnedVertex MeshVertices[] = {
			{ { 0.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 0.0f } },
			{ { 0.0f, 1.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 0.0f } },
			{ { 1.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 0.0f } }
		};

		// ---

		ETConstexpr uint16 MeshIndices[] = { 0u, 1u, 2u };

	} // anonymous namespace

	MeshAsset::MeshAsset(StringSpan path) ETNoexceptHint : Asset(path), _meshlets(MallocAllocator("Mesh Asset Element Collection Allocator")) {}

	// ---------------------------------------------------

	MeshDescriptor MeshAsset::DescribeSelf() const ETNoexceptHint {
		return MeshDescriptor{
			uint32(_meshlets.GetSize()),
			/*indexStream =*/0u,
			/*streamSizes =*/{ sizeof(MeshIndices), sizeof(MeshVertices) }
		};
	}

	// ---------------------------------------------------

	void MeshAsset::Stream(MeshElementRequest request) const ETNoexceptHint {
		switch (request.stream) {
		case 0:
			Copy(MeshIndices + request.first, MeshIndices + request.last, reinterpret_cast<MeshIndex*>(request.out));
			break;
		case 1:
			Copy(MeshVertices + request.first, MeshVertices + request.last, reinterpret_cast<SkinnedVertex*>(request.out));
			break;
		}
	}

	// ---------------------------------------------------

	Result MeshAsset::BindResources(Log& log, const AssetBuilder& asset) {
		using namespace ::Eldritch2::Graphics::FlatBuffers;
		using namespace ::flatbuffers;

		//	Verify the data we're working with can plausibly represent a mesh.
		const auto mesh(GetVerifiedRoot<FlatBuffers::MeshDescriptor>(asset.bytes, MeshDescriptorIdentifier()));
		ET_ABORT_UNLESS(mesh ? Result::Success : Result::InvalidParameter, log.Write(Error, "Data integrity check failed for {}, aborting load." ET_NEWLINE, GetPath()));

		ArrayList<Meshlet> surfaces(_meshlets.GetAllocator(), { Meshlet(GpuPrimitive::Triangle, 0u, ETCountOf(MeshIndices), "lol") });
		Swap(_meshlets, surfaces);

		return Result::Success;
	}

	// ---------------------------------------------------

	void MeshAsset::FreeResources() ETNoexceptHint {
		_meshlets.Clear(ReleaseMemorySemantics());
	}

	// ---------------------------------------------------

	ETPureFunctionHint StringSpan MeshAsset::GetExtension() ETNoexceptHint {
		using namespace ::Eldritch2::Graphics::FlatBuffers;

		return StringSpan(MeshDescriptorExtension(), StringSpan::SizeType(StringLength(MeshDescriptorExtension())));
	}

}}}} // namespace Eldritch2::Graphics::Vulkan::AssetViews
