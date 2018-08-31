/*==================================================================*\
  MeshSource.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics {

	ETInlineHint MeshSource::SurfaceDescription::SurfaceDescription(StringView pipeline, GpuPrimitive type, uint32 firstIndex, uint32 primitiveCount, Bounds bounds) ETNoexceptHint : type(type),
																																													  firstIndex(firstIndex),
																																													  primitiveCount(primitiveCount),
																																													  bounds(bounds) {
		pipeline.Copy(pipelineName, ETCountOf(pipelineName));
	}

}} // namespace Eldritch2::Graphics
