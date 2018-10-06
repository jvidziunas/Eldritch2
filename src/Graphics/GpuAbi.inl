/*==================================================================*\
  GpuAbi.inl
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

	ETInlineHint ETForceInlineHint MeshSurface::MeshSurface(GpuPrimitive type, uint32 firstIndex, uint32 primitiveCount, StringView pipeline) ETNoexceptHint : type(type),
																																							   firstIndex(firstIndex),
																																							   primitiveCount(primitiveCount) {
		pipeline.Copy(this->pipeline, ETCountOf(this->pipeline));
	}

}} // namespace Eldritch2::Graphics
