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

	ETInlineHint ETForceInlineHint Meshlet::Meshlet(GpuPrimitive primitive, MeshLargeIndex primitiveCount, MeshLargeIndex firstIndex, StringSpan pipeline) ETNoexceptHint : primitive(primitive),
																																											firstIndex(firstIndex),
																																											primitiveCount(primitiveCount) {
		pipeline.Copy(this->pipeline, ETCountOf(this->pipeline));
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint ETPureFunctionHint size_t GetHashCode(ImageTile tile, size_t seed = 0u) ETNoexceptHint {
		return ((tile.x * 73856093u) ^ (tile.y * 19349663u) ^ (tile.z * 83492791u)) + tile.subresource + 31u * seed;
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint ETPureFunctionHint bool operator<(ImageTile lhs, ImageTile rhs) ETNoexceptHint {
		return reinterpret_cast<const uint64&>(lhs) < reinterpret_cast<const uint64&>(rhs);
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint ETPureFunctionHint bool operator==(ImageTile lhs, ImageTile rhs) ETNoexceptHint {
		return reinterpret_cast<const uint64&>(lhs) == reinterpret_cast<const uint64&>(rhs);
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint ETPureFunctionHint bool operator!=(ImageTile left, ImageTile right) ETNoexceptHint {
		return !(left == right);
	}

}} // namespace Eldritch2::Graphics
