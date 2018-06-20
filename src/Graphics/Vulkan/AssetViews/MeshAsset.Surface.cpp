/*==================================================================*\
  MeshAsset.Surface.cpp
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

namespace Eldritch2 { namespace Graphics { namespace Vulkan { namespace AssetViews {

	MeshAsset::Surface::Surface(
		const Utf8Char* const pipeline,
		uint32                indexOffset,
		uint32                faceCount,
		uint16                indicesPerFace,
		Bounds                bounds) :
		faceCount(faceCount),
		indicesPerFace(indicesPerFace),
		indexOffset(indexOffset),
		bounds(bounds) {
		CopyString(pipelineName, pipeline);
	}

}}}} // namespace Eldritch2::Graphics::Vulkan::AssetViews
